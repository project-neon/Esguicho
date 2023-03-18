//Bibliotecas internas
#include "_config.h"
#include "funcoes.h"

//92: valor enviado no "ESC.write" pra inicializacao dos motores e para parar a rotacao
//ESC2: ESC da Esquerda (conectores da ESC com o motor-- fio vermelho com vermelho, preto com preto)
//ESC1: ESC da Direita (conectores da ESC com o motor-- fio vermelho com preto, preto com vermelho)

void setup(){
  Serial.begin(115200);
  motores_inicializacao();
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
}

void loop() {

  geral_test();
  /**o teste geral vai ligar o led, girar o motor esquerdo nos dois sentidos, em seguida o direito nos dois sentidos
  e simulara o robo seguindo em frente, tras, giro anti-horario, giro horario, desligar o led.
  **/

}
