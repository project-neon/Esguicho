//Bibliotecas internas
#include "_config.h"
#include "controller_IR.h"
#include "utils.h"

unsigned long contador = millis();

void setup(){
  Serial.begin(115200);
  controllerInit();
  motorsInit();
}

void loop() {

  controllerIR();

  while(contador_led < numero_piscadas){
    if((millis() - contador) < 300) digitalWrite(2, HIGH); // Acende o led do pino 2
    else digitalWrite(2, LOW); // Apaga o led do pino 2
    if((millis() - contador) > 600){
      contador = millis(); // Verifica se já passou 600 milisegundos
      contador_led++;
    }

  }

  printSpeed();
  
}