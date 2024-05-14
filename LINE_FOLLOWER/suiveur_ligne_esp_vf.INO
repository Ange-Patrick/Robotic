// Writed by Patrick WABO
// Engineer and PhD Student
// Fields : Automation and Artifical Intelligence
// Mail : waboangepatrick@gmail.com
// Linkedin : www.linkedin.com/in/ange-patrick-wabo

// If you have any question, feel free to contact me :).
 
 #include <SoftwareSerial.h>
 #include "Wire.h"

// Moteurs (motors)
const int pin_vitesse_gauche = 25; 
const int pin_vitesse_droite = 33; 
const int moteurs_gauches_1 = 23;
const int moteurs_gauches_2 = 19;
const int moteurs_droites_1 = 32;
const int moteurs_droites_2 = 18;

// setting PWM properties
const int freq = 100000;
const int gauche_channel = 0;
const int droite_channel = 1;
const int resolution = 8;

//Capteurs suiveurs de ligne 
const int capteur_ir[]= {26,27,14,12,13}; // Ir sensors pins from the left  to the right 
int valeur_ir[5]; // sensor values list
int mesure;

//regulation PID
float Kp = 50; 
float Ki = 0;
float Kd = 0;
float P = 0;
float I = 0;
float D = 0;
float erreur = 0;
float erreur_precedente = 0;
float valeur_PID;

//Vitesses (speed)
const int vitesse_defaut = 180; //(default_speed)
const int vitesse_max = 250;
const int diff = 15; //vitesse_droit - vitesse_gauche observee experimentalement (difference between rigth motor speed and left motor speed, observed by experiment)

String cas_speciale = "";

void setup() {
  
  Serial.begin(115200);

  ledcSetup(gauche_channel, freq, resolution);
  ledcSetup(droite_channel, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pin_vitesse_gauche, gauche_channel);
  ledcAttachPin(pin_vitesse_droite, droite_channel);
  
  pinMode(moteurs_gauches_1, OUTPUT);
  pinMode(moteurs_gauches_2, OUTPUT);
  pinMode(moteurs_droites_1, OUTPUT);
  pinMode(moteurs_droites_2, OUTPUT);
  
  for (int i=0; i<5; i++){
    pinMode(capteur_ir[i], INPUT);
  }
  
  digitalWrite(moteurs_gauches_1, LOW); 
  digitalWrite(moteurs_gauches_2, HIGH);
  digitalWrite(moteurs_droites_1, LOW);
  digitalWrite(moteurs_droites_2, HIGH);
  Serial.println("DEBUT");
}

void loop() {mesure_ligne();
  calcul_pid();
  control_moteur();
  Serial.println("turn");
  double temps_debut = millis();
  if (cas_speciale == "turn"){ //virage 90 degre et plus
    Serial.println("turn");
    delay(100);
  }  
  else if (cas_speciale == "forward"){
    Serial.println("turn");
    while(abs(erreur)>2){
      mesure_ligne();
    }
  }
  else{
    delay(10);
  }
  cas_speciale = "";
  Serial.println(erreur);
}



void mesure_ligne(){  //collect sensor measurements and compute the error value
  for(int i = 0; i < 5; i++) { 
    valeur_ir[i] = digitalRead(capteur_ir[i]); 
  }
  
  if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==1 &&  valeur_ir[3]== 0 &&  valeur_ir[4]==0) erreur = 0;
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==1 &&  valeur_ir[3]==1 &&  valeur_ir[4]==0) erreur = 1;
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==0 &&  valeur_ir[3]==1 &&  valeur_ir[4]==0) erreur = 2;
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==0 &&  valeur_ir[3]==1 &&  valeur_ir[4]==1) {erreur = 4; cas_speciale="turn";}
  
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==0 &&  valeur_ir[3]==0 &&  valeur_ir[4]==1) {erreur = 4; cas_speciale = "turn"; }
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==1 &&  valeur_ir[3]==1 &&  valeur_ir[4]==1) {erreur = 4; cas_speciale = "turn"; }
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==1 &&  valeur_ir[2]==1 &&  valeur_ir[3]==1 &&  valeur_ir[4]==1) {erreur = 4; cas_speciale = "turn"; }
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==1 &&  valeur_ir[3]==0 &&  valeur_ir[4]==1) {erreur = 4; cas_speciale = "turn"; } 
  
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==1 &&  valeur_ir[2]==1 &&  valeur_ir[3]==0 &&  valeur_ir[4]==0) erreur = -1;
  else if ( valeur_ir[0]==0 &&  valeur_ir[1]==1 &&  valeur_ir[2]==0 &&  valeur_ir[3]==0 &&  valeur_ir[4]==0) erreur = -2;
  else if ( valeur_ir[0]==1 &&  valeur_ir[1]==1 &&  valeur_ir[2]==0 &&  valeur_ir[3]==0 &&  valeur_ir[4]==0) {erreur = -4; cas_speciale = "turn";}
  
  else if ( valeur_ir[0]==1 &&  valeur_ir[1]==0 &&  valeur_ir[2]==0 &&  valeur_ir[3]==0 &&  valeur_ir[4]==0) {erreur = -4; cas_speciale = "turn";}  
  else if ( valeur_ir[0]==1 &&  valeur_ir[1]==1 &&  valeur_ir[2]==1 &&  valeur_ir[3]==0 &&  valeur_ir[4]==0) {erreur = -4; cas_speciale = "turn";} 
  else if ( valeur_ir[0]==1 &&  valeur_ir[1]==1 &&  valeur_ir[2]==1 &&  valeur_ir[3]==1 &&  valeur_ir[4]==0) {erreur = -4; cas_speciale = "turn";} 
  else if ( valeur_ir[0]==1 &&  valeur_ir[1]==0 &&  valeur_ir[2]==1 &&  valeur_ir[3]==0 &&  valeur_ir[4]==0) {erreur = -4; cas_speciale = "turn";} 
  
  else if ( valeur_ir[0]==1 &&  valeur_ir[1]==1 &&  valeur_ir[2]==1 &&  valeur_ir[3]==1  &&  valeur_ir[4]==1) {erreur=0;  cas_speciale="forward";}  
  //else if ( valeur_ir[0]==0 &&  valeur_ir[1]==0 &&  valeur_ir[2]==0 &&  valeur_ir[3]==0 &&  valeur_ir[4]==0)
}

void calcul_pid(){ //Compute PID value
  P = erreur;
  I += erreur;
  D = erreur - erreur_precedente;
  valeur_PID = Kp*P + Ki*I + Kd*D;  
  erreur_precedente = erreur;
}

void control_moteur(){  // Apply PID regulation
  //calcul de la vitesse de chaque moteur (comute the speed of each motors)
  int vitesse_moteur_gauche = vitesse_defaut + valeur_PID;
  int vitesse_moteur_droite = vitesse_defaut - valeur_PID;
  
  //The line below are the secret that helped us to win :)
  if(vitesse_moteur_gauche >= 0){
    if(vitesse_moteur_gauche > vitesse_max) vitesse_moteur_gauche = vitesse_max;
    digitalWrite(moteurs_gauches_1, LOW); 
    digitalWrite(moteurs_gauches_2, HIGH);
  }
  else{
    vitesse_moteur_gauche = 180;
    digitalWrite(moteurs_gauches_1, HIGH); 
    digitalWrite(moteurs_gauches_2, LOW);
    Serial.println("gauche arriere");
  }
  
  if(vitesse_moteur_droite>=0){
    digitalWrite(moteurs_droites_1, LOW); 
    digitalWrite(moteurs_droites_2, HIGH);
    if(vitesse_moteur_droite > vitesse_max) vitesse_moteur_droite = vitesse_max;
  }  
  else{
    vitesse_moteur_droite = 180;
    digitalWrite(moteurs_droites_1, HIGH); 
    digitalWrite(moteurs_droites_2, LOW);
    Serial.println("droite arriere");
  }
  
  ledcWrite(gauche_channel, abs(vitesse_moteur_gauche)); 
  ledcWrite(droite_channel, abs(vitesse_moteur_droite)); 

}

void mise_arret(){  // Stop the car
  ledcWrite(gauche_channel, 200); 
  ledcWrite(droite_channel, 200);    
  digitalWrite(moteurs_gauches_1, LOW);
  digitalWrite(moteurs_gauches_2, HIGH);
  digitalWrite(moteurs_droites_1, LOW);
  digitalWrite(moteurs_droites_2, HIGH); 
  delay(20);
  ledcWrite(gauche_channel, 0); 
  ledcWrite(droite_channel, 0); 
  while(1){
    //
  }
}


