//Tirei as infos daqui: http://www.esp32learning.com/code/esp32-and-infrared-receiver-example.php
//E daqui: https://www.pjrc.com/teensy/td_libs_IRremote.html
#include <IRremote.h>
 
#define JCONTROLLER 15
 
IRrecv irrecv(JCONTROLLER);
decode_results results;
 
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}
 
void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); //Verifica o valor lido pelo controle

    //Parece que também dá pra testar pra ver o tipo de controle!
    if (results.decode_type == NEC) {
      Serial.print("NEC: ");
    } else if (results.decode_type == SONY) {
      Serial.print("SONY: ");
    } else if (results.decode_type == RC5) {
      Serial.print("RC5: ");
    } else if (results.decode_type == RC6) {
      Serial.print("RC6: ");
    } else if (results.decode_type == UNKNOWN) {
      Serial.print("UNKNOWN: ");
    }

    irrecv.resume();
  }
}