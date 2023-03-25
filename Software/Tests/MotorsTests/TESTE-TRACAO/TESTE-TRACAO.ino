//Bibliotecas internas
#include "_config.h"
#include "motors.h"
#include "controller_IR.h"
#include "utils.h"

bool isLedOn = false;

void setup(){
  Serial.begin(115200);

  controllerInit();
  motorsInit();
}

void loop() {

  controllerIR();

  if((millis() - LedCounter > 300) and BlinksCounter > 0) {
    isLedOn ? digitalWrite(2, LOW) : digitalWrite(2, HIGH);
    isLedOn = isLedOn ? false : true;
    LedCounter = millis();
    BlinksCounter = BlinksCounter - 5;
  }

  if(go) {
    motorsOutput();
    printSpeed();
  }

}
