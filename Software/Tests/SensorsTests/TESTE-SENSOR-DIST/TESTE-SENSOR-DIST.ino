#include <VL53L0X.h>    //sensores de distância
#include <Wire.h>       //Auxiliar dos sensores

//Bibliotecas internas
#include "_config.h"

VL53L0X sensorL;  //Sensor da esquerda
VL53L0X sensorC;  //Sensor da frente
VL53L0X sensorR;  //Sensor da direita

//Variáveis que vão receber os valores lidos dos pinos dos sensores
int distL;  //Valor lido pelo sensor da esquerda
int distC;  //Valor lido pelo sensor da frente
int distR;  //Valor lido pelo sensor da direita

void setup(){
  
  Serial.begin(115200);
  
  //Iniciando o endereçamento dos sensores
  Wire.begin();
  
  pinMode(SDIST_L, OUTPUT);
  pinMode(SDIST_C, OUTPUT);
  pinMode(SDIST_R, OUTPUT);

  digitalWrite(SDIST_L, LOW);
  digitalWrite(SDIST_C, LOW);
  digitalWrite(SDIST_R, LOW);
  
  pinMode(SDIST_L, INPUT);
  sensorL.init(true);
  sensorL.setAddress((uint8_t)0x21); //endereço do sensor da esquerda

  pinMode(SDIST_C, INPUT);
  sensorC.init(true);
  sensorC.setAddress((uint8_t)0x23); //endereço do sensor da frente

  pinMode(SDIST_R, INPUT);
  sensorR.init(true);
  sensorR.setAddress((uint8_t)0x25); //endereço do sensor da direita

  sensorL.setTimeout(100);
  sensorC.setTimeout(100);
  sensorR.setTimeout(100);
}

void loop() {
  //Armazena os valores lidos nas respectivas variáveis
  distL = sensorL.readRangeSingleMillimeters();
  distC = sensorC.readRangeSingleMillimeters();
  distR = sensorR.readRangeSingleMillimeters();  
  
  // Mostra o valor de cada sensor na tela e a decisão escolhida
  Serial.print("L: ");
  Serial.print(distL);
  Serial.print("\t");
  Serial.print("C: ");
  Serial.print(distC);
  Serial.print("\t");
  Serial.print("R: ");
  Serial.print(distR);
  Serial.println("\t\t");

}
