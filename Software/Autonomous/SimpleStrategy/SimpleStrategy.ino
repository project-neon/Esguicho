//Bibliotecas internas
#include "config.h"
#include "constants_declaration.h"
#include "controller_IR.h"
#include "motors.h"
#include "distance_sensors.h"
#include "utils.h"

int flag = 0; //Direita --> 1, Esquerda --> -1, Valor inicial --> 0

void setup(){
  Serial.begin(115200);

  controllerInit();
  sensorsInit();
  motorsInit();
}

void loop() {

  controllerIR();

//////////////////////////////Led Ligado//////////////////////////////
  if(stage == 1) {
    digitalWrite(2, HIGH);
//////////////////////////////Estrategia//////////////////////////////
  } else if(stage == 2) {

    distanceRead();
    printDistances();

    if(distC < 600){
      speedL = speedR = 85;
    } else{
      speedL = 21;
      speedR = -21;
    }
//////////////////////////////Robo Parou//////////////////////////////
  } else {
    speedL = speedR = 0;
  }

  motorsOutput();
  printSpeed();
}
