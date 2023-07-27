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
    if(distC < distAtk/3 and (distL < distAtk/3 or distR < distAtk/3)) {
      Serial.print("ATACANDO MAX \t\t");
      speedL = speedR = speedAtk;
    } else if(distC < distAtk and (distL < distAtk or distR < distAtk)) {
      Serial.print("ATACANDO \t\t");
      speedL = speedR = speedStandard;
    } else if (distL < distAtk or distR < distAtk) {
      (distL < distAtk) ? Serial.print("ESQ \t\t") : Serial.print("DIR \t\t");
      speedL = (distL < distAtk) ? speedStandard*0.9 : speedStandard;
      speedR = (distL < distAtk) ? speedStandard : speedStandard*0.9;
      flag = (distL < distAtk) ? -1 : 1;
    } else {
      (flag == -1) ? Serial.print("PROCURANDO ESQ \t\t") :  Serial.print("PROCURANDO DIR \t\t");
      speedL = (flag == -1) ? -1*searchSpeed : searchSpeed;
      speedR = (flag == -1) ? searchSpeed : -1*searchSpeed;
    }
//////////////////////////////Robo Parou//////////////////////////////
  } else {
    speedL = speedR = 0;
  }

  motorsOutput();
  printSpeed();
  printDistances();
}
