/* 
 * Código para testar uma estratégia simples, sem considerar o controle do juiz
 */

//Bibliotecas externas
#include <ESP32Servo.h> //Comunicação com a ESC de cada motor
#include <VL53L0X.h>    //sensores de distância
#include <Wire.h>       //Auxiliar dos sensores

//Bibliotecas internas
#include "_config.h"

//Cria os objetos para  cada ESC
Servo ESCL; //ESC que controla o motor da esquerda
Servo ESCR; //ESC que controla o motor da direita

//Cria os objetos para cada sensor
VL53L0X sensorL;  //Sensor da esquerda
VL53L0X sensorC;  //Sensor da frente
VL53L0X sensorR;  //Sensor da direita

int distL = 500;
int distC = 500;
int distR = 500;
int distMax = 500;

int speedL = 0;
int speedR = 0;

String direction = "COND. INICIAL"; //Variável auxiliar que indicará o sentido determinado pelos sensores
int flag = -1; //Direita --> 0, Esquerda --> 1, Valor inicial --> -1


void setup(){
  
  Serial.begin(115200);
  
  //-----------------SENSORES---------------//
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
  
  //-----------------ESC---------------//
  //Configurando o sinal PWM que será enviado aos ESC's
  ESCL.setPeriodHertz(50);             // Estabelece a frequência do PWM (50Hz)
  ESCL.attach(MOTOR_L, 1000,2000);     // (Pino onde será enviado o sinal, largura de pulso mínima, largura de pulso máxima)  
}

void simpleStrategy() {
  if (distR < distMax && distC < distMax && distL >= distMax){
    speedL = 100;
    speedR = 30;
    direction = "FRENTE + DIR";
    flag = 0;
  } 
  else if(distR >= distMax && distC < distMax && distL < distMax){
    speedL = 30;
    speedR = 100;
    direction = "FRENTE + ESQ";
    flag = 1;
  } 
  else if(distC >= distMax && distL >= distMax && (distR < distMax || flag == 0)){
    speedL = 100;
    speedR = 0;
    direction = "DIREITA";
    flag = 0;
  } else if(distR >= distMax && distC >= distMax && (distL < distMax || flag == 1)){
    speedL = 0;
    speedR = 100;
    direction = "ESQUERDA";
    flag = 1;
  } 
  else if(distC < distMax){
    speedL = 100;
    speedR = 100;
    direction = "FRENTE";
  }   
  else{
    speedL = 0;
    speedR = 0;
    direction = "IMPOSSIVEL"; // Sensores da esquerda e direita detectando algo simultaneamente e o sensor da frente não
  }
}

 
void loop() {
  //Armazena os valores lidos nas respectivas variáveis
  distL = sensorL.readRangeSingleMillimeters();
  distC = sensorC.readRangeSingleMillimeters();
  distR = sensorR.readRangeSingleMillimeters();  
  
  simpleStrategy();
 
  // Mostra o valor que será enviado às ESCS
  Serial.print(speedL);
  Serial.print("\t");
  Serial.print(speedR); 
  Serial.print("\t\t");
  
  // Mostra o valor de cada sensor na tela e a decisão escolhida
  Serial.print("R: ");
  Serial.print(distR);
  Serial.print("\t");
  Serial.print("C: ");
  Serial.print(distC);
  Serial.print("\t");
  Serial.print("L: ");
  Serial.print(distL);
  Serial.print("\t\t");
  Serial.println(direction);

  // Converte os valores de 0 a 100 para os valores lidos pelos ESC's que vão de 0 a 180
  speedL = map(speedL, 0, 100, 0, 180);
  speedR = map(speedR, 0, 100, 0, 180);
  
  // Envia o valor para as ESCs
  ESCL.write(speedL); 
  ESCR.write(speedR); 
}