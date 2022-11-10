//Bibliotecas externas
#include <Wire.h>       //Auxiliar dos sensores
#include <IRremote.hpp>

//Bibliotecas internas
#include "_config.h"

const int I1 = 16;
const int I2 = 2064;
const int I3 = 1040;
const int I4 = 3088;
const int I5 = 528;
const int I6 = 2576;

String Rdirection = ""; //Variável que indicará o sentido determinado pelos sensores

int value = 0;
int stage = 0;

IRrecv irrecv(JCONTROLLER);
decode_results results;



void juiz() {
  if (irrecv.decode(&results)) {
    value = (results.value);
    Serial.print("value: ");
    Serial.println(value);
    switch(value)
    {
      case I1:
        if(stage == 0)
        {
          Serial.println("primeiro");  
          stage = 1;
        }
        break;
      case I2:
        if(stage == 1)
        {
          Serial.println("segundo");
          stage = 2;
        }
        break;
      case I3:
            Serial.println("MORREU");  
            stage = -10;
            //Rdirection = "MORTO";
            //Serial.println(stage);
        break;
    }
    irrecv.resume();
  }
}
