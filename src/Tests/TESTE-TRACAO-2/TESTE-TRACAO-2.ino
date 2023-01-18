//Bibliotecas internas
#include "_config.h"
#include "motors.h"
#include "controller_IR.h"
#include "utils.h"

bool ledLigado = false;

void setup(){
  Serial.begin(115200);

  controllerInit();
  motorsInit();
}

void loop() {

  controllerIR();

  if((millis() - contador > 300) and numeroPiscadas > 0) {
    ledLigado ? digitalWrite(2, LOW) : digitalWrite(2, HIGH);
    ledLigado = ledLigado ? false : true;
    contador = millis();
    numeroPiscadas = numeroPiscadas - 5;
  }

  if(go) {
    motorsOutput();
    printSpeed();
  }
  
}
