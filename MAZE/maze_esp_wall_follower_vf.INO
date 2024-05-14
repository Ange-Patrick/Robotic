// Writed by Patrick WABO
// Engineer and PhD Student
// Fields : Automation and Artifical Intelligence
// Mail : waboangepatrick@gmail.com
// Linkedin : www.linkedin.com/in/ange-patrick-wabo

// If you have any question, feel free to contact me :).

//This code is one of our greatest successes during the competition. it took us many days of work to obtain a robust solution.
// it can be upgrated to enhance the efficiency. We didn't do it, but if we decide to take part in the Elviatec Olympics again we will.
// I encourage you to watch an extract of the video showing how the robot works.


// Moteurs (motors)
const int pin_vitesse_gauche = 25; 
const int pin_vitesse_droite = 33; 
const int moteurs_gauches_1 = 23;
const int moteurs_gauches_2 = 19;
const int moteurs_droites_1 = 32;
const int moteurs_droites_2 = 18;

// setting PWM properties
const int freq = 100000;
const int channel_gauche = 0;
const int channel_droite = 1;
const int resolution = 8;

// distance sensor
const int trig_gauche = 2;
const int echo_gauche = 15;
const int trig_avant = 16;
const int echo_avant = 4;
const int trig_droite = 5 ;
const int echo_droite = 17;
const int trig_droite_rotation = 22;
const int echo_droite_rotation = 36;

//const int ir_sensor = 26;


// function variable
const int side_distance_treshold = 20; 
const int front_distance_treshold = 10;
const int vitesse_avance = 200;   //forward speed
const int vitesse_rotation = 255;  // turn speed

//regulation
const int consigne_distance = 9;
const float Kp = 16 ; 
const float Ki = 0;
const float Kd = 1;
float P = 0;
float I = 0;
float D = 0;
float erreur = 0;
float erreur_precedente = 0;
float valeur_PID;



void setup() {  
  Serial.begin(115200);
  
  ledcSetup(channel_gauche, freq, resolution);
  ledcSetup(channel_droite, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(pin_vitesse_gauche, channel_gauche);
  ledcAttachPin(pin_vitesse_droite, channel_droite);  
  pinMode(moteurs_gauches_1, OUTPUT);
  pinMode(moteurs_gauches_2, OUTPUT);
  pinMode(moteurs_droites_1, OUTPUT);
  pinMode(moteurs_droites_2, OUTPUT);
  
  pinMode(trig_gauche, OUTPUT);
  pinMode(trig_avant, OUTPUT);
  pinMode(trig_droite, OUTPUT);
  pinMode(trig_droite_rotation, OUTPUT);
  pinMode(echo_gauche, INPUT);
  pinMode(echo_avant, INPUT);
  pinMode(echo_droite, INPUT);\
  pinMode(echo_droite_rotation, INPUT);\
  // Clears the trigPin condition
  digitalWrite(trig_gauche, LOW);digitalWrite(trig_avant, LOW);digitalWrite(trig_droite, LOW);
  
  //while(mesure(trig_gauche,echo_gauche) < 25){
    
  //}
  
}

void loop(){
  suivre_mur();
  
  float distance_avant =  mesure(trig_avant,echo_avant);  
  Serial.print("distance avant : ");Serial.println(distance_avant); 
  float distance_droite_rotation = mesure(trig_droite_rotation,echo_droite_rotation);   
  Serial.print("distance droite rotation: ");Serial.println(distance_droite_rotation); 
  int i=0;
  while(distance_avant < 8 || distance_droite_rotation < 7){
    if(distance_avant < 8){
      arret();
    }
    distance_avant =  mesure(trig_avant,echo_avant);
    distance_droite_rotation =  mesure(trig_droite_rotation,echo_droite_rotation);
    i++; 
    if(i==5){
      rotation();
      break;
    }
  } 
}



float mesure(int trigPin, int echoPin){ // Measuring distance using ultrasound sensors 
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duree = pulseIn(echoPin, HIGH);
  // Calculating the distance
  long distance = duree * 0.034 / 2; // Speed of sound wave divided by 2 (go and back); distance en Cm
  return(distance);
}


void suivre_mur(){ //follow the wall
  float distance_droite = 0;
  for (int i=0;i<3;i++){
    distance_droite +=  mesure(trig_droite,echo_droite)/3;
    delayMicroseconds(100);
  }  
  Serial.print("distance : ");Serial.println(distance_droite);  
  
  
  erreur = distance_droite - consigne_distance;
  P = erreur;
  I += erreur;
  D = erreur - erreur_precedente;  
  valeur_PID = Kp*P + Ki*I + Kd*D;
  
  if(distance_droite >= 25){
    float distance_droite_rotation = 0;
    for (int i=0;i<2;i++){
      distance_droite_rotation = mesure(trig_droite_rotation,echo_droite_rotation)/2;   
      delayMicroseconds(100);
    }  
    if (distance_droite_rotation > 20){
      contournement_mur();
      return;      
    }
  }
    
  int  vitesse_gauche = restraining(vitesse_avance + valeur_PID);
  int vitesse_droite = restraining(vitesse_avance - valeur_PID); // 15 represents the difference between left and rigth side motors
  
  
  Serial.print("Vg = "); Serial.println(vitesse_gauche);
  Serial.print("Vd = "); Serial.println(vitesse_droite);  
  ledcWrite(channel_gauche, vitesse_gauche); 
  ledcWrite(channel_droite, vitesse_droite);    
  digitalWrite(moteurs_gauches_1, LOW);
  digitalWrite(moteurs_gauches_2, HIGH);
  digitalWrite(moteurs_droites_1, LOW);
  digitalWrite(moteurs_droites_2, HIGH);  
}

void contournement_mur(){ // Bypass the wall
  Serial.println("Contournement");
  //arret(); delay(2000);
  if(mesure(trig_droite,echo_droite)){
    
  }
  while(1){
    float distance_droite = 0;
    //float distance_droite_rotation = 0;    
    for (int i=0;i<2;i++){
      distance_droite +=  mesure(trig_droite,echo_droite)/2;
      delayMicroseconds(100);
      //distance_droite_rotation = mesure(trig_droite_rotation,echo_droite_rotation)/2;   
      //delayMicroseconds(100);
    }      
    if(distance_droite < 13){
      break;
    }     
    /*
    //cas du ciment/carreaux
    digitalWrite(moteurs_gauches_1, LOW);
    digitalWrite(moteurs_gauches_2, HIGH);
    digitalWrite(moteurs_droites_1, LOW);
    digitalWrite(moteurs_droites_2, LOW); 
    ledcWrite(channel_gauche, 255); 
    ledcWrite(channel_droite, 255);*/
    
    //cas du contre plaquet
    digitalWrite(moteurs_gauches_1, LOW);
    digitalWrite(moteurs_gauches_2, HIGH);
    digitalWrite(moteurs_droites_1, HIGH);
    digitalWrite(moteurs_droites_2, LOW); 
    ledcWrite(channel_gauche, 255); 
    ledcWrite(channel_droite, 150);    
  }
}

int restraining(int vitesse){
  if(vitesse<0) return 0;
  else if(vitesse>255) return 255;
  else return vitesse;
}


void rotation(){ 
  gauche();
  while(1){
    float distance_avant = 0;
    float distance_droite_rotation = 0;
    for (int i=0;i<5;i++){
      distance_avant+=  mesure(trig_avant,echo_avant)/5;
      delayMicroseconds(100);
      distance_droite_rotation +=  mesure(trig_droite_rotation,echo_droite_rotation)/5;
      delayMicroseconds(100);
    }
    if(distance_avant>30 && distance_droite_rotation>10 ){
      break;
    }
  }
  //arret();
}


void arret(){  
  arriere();delay(10); //decommentez lorsque la vitesse augmente.
  ledcWrite(channel_gauche, 0); 
  ledcWrite(channel_droite, 0); 
  digitalWrite(moteurs_gauches_1, LOW);
  digitalWrite(moteurs_gauches_2, LOW);
  digitalWrite(moteurs_droites_1, LOW);
  digitalWrite(moteurs_droites_2, LOW); 
  delay(40); 
}

void gauche(){
  digitalWrite(moteurs_gauches_1, HIGH);
  digitalWrite(moteurs_gauches_2, LOW);
  digitalWrite(moteurs_droites_1, LOW);
  digitalWrite(moteurs_droites_2, HIGH); 
  ledcWrite(channel_gauche, 255); 
  ledcWrite(channel_droite, 255); 
}

void arriere(){
  digitalWrite(moteurs_gauches_1, HIGH);
  digitalWrite(moteurs_gauches_2, LOW);
  digitalWrite(moteurs_droites_1, HIGH);
  digitalWrite(moteurs_droites_2, LOW); 
  ledcWrite(channel_gauche, 200); 
  ledcWrite(channel_droite, 200); 
}
