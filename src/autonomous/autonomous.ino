/* 
 * Código para testar o acionamento de 2 motores
 * utilizando 3 sensores VL53L0X como acionadores
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

String direction = ""; //Variável que indicará o sentido determinado pelos sensores
int strategy = 0; //Por enquanto, eu estou escrevendo no código, depois vai ser decidido de acordo com o controle do 
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
  
  //Configurando o sinal PWM que será enviado aos ESC's
  ESCL.setPeriodHertz(50);             // Estabelece a frequência do PWM (50Hz)
  ESCL.attach(MOTOR_L, 1000,2000);     // (Pino onde será enviado o sinal, largura de pulso mínima, largura de pulso máxima)
  ESCR.setPeriodHertz(50);              
  ESCR.attach(MOTOR_R, 1000,2000);     
  /*                                      
   * É necessário estabelecer uma faixa de largura de pulso para que o ESC
   * responda corretamente a variação do sinal analógico lido do potenciômetro
   * O valor da largura de pulso deve ser informado na escala de micro-segundos
   * No caso, a largura de pulso do PWM mínima é de 1000us e a máxima é de 2000us                                      
   */
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

//Condições dessa estratégia:
//O robô já começa virado e com uma certa distância da borda (a decidir pelos testes #TODO)
//O robô n pensa, só acelera em meia lua!
void meiaLua() {
  //TODO: testar o tempo que o robô demora pra percorrer metade da arena e qual a melhor vel
  for (int timer = 0; timer < 1000 * 1; timer++) {
    speedL = map(25, 0, 100, 0, 180);
    speedR = map(100, 0, 100, 0, 180);
    ESCL.write(speedL); 
    ESCR.write(speedR); 
  }
  flag = 1; //Considera que o inimigo deve estar na esquerda pois atacou pela direita
  strategy = 0; //Retorna pra estratégia padrão caso o ataque inicial não tenha dado certo
}

//Condições dessa estratégia:
//O robô comseça virado pro inimigo e dá uma voltinha pra poder fazer a meia lua
//O robô n pensa, só acelera em meia lua!
void meiaLuaEmS() {
  //TODO: testar o tempo que o robô demora pra percorrer metade da arena e qual a melhor vel
  for (int timer = 0; timer < 1000 * 0.25; timer++) {
    speedL = map(100, 0, 100, 0, 180);
    speedR = map(25, 0, 100, 0, 180);
    ESCL.write(speedL); 
    ESCR.write(speedR); 
  }
  for (int timer = 0; timer < 1000 * 0.75; timer++) {
    speedL = map(25, 0, 100, 0, 180);
    speedR = map(100, 0, 100, 0, 180);
    ESCL.write(speedL); 
    ESCR.write(speedR); 
  }
  flag = 1; //Considera que o inimigo deve estar na esquerda pois atacou pela direita
  strategy = 0; //Retorna pra estratégia padrão caso o ataque inicial não tenha dado certo
}
 
void loop() {
  //Armazena os valores lidos nas respectivas variáveis
  distL = sensorL.readRangeSingleMillimeters();
  distC = sensorC.readRangeSingleMillimeters();
  distR = sensorR.readRangeSingleMillimeters();  

  //Tomadas de decisão
  //Os valores aqui utilizados servem apenas para um teste de tomada de decisão
  //O número "200" é um número aleatório apenas para o teste

  //TODO: Alterar pra começar verificando as distC/L/R
  if(flag == -1 && distC > 5000 && distL > 5000 && distR > 5000 ){
    speedL = 0;
    speedR = 0;
    direction = "COND. INICIAL";
  }
  
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
