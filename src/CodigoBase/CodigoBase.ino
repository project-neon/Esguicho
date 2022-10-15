//Bibliotecas internas
#include "estrategias.h"
#include <ESP32Servo.h> //Comunicação com a ESC de cada motor
#include <VL53L0X.h>    //sensores de distância

int distMax = 600;  //Distância máxima permitida como referência para os sensores (em milímetros)
int distAtk = 150;

int speedMax = 100; //vel max
int searchSpeed = 25; //vel de girar no proprio eixo
int aproxSpeed = 25;//vel ao se aproximar do oponente

int flag = -1; //Direita --> 1, Esquerda --> -1, Valor inicial --> 0

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

void setup(){
  
  Serial.begin(115200);
  
  //Iniciando o endereçamento dos sensores
  Wire.begin();
  irrecv.enableIRIn(); // Start the receiver
  pinMode(JCONTROLLER, INPUT);
  pinMode(2, OUTPUT);
   
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

void loop() {
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
  if(distL > 600){
      distL = 600;
    }
    if(distC > 600){
      distC = 600;
    }
    if(distR > 600){
      distR = 600;
    }
    
  juiz();

  if(stage == 1){
    if((millis() - contador) < 300){
    // Acende o led do pino 2
    digitalWrite(2, HIGH);
    }
    else{
      // Apaga o led do pino 2
      digitalWrite(2, LOW);
    }
    // Verifica se já passou 600 milisegundos
      if((millis() - contador) > 600){
        contador = millis();
  }
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  if(stage == 2){
    if(distL <= distR){
      //flag para a esquerda
      flag = -1;
    }
    else{
      //flag para a direita
      flag = 1;
    }

    //inicio das decisões
    if(distC < distAtk){
      Serial.print("ATACANDO ");
      speedL = speedMax;
      speedR = speedMax;
    }
    
    else if(distC < distMax){
      Serial.print("APROXIMANDO ");
      //se o oponente estiver para a esquerda a diferença será negativa, para a direita será positiva
      DistDif = distL - distR;
      
      if(DistDif < 0){
        //oponente na esquerda
        speedL = aproxSpeed-(aproxSpeed*abs(DistDif)/600);
        speedR = aproxSpeed;
      }
      else{
        //oponente na direita
        speedL = aproxSpeed;
        speedR = aproxSpeed-(aproxSpeed*abs(DistDif)/600);
      }
    }
    
    else{
      Serial.println("PROCURANDO");
      /*
      if (flag == -1){
        Serial.print("PROCURANDO ");
        //oponente a esquerda
        speedL = -1*searchSpeed;
        speedR = searchSpeed;
      }
      else{
        //oponente a direita
        speedL = searchSpeed;
        speedR = -1*searchSpeed;
      }
      */
      speedL = flag * searchSpeed;
      speedR = flag * searchSpeed * (-1);
    }
  }
  //fim das decisões
  /////////////////////////////////////////////////////////////////////////////////////////////
  
  else{
    speedL = 0;
    speedR = 0;
  }
  
  // Mostra o valor que será enviado ao map
  Serial.print("speedL: ");
  Serial.print(speedL);
  Serial.print("\t");
  Serial.print("speedR: ");
  Serial.print(speedR); 
  Serial.print("\t\t");
  
  speedL = map(speedL, -100, 100, 0, 180);
  speedR = map(speedR, -100, 100, 0, 180);
  ESCL.write(speedL); 
  ESCR.write(speedR);
  
 

  
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
