//Bibliotecas internas
#include "_config.h"
#include "motors.h"
#include "controller_IR.h"

unsigned long contador = millis();

void setup(){
  Serial.begin(115200);
  controllerInit();
  motorsInit();
}

void loop() {
  controllerIR();

  if(stage == 1) {
    if((millis() - contador) < 300) digitalWrite(2, HIGH); // Acende o led do pino 2
    else digitalWrite(2, LOW); // Apaga o led do pino 2
    if((millis() - contador) > 600) contador = millis(); // Verifica se já passou 600 milisegundos
  }
  else if(stage == 2) {
    speedL = speedR = 50;
    motorsOutput();
  } else {
    speedL = speedR = 0;
    motorsOutput();
  }
  Serial.print("speedL: ");
  Serial.print(speedL);
  Serial.print("\t");
  Serial.print("speedR: ");
  Serial.print(speedR); 
  Serial.print("\t\t");
}
