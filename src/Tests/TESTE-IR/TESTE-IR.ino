//Tirei as infos daqui: http://www.esp32learning.com/code/esp32-and-infrared-receiver-example.php
//E daqui: https://www.pjrc.com/teensy/td_libs_IRremote.html
#include <IRremote.hpp>
 
#define JCONTROLLER 4
 
IRrecv irrecv(JCONTROLLER);
decode_results results;
 
void setup() {
  pinMode(JCONTROLLER, INPUT);
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
}
 
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); //Verifica o valor lido pelo controle
    irrecv.resume();
  }
}
