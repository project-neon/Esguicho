#include <VL53L0X.h>  // Sensores de distância, Pololu 1.3.1
#include <Wire.h>     // Biblioteca I2C

VL53L0X sensorL;  // Sensor da esquerda
VL53L0X sensorC;  // Sensor da frente
VL53L0X sensorR;  // Sensor da direita

VL53L0X* sensorsList[3] = { &sensorL, &sensorC, &sensorR };  // Lista de ponteiros com os sensores

uint8_t sensorsAddresses[3] = { 0x1, 0x2, 0x3 };  // Endereços pro I2C

int distL;  // Valor lido pelo sensor da esquerda
int distC;  // Valor lido pelo sensor da frente
int distR;  // Valor lido pelo sensor da direita

int enemyLastTimeSeenLeft = false; // Define qual o último lado que vimos o inimigo

void DistanceSensorVL53L0XInit() {

  Wire.begin();  // Inicia o I2C
  Wire.setClock(400000); //TODO: iniciar o Wire em uma funcao separada no setup do codigo

	//Desligamos todos os sensores para pode endereçar os sensores
	//Pois os sensores podem vir de fábrica com um mesmo endereço
	//Então para não dar conflito, vamos de um em um
  for (int i = 0; i < 3; i++) {
    pinMode(VL53LOX_XSHUT_LIST_PINS[i], OUTPUT);
    digitalWrite(VL53LOX_XSHUT_LIST_PINS[i], LOW);
  }

  delay(10);

	//Para não dar conflito, os sensores sao ligados de um em um
	//Quando trocamos o valor pra INPUT, o próprio VL530X começa a alimentar
	//o XShut com o HIGH. Não é recomendado fazer HIGH com um digitalWrite(XSHUT_PIN, HIGH)
  for (int i = 0; i < 3; i++) {
    pinMode(VL53LOX_XSHUT_LIST_PINS[i], INPUT); // XSHUT volta pra HIGH
    sensorsList[i]->init(); // Inicia o sensor usando a função da lib
    sensorsList[i]->setAddress(sensorsAddresses[i]); // Endereça o sensor
    sensorsList[i]->setTimeout(100); // Tempo que pode ficar sem responder
    sensorsList[i]->startContinuous(); // Modo contínuo, ↓ Precisão ↑ Velocidade
    delay(10);
  }
}

void readDistanceSensorsValues() {
  // Armazena os valores lidos nos sensores
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
