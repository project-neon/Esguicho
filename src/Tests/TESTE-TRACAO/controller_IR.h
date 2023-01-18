//Bibliotecas externas
#include <Wire.h>       //Auxiliar dos sensores
#include <IRremote.hpp> //InfraRed - Controle juiz v3.9.0
#include "motors.h"

// -- Controle Ana -- //
// const int I1 = 551520375;
// const int I2 = 551504055;
// const int I3 = 551536695;
// const int I4 = 551495895;
// const int I5 = 551528535;
// const int I6 = 551512215;

// -- Controle Competicao -- //
const int I1 = 16;
const int I2 = 2064;
const int I3 = 1040;
const int I4 = 3088;
const int I5 = 528;
const int I6 = 2576;
const int I7 = 5152;

int contador_led = 0;
int numero_piscadas = 0;
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
        stage = 20;
        Serial.println(stage);  
        contador_led = 0;
        numero_piscadas = stage/10;
        speedL = speedR = stage;
        break;

      case I2:
        stage = 40;
        Serial.println(stage);  
        contador_led = 0;
        numero_piscadas = stage/10;
        speedL = speedR = stage;
        break;

      case I3:
        stage = 60;
        Serial.println(stage);  
        contador_led = 0;
        numero_piscadas = stage/10;
        speedL = speedR = stage;
        break;
      
      case I4:
        stage = 80;
        Serial.println(stage);  
        contador_led = 0;
        numero_piscadas = stage/10;
        speedL = speedR = stage;
        break;

      case I5:
        stage = 100;
        Serial.println(stage);  
        contador_led = 0;
        numero_piscadas = stage/10;
        speedL = speedR = stage;
        break;
      
      case I6:
        Serial.println("GO!!");  
        digitalWrite(2, HIGH);
        motorsOutput();
        break;

      case I7:
        stage = 0;
        Serial.println("STOP!!");  
        digitalWrite(2, LOW);
        contador_led = 0;
        numero_piscadas = stage/10;
        speedL = speedR = stage;
        motorsOutput();
        break;
    }
    irrecv.resume();    
  }
}
