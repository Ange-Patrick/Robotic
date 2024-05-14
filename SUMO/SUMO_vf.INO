// Writed by Patrick WABO
// Engineer and PhD Student
// Fields : Automation and Artifical Intelligence
// Mail : waboangepatrick@gmail.com
// Linkedin : www.linkedin.com/in/ange-patrick-wabo

// If you have any question, feel free to contact me :).


// Moteurs (Motors)
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

//sensors
const int pin_ir_avant = 26; // Ir sensors placed in front the car
const int pin_ir_arriere = 27; // Ir sensors placed on the rear of the car
const int trig_avant = 16; // ultrasoud trig
const int echo_avant = 4; // ultrasound echo

int temps_debut = 0;
double delais;

const int attack_distance = 40; // Minimum distance required for an attack

void setup() {
  Serial.begin(115200);
  
  ledcSetup(channel_gauche, freq, resolution);
  ledcSetup(channel_droite, freq, resolution);
  
  ledcAttachPin(pin_vitesse_gauche, channel_gauche);
  ledcAttachPin(pin_vitesse_droite, channel_droite);  
  pinMode(moteurs_gauches_1, OUTPUT);
  pinMode(moteurs_gauches_2, OUTPUT);
  pinMode(moteurs_droites_1, OUTPUT);
  pinMode(moteurs_droites_2, OUTPUT);
  
  pinMode(pin_ir_avant, INPUT);
  pinMode(pin_ir_arriere, INPUT);
  pinMode(trig_avant, OUTPUT);
  pinMode(echo_avant, INPUT);
  
  start_routine();
  
}

void loop() {
  Serial.print(digitalRead(pin_ir_avant));Serial.print("/");Serial.println(mesure(trig_avant, echo_avant));
  search();
}

void start_routine(){   // Set the first behavior when the car is placed in the ring
  temps_debut = millis();
  //gauche();
  //delay(500); //6.5Volts
  avant();
  ledcWrite(channel_gauche, 255); 
  ledcWrite(channel_droite, 255);
  while(digitalRead(pin_ir_avant) == 1){
    //pass
  }
  arriere();
  delais = millis()-temps_debut;
  delay(int(delais/3));
  arret();  
}

void search(){  // look for an opponent inside the ring
  avant();
  ledcWrite(channel_gauche, 220); 
  ledcWrite(channel_droite, 0);  
  while(mesure(trig_avant, echo_avant) <= attack_distance && digitalRead(pin_ir_avant) == 1 && digitalRead(pin_ir_arriere) == 1){
    attack();    
  }
  back_off();
  delayMicroseconds(10);
}

void attack(){ // if detect an opponent then attack
  avant();
  int vitesse = 255;
  if(mesure(trig_avant, echo_avant)>30) vitesse = 200;
  ledcWrite(channel_gauche, vitesse); 
  ledcWrite(channel_droite, vitesse);  
}

void back_off(){   // run away from the line in the opposite direction to avoid leaving the ring
  
  if(digitalRead(pin_ir_avant) == 0){ // The line is detected by the sensor which is in front
    arret();
    arriere();    
    ledcWrite(channel_gauche, 255); 
    ledcWrite(channel_droite, 255);  
    delay(int(delais/3));
    arret();
  }
  else if(digitalRead(pin_ir_arriere) == 0){// The line is detected by the sensor on the rear
    arret();
    avant();    
    ledcWrite(channel_gauche, 255); 
    ledcWrite(channel_droite, 255);  
    delay(int(delais/3));
    arret();
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

void avant(){ // forward
    digitalWrite(moteurs_gauches_1, LOW);
    digitalWrite(moteurs_gauches_2, HIGH);
    digitalWrite(moteurs_droites_1, LOW);
    digitalWrite(moteurs_droites_2, HIGH); 
}

void arret(){  //stop
  arriere();delay(10); //decommentez lorsque la vitesse augmente.
  ledcWrite(channel_gauche, 0); 
  ledcWrite(channel_droite, 0); 
  digitalWrite(moteurs_gauches_1, LOW);
  digitalWrite(moteurs_gauches_2, LOW);
  digitalWrite(moteurs_droites_1, LOW);
  digitalWrite(moteurs_droites_2, LOW); 
  delay(40); 
}

void gauche(){ //Left
  digitalWrite(moteurs_gauches_1, HIGH);
  digitalWrite(moteurs_gauches_2, LOW);
  digitalWrite(moteurs_droites_1, LOW);
  digitalWrite(moteurs_droites_2, HIGH); 
  ledcWrite(channel_gauche, 255); 
  ledcWrite(channel_droite, 255); 
}

void droite(){ //right
  digitalWrite(moteurs_gauches_1, LOW);
  digitalWrite(moteurs_gauches_2, HIGH);
  digitalWrite(moteurs_droites_1, HIGH);
  digitalWrite(moteurs_droites_2, LOW); 
  ledcWrite(channel_gauche, 255); 
  ledcWrite(channel_droite, 255); 
}

void arriere(){ //back
  digitalWrite(moteurs_gauches_1, HIGH);
  digitalWrite(moteurs_gauches_2, LOW);
  digitalWrite(moteurs_droites_1, HIGH);
  digitalWrite(moteurs_droites_2, LOW); 
  ledcWrite(channel_gauche, 200); 
  ledcWrite(channel_droite, 200); 
}


