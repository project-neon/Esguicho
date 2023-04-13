//Bibliotecas externas
#include <Wire.h>       //Auxiliar dos sensores
#include <IRremote.hpp> //InfraRed - Controle juiz v3.9.0

// -- Controle Ana -- //
const int I1 = 551520375;
const int I2 = 551504055;
const int I3 = 551536695;
const int I4 = 551495895;
const int I5 = 551528535;
const int I6 = 551512215;

// -- Controle Competicao -- //
// const int I1 = 16;
// const int I2 = 2064;
// const int I3 = 1040;
// const int I4 = 3088;
// const int I5 = 528;
// const int I6 = 2576;

int valueIR = 0;
int LedCounter = 0;
int BlinksCounter = 0;
int guardaVelocidade = 0;
bool go = false;

IRrecv irrecv(JCONTROLLER);
decode_results results;

void controllerInit() {
  //Iniciando o endereçamento dos sensores
  Wire.begin();
  irrecv.enableIRIn(); // Start the receiver
  pinMode(JCONTROLLER, INPUT);
  pinMode(2, OUTPUT);
}

void controllerIR() {
  if (irrecv.decode(&results)) {
    valueIR = (results.value);
    Serial.print("valueIR: ");
    Serial.println(valueIR);
    switch(valueIR)
    {
      case (I1 or I2 or I3  or I4  or I5  or I6):
        LedCounter = millis();
      case I1:
        Serial.println("20");
        speedL = speedR = BlinksCounter = 20;
        break;
      case I2:
        Serial.println("40");
        speedL = speedR = BlinksCounter = 40;
        break;
      case I3:
        Serial.println("60");
        speedL = speedR = BlinksCounter = 60;
        break;
      case I4:
        Serial.println("80");
        speedL = speedR = BlinksCounter = 80;
        break;
      case I5:
        Serial.println("100");
        speedL = speedR = BlinksCounter = 100;
        break;
      case I6:
        Serial.println("Inicia/para");
        if (go) {
          guardaVelocidade = speedL;
          speedL = speedR = 0;
          go = false;
          motorsOutput();
          Serial.println("PAROU!");
          speedL = speedR = guardaVelocidade;
        } else {
          go = true;
        }
        break;
      default:
       break;
    }
    irrecv.resume();
  }
}
