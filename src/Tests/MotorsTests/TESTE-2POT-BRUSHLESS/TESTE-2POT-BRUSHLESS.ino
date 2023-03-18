/*
 * Código para testar 2 motores brushless
 * o controle será feito através de 2 potenciômetros,
 * onde o primeiro potenciômetro irá determinar o sentido de rotação e a velocidade
 * e o segundo potenciômetro irá determinar para qual lado o robô irá fazer a curva
 */

//Inclui as bibliotecas necessárias
#include <ESP32Servo.h> //Biblioteca utilizada para comunicar com a ESC de cada motor

//Cria os objetos para  cada ESC
Servo ESCL; //ESC que controla o motor da esquerda
Servo ESCR; //ESC que controla o motor da direita

//Pinos GPIO usados para conectar o ESP32 a cada ESC
int ESCLPin = 16; //Pino para controlar o ESC da esquerda
int ESCRPin = 17; //Pino para controlar o ESC da direita

//Pinos GPIO usados para conectar o ESP32 a cada potenciômetro
int potPinV = 39;   // Pino GPIO usado para receber o sinal do potenciômetro VELOCIDADE
int potPinD = 32;   // Pino GPIO usado para receber o sinal do potenciômetro DIREÇÃO

//Variáveis que vão receber os valores lidos dos pinos dos potenciômetros
int vel;            // variável com o valor do potPinV
int dir;            // variável com o valor do potPinD

//Variáveis que receberão o valor da velocidades que serão enviadas para os ESC's
int speedL; //Valor de velocidade para o ESC da esquerda
int speedR; //Valor de velocidade para o ESC da direita

int margem = 10; //margem para centralizar o potenciômetro

String Dir = ""; //Variável que indicará o sentido determinado pelos sensores


void setup(){
  
  Serial.begin(115200);
  
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
 
void loop() {
  /*
   * IMPORTANTE
   * 
   * Para esse teste, foi utilizado apenas ESC's giram os motores em apenas um sentido, ou seja,
   * ESC's que NÃO SÃO BIDIRECIONAIS, logo, os valores de máximo e mínimo do potenciômetro relacionado a velocidade
   * irá determinar apenas os valores de máximo e mínimo da velocidade em apenas um sentido de rotação.
   */
  vel = analogRead(potPinV);        // Lê o valor do potenciômetro da velocidae ( 0 - 4095 )
  dir = analogRead(potPinD);        // Lê o valor do potenciômetro da direção ( 0 - 4095 )    
       
  vel = map(vel, 0, 4095, 0, 100);    // Converte o valor lido para valores entre 0 e 100
  dir = map(dir, 0, 4095, -100, 100); // Converte o valor lido para valores entre -100 e 100
  /*
   * Para determinar o lado para qual o robô irá realizar a curva,
   * os valores lidos pelo potenciômetro são convertidos em valores entre -100 e 100.
   * Isso é pelo fato de que se o potenciômetro estiver rotacionado totalmente para a esquerda, o valor lido será 0 (zero),
   * e isso corresponde que o robô deverá fazer uma curva mais fechada para a esquerda, por isso os valores de -100 e 100
   * indicam quão fechada é a curva, e o sinal de negativo (-) ou positivo (+) indica o lado da curva.
   */
  
  // Mostra o valor lido pelos potenciômetros
  Serial.print(vel);
  Serial.print("\t\t");
  Serial.print(dir); 
  Serial.print("\t\t");

  //Tomada de decisão
  /*
   * Quanto mais o potenciômetro da direção se aproximar dos valores das extremidades,
   * maior será o valor decrementado da roda do lado para o qual o robô irá realizar a curva.
   * 
   * Isso acontece porque para realizar uma curva para a diretita, por exemplo, a roda da direita de girar 
   * mais devagar do que a roda da esquerda, para que haja a rotação do robô. Para o lado esquerdo a analogia
   * é semelhante.
   * 
   */
  //Estabelecendo uma margem para centralização do potenciômetro
  if (dir >= -margem && dir <= margem){
    dir = 0; 
  }
   
  if (dir > 0){ //Curva para a direita
      speedL = vel;
      speedR = vel - vel*dir/100;
      Dir = "RIGHT";
  }
  else if(dir < 0){ //Curva para esquerda
      speedL = vel + vel*dir/100;
      speedR = vel;
      Dir = "LEFT";
  }
  else{ //Vai para frente de acordo com o valor da velocidade
      speedL = vel;
      speedR = vel;
      Dir = "CENTRO";
  }

  // Realiza a conversão para valores entre  0 e 180
  speedL = map(speedL, 0, 100, 0, 180);    
  speedR = map(speedR, 0, 100, 0, 180);

  //Mostra os valores que serão enviados aos ESC's
  Serial.print(speedL);                  
  Serial.print("\t\t");
  Serial.print(speedR); 
  Serial.print("\t\t");
  Serial.println(Dir);

  // Envia os valores convertidos para os ESC's
  ESCL.write(speedL);                   
  ESCR.write(speedR); 
}
