#include <Arduino.h>
#include <ESP32Servo.h> //Comunicação com a ESC de cada motor v0.11.0

#define MOTOR 16

Servo ESC; //ESC que controla o motor da esquerda

void setup(){
  Serial.begin(115200);

  ESC.setPeriodHertz(50); 
  ESC.attach(MOTOR, 1000,2000);
  ESC.write(180); 
  delay(500);
  ESC.write(0); 
  delay(500);
  ESC.write(90); 
  delay(500);
}

void loop() {

  // ESC.write(130); 
  // delay(3000);
  // ESC.write(50); 
  
}
