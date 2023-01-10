#include <ESP32Servo.h> //Comunicação com a ESC de cada motor v0.11.0
#include <VL53L0X.h>    //sensores de distância v1.3.1

int distMax = 600;  //Distância máxima permitida como referência para os sensores (em milímetros)
int distAtk = 400;

int speedMax = 50; //vel max
int searchSpeed = speedMax*0.6;//vel de girar no proprio eixo
int aproxSpeed = speedMax*0.25;//vel ao se aproximar do oponente
int timeApproaching = 0;

int flag = 0; //Direita --> 1, Esquerda --> -1, Valor inicial --> 0

//Cria os objetos para  cada ESC
Servo ESCL; //ESC que controla o motor da esquerda
Servo ESCR; //ESC que controla o motor da direita

//Cria os objetos para cada sensor
VL53L0X sensorL;  //Sensor da esquerda
VL53L0X sensorC;  //Sensor da frente
VL53L0X sensorR;  //Sensor da direita


//Variáveis que receberão o valor da velocidades que serão enviadas para os ESC's
int speedL = 0; //Valor de velocidade para o ESC da esquerda
int speedR = 0; //Valor de velocidade para o ESC da direita

//Variáveis que vão receber os valores lidos dos pinos dos sensores
int distL;  //Valor lido pelo sensor da esquerda
int distC;  //Valor lido pelo sensor da frente
int distR;  //Valor lido pelo sensor da direita

int DistDif;

unsigned long contador = millis();
long intervalo = 100;