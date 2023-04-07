//Bibliotecas internas
#include "config.h"
#include "constants_declaration.h"
#include "controller_IR.h"
#include "motors.h"
#include "distance_sensors.h"
#include "utils.h"

int flag = 0; //Direita --> 1, Esquerda --> -1, Valor inicial --> 0
unsigned long LedCounter = millis();

void setup(){
  Serial.begin(115200);

  controllerInit();
  sensorsInit();
  motorsInit();
}

void loop() {

  controllerIR();

  if(stage == 1) {

    if((millis() - LedCounter) >= 300) {
      digitalWrite(2, !digitalRead(2)); //Pisca o Led
      LedCounter = millis();
    }
//////////////////////////////Inicio das decisões//////////////////////////////
  } else if(stage == 2) {

    distanceRead();
    printDistances();

    if(distC < 600){
      speedL = speedR = 85;
    } else{
      speedL = 21;
      speedR = -21;
    }
//////////////////////////////Fim das decisões//////////////////////////////
  } else {
    speedL = speedR = 0;
  }

  motorsOutput();
  printSpeed();
}
