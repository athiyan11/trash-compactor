#include <Arduino.h>
#include <NewPing.h>
#include <SoftwareSerial.h>


int TRIG_PIN  = 8; 
int ECHO_PIN  = 10; 
const int RELAY_PIN = 13;
const int RELAY_PIN2 = 12;
const int Flame_Pin = 7;
int IRSensor = 4;
SoftwareSerial sw(0, 1);

const int DISTANCE_THRESHOLD = 25; 
int dist;
int val;

float duration_us, distance_cm;

void setup() {
  Serial.begin (115200);
  pinMode(IRSensor,INPUT);
  pinMode(RELAY_PIN2,OUTPUT);       
  pinMode(TRIG_PIN, OUTPUT);  
  pinMode(ECHO_PIN, INPUT);   
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(Flame_Pin,INPUT);
  sw.begin(9600);
  
}

int IR_Val()
{
  int Sense_val = digitalRead(IRSensor);
  return Sense_val;
}
int Fal_Val()
{
  int Sense_val = digitalRead(Flame_Pin);
  return Sense_val;
}


int sonar_Distance() 
{
  
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

 
  duration_us = pulseIn(ECHO_PIN, HIGH);

  distance_cm = 0.017 * duration_us;
  return distance_cm ;
}


void loop ()
{
  dist = sonar_Distance();
  val = IR_Val();
  
  if(dist < DISTANCE_THRESHOLD && val == HIGH)
  {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(RELAY_PIN2, LOW); 
    Serial.println("LOOP 1"); 
    sw.println("Distance From Sonar ");
    sw.print(dist);
  }
    
  else if (val == LOW && dist > DISTANCE_THRESHOLD)
  {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(RELAY_PIN2, HIGH); 
    Serial.println("LOOP 2");

  }
  else if(val == LOW || dist > DISTANCE_THRESHOLD)
  {
     digitalWrite(RELAY_PIN, LOW);
    digitalWrite(RELAY_PIN2, HIGH); 
    Serial.println("LOOP 3");

  }

  else if (Fal_Val()==0)
  {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(RELAY_PIN2, HIGH);

    Serial.println("Flame Detected");
    sw.println("FLAME Detected From Aurdino");
    sw.print(Fal_Val());

  }



  //Serial.print("distance: ");
  //Serial.print(dist);
 // Serial.println(" cm");
  //Serial.println(val);

  delay(100);
}
