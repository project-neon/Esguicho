//Bibliotecas internas
#include "_config.h"
#include "constants_declaration.h"
#include "controller_IR.h"
#include "motors.h"
#include "distance_sensors.h"
#include "utils.h"

int flag = 0; //Direita --> 1, Esquerda --> -1, Valor inicial --> 0
unsigned long contador = millis();

void setup(){
  Serial.begin(115200);

  controllerInit();
  sensorsInit();
  motorsInit();
}

void loop() {

  controllerIR();
  distanceRead();

  if(stage == 1) {
    if((millis() - contador) < 300) digitalWrite(2, HIGH); // Acende o led do pino 2
    else digitalWrite(2, LOW); // Apaga o led do pino 2
    if((millis() - contador) > 600) contador = millis(); // Verifica se já passou 600 milisegundos
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  else if(stage == 2) {
    //inicio das decisões
    if(distL < distAtkMax and distR < distAtkMax){
      Serial.print("ATACANDO MÁX \t\t");
      speedL = speedR = speedATK;
    }
    else if(distC < 100 or (distL < distAtk and distR < distAtk)){
      Serial.print("ATACANDO \t\t");
      speedL = speedR = speedMax;
    }
    else if (distL < distAtk or distR < distAtk){
      (distL < distAtk) ? Serial.print("ESQ \t\t") : Serial.print("DIR \t\t");
      speedL = (distL < distAtk) ? speedMax*0.3 : speedMax;
      speedR = (distL < distAtk) ? speedMax : speedMax*0.3;
      flag = (distL < distAtk) ? -1 : 1;
    }
    else{
      (flag == -1) ? Serial.print("PROCURANDO ESQ \t\t") :  Serial.print("PROCURANDO DIR \t\t");;
      speedL = (flag == -1) ? -1*searchSpeed : searchSpeed;
      speedR = (flag == -1) ? searchSpeed : -1*searchSpeed;
    }

    motorsOutput();
    
  }
  //fim das decisões
  /////////////////////////////////////////////////////////////////////////////////////////////
  else {
    speedL = speedR = 0;
    motorsOutput();
  }


  printSpeed();
  printDistances();
  
}
