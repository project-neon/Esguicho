//Bibliotecas externas
#include <Wire.h>       //Auxiliar dos sensores
#include <IRremote.hpp> //InfraRed - Controle juiz v3.9.0

// -- Controle Ana -- //
// const int I1 = 551520375;
// const int I2 = 551504055;
// const int I3 = 551536695;
// const int I4 = 551495895;
// const int I5 = 551528535;
// const int I6 = 551512215;
// const int I7 = 551544855;
// const int I8 = 551491815;
// const int I9 = 551524455;
// const int I0 = 551487735;

// -- Controle Matt -- //
// const int I1 = 1086290565;
// const int I2 = 1086306885;
// const int I3 = 1086290055;
// const int I4 = 1086280365;
// const int I5 = 1086296685;
// const int I6 = 1086279855;
// const int I7 = 1086282405;
// const int I8 = 1086298725;
// const int I9 = 1086281895;
// const int I0 = 1086304845;


// -- Controle Competicao -- //
const int I1 = 16;
const int I2 = 2064;
const int I3 = 1040;
const int I4 = 3088;
const int I5 = 528;
const int I6 = 2576;
const int I7 = 1552;
const int I8 = 3600;
const int I9 = 272;
const int I0 = 2320;


int valueIR = 0;
int stage = 0;

void controllerInit() {
  //Iniciando o endereçamento dos sensores
  Wire.begin();
  // IrReceiver.begin(JCONTROLLER, ENABLE_LED_FEEDBACK); // Start the receiver
  IrReceiver.begin(JCONTROLLER); // Start the receiver
  pinMode(JCONTROLLER, INPUT);
  pinMode(2, OUTPUT);
}

void controllerIR() {
  if (IrReceiver.decode()) {
    valueIR = IrReceiver.decodedIRData.command;
    // Serial.println("IrReceiver.decodedIRData.decodedRawData");
    // Serial.println(IrReceiver.decodedIRData.decodedRawData);
    // Serial.println("IrReceiver.decodedIRData.command");
    // Serial.println(IrReceiver.decodedIRData.command);
    if (IrReceiver.decodedIRData.decodedRawData > 0 && IrReceiver.decodedIRData.decodedRawData < 1000) {
      switch(valueIR)
    {
      case 0:
        if(stage == 0) {
          Serial.println("primeiro");
          stage = 1;
        }
        digitalWrite(2, LOW);
        delay(50);
        break;
      case 1:
        if(stage == 1) {
          Serial.println("segundo");
          stage = 2;
          digitalWrite(2, LOW);
        }
        break;
      case 2:
        Serial.println("MORREU");
        stage = -10;
        break;
      case 3:
        Serial.println("LOAD");
        stage = 0;
        digitalWrite(2, HIGH);
        break;
    }
    }
    IrReceiver.resume();
  }
}
