#include <VL53L0X.h>    //sensores de distância v1.3.1

//Cria os objetos para cada sensor
VL53L0X sensorL;  //Sensor da esquerda
VL53L0X sensorC;  //Sensor da frente
VL53L0X sensorR;  //Sensor da direita

//Variáveis que vão receber os valores lidos dos pinos dos sensores
int distL;  //Valor lido pelo sensor da esquerda
int distC;  //Valor lido pelo sensor da frente
int distR;  //Valor lido pelo sensor da direita

void sensorsInit() {
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

void distanceRead() {
    //Armazena os valores lidos nas respectivas variáveis 
    distL = sensorL.readRangeSingleMillimeters();
    distC = sensorC.readRangeSingleMillimeters();
    distR = sensorR.readRangeSingleMillimeters(); 
    if(stage == 0){
        if((distL > 60000) or (distC > 60000) or (distR > 60000)) {
        digitalWrite(2, HIGH);
        }
        else{
        digitalWrite(2, LOW);
        }
    }
    
    if(distL > 600) distL = 600;
    if(distC > 600) distC = 600;
    if(distR > 600) distR = 600;
}