//Bibliotecas externas
#include <Wire.h>       //Auxiliar dos sensores
#include <IRremote.hpp> //InfraRed - Controle juiz v3.9.0
#define JCONTROLLER 4

// -- Controle Competicao -- //
const int I1 = 16;
const int I2 = 2064;
const int I3 = 1040;
const int I4 = 3088;
const int I5 = 528;
const int I6 = 2576;

int valueIR = 0;

IRrecv irrecv(JCONTROLLER);
decode_results results;

void controllerInit() {
  //Iniciando o endereçamento dos sensores
  Wire.begin();
  irrecv.enableIRIn(); // Start the receiver
  pinMode(JCONTROLLER, INPUT);
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
}

void controllerIR() {
  if (irrecv.decode(&results)) {
    valueIR = results.value;
    Serial.print("valueIR: ");
    Serial.println(valueIR);
    switch(valueIR) {
      case I1:
        digitalWrite(2, LOW);
        delay(50);
        break;
    }
    irrecv.resume();
  } 
  digitalWrite(2, HIGH);
}

