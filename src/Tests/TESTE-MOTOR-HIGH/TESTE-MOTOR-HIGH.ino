//Bibliotecas internas
#include "_config.h"

void setup(){
  Serial.begin(115200);

  pinMode(MOTOR_R, OUTPUT); //ESC1
  digitalWrite(MOTOR_R, HIGH);
  pinMode(MOTOR_L, OUTPUT); //ESC2
  digitalWrite(MOTOR_L, HIGH);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void loop() {

  
  
}
