//Fontes: http://www.esp32learning.com/code/esp32-and-infrared-receiver-example.php
//      : https://www.pjrc.com/teensy/td_libs_IRremote.html
#include <IRremote.hpp>
#include "controller_IR.h"

void setup() {
  Serial.begin(115200);
  controllerInit();
}

void loop() {
  controllerIR();
}
