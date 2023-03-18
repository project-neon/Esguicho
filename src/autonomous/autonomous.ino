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

bool needsToStop(bool velEsqPositivaNova, bool velDirPositivaNova ) {
  if (speedL == 0 and speedR == 0) return false; //Se a vel é 0, pode ir pra frente ou trás sem parar

  speedLPositiva = speedL > 5 ? true : false; //Adicionei uma margem para ficar de acordo com a de 84 ate 94 que vai pra esc
  speedRPositiva = speedR > 5 ? true : false;
  if ((speedLPositiva == velEsqPositivaNova) and (speedRPositiva == velDirPositivaNova)) {
    if (contadorFlutuacoes > 0) contadorFlutuacoes--; //Pode ser que a gnt soh teve variacoes temporarias, entao a gnt vai tirando elas
    return false; //Se o sentido q vai rodar é igual ao anterior, nao para!
  }

  contadorFlutuacoes++;
  if (contadorFlutuacoes >= 10) {
    speedL = speedR = 0; // APenas para caso a gnt tenha tido flutuacoes seguidas o suficiente. 10 eh o bastante?
    motorsOutput();
    delay(100); //TODO: Podemos aumentar esse valor p/ ver no codigo mais claramente se ele para antes de mudar a direcao
    contadorFlutuacoes = 0;
    return false; //Retorna falso para o codigo nao fazer um return dentro do loop!
  }
  return true;

}

void loop() {

  controllerIR();
  distanceRead();

//////////////////////////////Pisca o Led//////////////////////////////
  if(stage == 1) {
    if((millis() - contador) >= 300) {
      digitalWrite(2, !digitalRead(2));
      contador = millis();
    }
//////////////////////////////Inicio das decisões//////////////////////////////
  } else if(stage == 2) {

    if(distL < distAtkMax and distR < distAtkMax) {
      if (needsToStop(true,true)) return;
      Serial.print("ATACANDO MÁX \t\t");
      speedL = speedR = speedMax;
    } else if(distC < 100 or (distL < distAtk and distR < distAtk)) {
      if (needsToStop(true,true)) return;
      Serial.print("ATACANDO \t\t");
      speedL = speedR = speedStandard;
    } else if (distL < distAtk or distR < distAtk) {
      if (needsToStop(true,true)) return;
      (distL < distAtk) ? Serial.print("ESQ \t\t") : Serial.print("DIR \t\t");
      speedL = (distL < distAtk) ? speedStandard*0.3 : speedStandard;
      speedR = (distL < distAtk) ? speedStandard : speedStandard*0.3;
      flag = (distL < distAtk) ? -1 : 1;
    } else {
      if (needsToStop(flag == -1, flag != -1)) return;
      (flag == -1) ? Serial.print("PROCURANDO ESQ \t\t") :  Serial.print("PROCURANDO DIR \t\t");;
      speedL = (flag == -1) ? -1*searchSpeed : searchSpeed;
      speedR = (flag == -1) ? searchSpeed : -1*searchSpeed;
    }

    motorsOutput();
//////////////////////////////Fim das decisões//////////////////////////////
  } else {
    speedL = speedR = 0;
    motorsOutput();
  }


  printSpeed();
  printDistances();

}
