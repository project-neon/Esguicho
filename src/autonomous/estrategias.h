//Bibliotecas externas
#include <ESP32Servo.h> //Comunicação com a ESC de cada motor
#include <VL53L0X.h>    //sensores de distância
#include <Wire.h>       //Auxiliar dos sensores

//Bibliotecas internas
#include "_config.h"

//Cria os objetos para  cada ESC
Servo ESCL; //ESC que controla o motor da esquerda
Servo ESCR; //ESC que controla o motor da direita

int refSpeed = 100;
//Cria os objetos para cada sensor
VL53L0X sensorL;  //Sensor da esquerda
VL53L0X sensorC;  //Sensor da frente
VL53L0X sensorR;  //Sensor da direita

//Variáveis que vão receber os valores lidos dos pinos dos sensores
int distL;  //Valor lido pelo sensor da esquerda
int distC;  //Valor lido pelo sensor da frente
int distR;  //Valor lido pelo sensor da direita

int distMax = 200;  //Distância máxima permitida como referência para os sensores (em milímetros)

//Variáveis que receberão o valor da velocidades que serão enviadas para os ESC's
int speedL; //Valor de velocidade para o ESC da esquerda
int speedR; //Valor de velocidade para o ESC da direita

String direction = ""; //Variável que indicará o sentido determinado pelos sensores

int currentTime = 0;
int startTime = 0;

int strategy = -1; //Por enquanto, eu estou escrevendo no código, depois vai ser decidido de acordo com o controle do 
//0 = simples
//1 = meialua
//2 = meialua em s

/*
 * No momento em que a placa é alimentada, é necessário que o valor enviado aos ESC's seja o valor 0 (zero)
 * para que os ESC's não entrem em modo de configuração. 
 * Para isso, iniciamos o valor da flag em (-1), pois é um valor que só acontecerá no momento de inicialização.
 * Assim garantimos que no primeiro momento, o valor enviado aos ESC's será 0 (zero).
 */
int flag = -1; //Direita --> 0, Esquerda --> 1, Valor inicial --> -1

void simpleStrategy() {
  if(flag == -1 && distC > 5000 && distL > 5000 && distR > 5000 ){
    speedL = 0;
    speedR = 0;
    direction = "COND. INICIAL";
  }
  else if (distR < distMax && distC < distMax && distL >= distMax){
    speedL = refSpeed;
    speedR = 30;
    direction = "FRENTE + DIR";
    flag = 0;
  } 
  else if(distR >= distMax && distC < distMax && distL < distMax){
    speedL = 30;
    speedR = refSpeed;
    direction = "FRENTE + ESQ";
    flag = 1;
  } 
  else if(distC >= distMax && distL >= distMax && (distR < distMax || flag == 0)){
    speedL = refSpeed;
    speedR = -1*refSpeed;
    direction = "DIREITA";
    flag = 0;
  } else if(distR >= distMax && distC >= distMax && (distL < distMax || flag == 1)){
    speedL = -1*refSpeed;
    speedR = refSpeed;
    direction = "ESQUERDA";
    flag = 1;
  } 
    else if(distC < distMax){
    speedL = refSpeed;
    speedR = refSpeed;
    direction = "FRENTE";
  }   
  else{
    speedL = 0;
    speedR = 0;
    direction = "IMPOSSIVEL"; // Sensores da esquerda e direita detectando algo simultaneamente e o sensor da frente não
  }
}

//Condições dessa estratégia:
//O robô já começa virado e com uma certa distância da borda (a decidir pelos testes #TODO)
//O robô n pensa, só acelera em meia lua!
void meiaLua() {
  direction = "meia lua";
  //TODO: testar o tempo que o robô demora pra percorrer metade da arena e qual a melhor vel
  currentTime = millis();

  if (startTime == 0) startTime = currentTime;
  else if (currentTime - startTime >= 1000) {
    strategy = 0;
    flag = 1; //Considera que o inimigo deve estar na esquerda pois atacou pela direita
  }
  speedL = 0.25*refSpeed;
  speedR = refSpeed; 
 
}

//Condições dessa estratégia:
//O robô comseça virado pro inimigo e dá uma voltinha pra poder fazer a meia lua
//O robô n pensa, só acelera em meia lua!
void meiaLuaEmS() {
  direction = "meia lua em S";
  //TODO: testar o tempo que o robô demora pra percorrer metade da arena e qual a melhor vel
  currentTime = millis();

  if (startTime == 0) startTime = currentTime;
  if (currentTime - startTime <= 1000* 0.25) {
    speedL = refSpeed;
    speedR = 0.25*refSpeed;
  } else if (currentTime - startTime <= 1000* 0.75) {
    speedL = 0.25*refSpeed;
    speedR = refSpeed;
  }
  else {
    strategy = 0;
    flag = 1; //Considera que o inimigo deve estar na esquerda pois atacou pela direita
  }
}
  //TODO: testar o tempo que o robô demora pra percorrer metade da arena e qual a melhor vel

void selecionarEstrategia(){
  switch(strategy) {
    case 0:
      simpleStrategy();
      break;
    case 1:
      meiaLua();
      break;
    case 2:
      meiaLuaEmS();
      break;
    default:
      simpleStrategy();
      break;
  }
}
