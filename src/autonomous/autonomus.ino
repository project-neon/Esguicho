/* 
 * Código para testar o acionamento de 2 motores
 * utilizando 3 sensores VL53L0X como acionadores
 */

//Inclui as bibliotecas necessárias
#include <ESP32Servo.h> //Biblioteca utilizada para comunicar com a ESC de cada motor
#include <Wire.h>       //Biblieoteca auxiliar para os sensores
#include <VL53L0X.h>    //Biblioteca para utilizar os sensores

//Cria os objetos para  cada ESC
Servo ESCL; //ESC que controla o motor da esquerda
Servo ESCR; //ESC que controla o motor da direita

//Cria os objetos para cada sensor
VL53L0X sensorL;  //Sensor da esquerda
VL53L0X sensorC;  //Sensor da frente
VL53L0X sensorR;  //Sensor da direita

//Pinos GPIO usados para conectar o ESP32 a cada ESC
int ESCLPin = 16; //Pino para controlar o ESC da esquerda
int ESCRPin = 17; //Pino para controlar o ESC da direita

//Pinos dos sensores de borda
int SLeftPin = 34;
int SRightPin = 39;

//Pinos GPIO usados para conectar o ESP32 a cada pino XSHUT dos sensores
int SBorderLeftPin = 23;    //XSHUT do sensor da esquerda
int SCenterPin = 19;  //XSHUT do sensor da frente
int SRightPin = 18;   //XSHUT do sensor da direita

//Variáveis que vão receber os valores lidos dos pinos dos sensores
int distL;  //Valor lido pelo sensor da esquerda
int distC;  //Valor lido pelo sensor da frente
int distR;  //Valor lido pelo sensor da direita

int distMax = 200;  //Distância máxima permitida como referência para os sensores (em milímetros)

//Variáveis que receberão o valor da velocidades que serão enviadas para os ESC's
int speedL; //Valor de velocidade para o ESC da esquerda
int speedR; //Valor de velocidade para o ESC da direita

String direction = ""; //Variável que indicará o sentido determinado pelos sensores
String strategy = "simple"; //Por enquanto, eu estou escrevendo no código, depois vai ser decidido de acordo com o controle do Juiz

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
  
  pinMode(SLeftPin, OUTPUT);
  pinMode(SCenterPin, OUTPUT);
  pinMode(SRightPin, OUTPUT);

  digitalWrite(SLeftPin, LOW);
  digitalWrite(SCenterPin, LOW);
  digitalWrite(SRightPin, LOW);
  
  pinMode(SLeftPin, INPUT);
  sensorL.init(true);
  sensorL.setAddress((uint8_t)0x21); //endereço do sensor da esquerda

  pinMode(SCenterPin, INPUT);
  sensorC.init(true);
  sensorC.setAddress((uint8_t)0x23); //endereço do sensor da frente

  pinMode(SRightPin, INPUT);
  sensorR.init(true);
  sensorR.setAddress((uint8_t)0x25); //endereço do sensor da direita

  sensorL.setTimeout(100);
  sensorC.setTimeout(100);
  sensorR.setTimeout(100);
  
  //Configurando o sinal PWM que será enviado aos ESC's
  ESCL.setPeriodHertz(50);             // Estabelece a frequência do PWM (50Hz)
  ESCL.attach(ESCLPin, 1000,2000);     // (Pino onde será enviado o sinal, largura de pulso mínima, largura de pulso máxima)
  ESCR.setPeriodHertz(50);              
  ESCR.attach(ESCRPin, 1000,2000);     
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
 
void loop() {
  //Armazena os valores lidos nas respectivas variáveis
  distL = sensorL.readRangeSingleMillimeters();
  distC = sensorC.readRangeSingleMillimeters();
  distR = sensorR.readRangeSingleMillimeters();  

  //Tomadas de decisão
  //Os valores aqui utilizados servem apenas para um teste de tomada de decisão
  //O número "200" é um número aleatório apenas para o teste

  //TODO: Alterar pra comerçar verificando as distC/L/R
  if(flag == -1 && distC > 5000 && distL > 5000 && distR > 5000 ){
    speedL = 0;
    speedR = 0;
    direction = "COND. INICIAL";
  }
  
  switch(strategy) {
    case "simple":
      simpleStrategy();
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
