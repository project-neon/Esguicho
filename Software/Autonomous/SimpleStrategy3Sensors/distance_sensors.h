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
    //Iniciando o endereçamento dos sensores
    Wire.begin();

    //Desligamos todos os sensores para pode endereçar os sensores
    //Pois os sensores podem vir de fábrica com um mesmo endereço
    //Então para não dar conflito, vamos de um em um
    pinMode(SDIST_L, OUTPUT);
    pinMode(SDIST_C, OUTPUT);
    pinMode(SDIST_R, OUTPUT);

    digitalWrite(SDIST_L, LOW);
    digitalWrite(SDIST_C, LOW);
    digitalWrite(SDIST_R, LOW);

    //Para não dar conflito, os sensores sao ligados de um em um
    //Quando trocamos o valor pra INPUT, o próprio VL530X começa a alimentar
    //o XShut com o HIGH. Não é recomendado fazer HIGH com um digitalWrite(SDIST_R, HIGH)
    pinMode(SDIST_L, INPUT);
    sensorL.init(true);
    sensorL.setAddress((uint8_t)0x21); //endereço do sensor da esquerda

    pinMode(SDIST_C, INPUT);
    sensorC.init(true);
    sensorC.setAddress((uint8_t)0x23); //endereço do sensor da frente

    pinMode(SDIST_R, INPUT);
    sensorR.init(true);
    sensorR.setAddress((uint8_t)0x25); //endereço do sensor da direita

    sensorL.setTimeout(500);
    sensorC.setTimeout(500);
    sensorR.setTimeout(500);
    sensorL.startContinuous();
    sensorC.startContinuous();
    sensorR.startContinuous();
}

void distanceRead() {
    //Armazena os valores lidos nas respectivas variáveis
    distL = sensorL.readRangeContinuousMillimeters();
    distC = sensorC.readRangeContinuousMillimeters();
    distR = sensorR.readRangeContinuousMillimeters();
    if(stage == 0){
        if((distL > 60000) or (distC > 60000) or (distR > 60000)) {
        digitalWrite(2, HIGH);
        } else{
        digitalWrite(2, LOW);
        }
    }

    if(distL > 700) distL = 700;
    if(distC > 700) distC = 700;
    if(distR > 700) distR = 700;
}
