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
int stage = 0;

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
        break;
    }
    irrecv.resume();
  }
}
