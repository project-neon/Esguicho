#include <ESP32Servo.h> //Comunicação com a ESC de cada motor v0.11.0

int speedMax = 50; //vel max
int searchSpeed = speedMax*0.6;//vel de girar no proprio eixo
int aproxSpeed = speedMax*0.25;//vel ao se aproximar do oponente
int timeApproaching = 0;

int flag = 0; //Direita --> 1, Esquerda --> -1, Valor inicial --> 0

//Cria os objetos para  cada ESC
Servo ESCL; //ESC que controla o motor da esquerda
Servo ESCR; //ESC que controla o motor da direita

//Variáveis que receberão o valor da velocidades que serão enviadas para os ESC's
int speedL = 0; //Valor de velocidade da esquerda
int speedR = 0; //Valor de velocidade da direita

int EscSpeedL = 0; //Valor de velocidade para o ESC da esquerda
int EscSpeedR = 0; //Valor de velocidade para o ESC da direita

int DistDif;

unsigned long contador = millis();
long intervalo = 100;