#include <ESP32Servo.h>
Servo ESCL;
Servo ESCR;

int motorUnbiasedVel = 92; //Velocidade 0 da ESC


void motores_inicializacao(){
  ESCL.setPeriodHertz(50);             // Estabelece a frequência do PWM (50Hz)
  ESCR.setPeriodHertz(50);
  ESCL.attach(MOTOR_L, 1000,2000);     // (Pino onde será enviado o sinal, largura de pulso mínima, largura de pulso máxima)
  ESCR.attach(MOTOR_R, 1000,2000);
  ESCL.write(motorUnbiasedVel);
  ESCR.write(motorUnbiasedVel);
/*
    * É necessário estabelecer uma faixa de largura de pulso para que o ESC
    * responda corretamente a variação do sinal analógico lido do potenciômetro
    * O valor da largura de pulso deve ser informado na escala de micro-segundos
    * No caso, a largura de pulso do PWM mínima é de 1000us e a máxima é de 2000us
    */
}

void motorL_test(){
  //gira 80% pra frente, dps para, dps 80% pra tras

  delay(1500);
  ESCL.write(162);
  delay(500);
  ESCL.write(motorUnbiasedVel);
  delay(500);
  ESCL.write(18);
  delay(500);
  ESCL.write(motorUnbiasedVel);
  delay(500);
}

void motorR_test(){
  //gira 80% pra frente, dps para, dps 80% pra tras
  delay(500);
  ESCR.write(162);
  delay(500);
  ESCR.write(motorUnbiasedVel);
  delay(500);
  ESCR.write(18);
  delay(500);
  ESCR.write(motorUnbiasedVel);
  delay(500);
}

void motores_frente_parado_tras(){
  //gira 80% pra frente, dps para, dps 80% pra tras
  delay(500);
  ESCL.write(162);
  ESCR.write(162);
  delay(500);
  ESCL.write(motorUnbiasedVel);
  ESCR.write(motorUnbiasedVel);
  delay(500);
  ESCL.write(18);
  ESCR.write(18);
  delay(500);
  ESCL.write(motorUnbiasedVel);
  ESCR.write(motorUnbiasedVel);
  delay(500);
}

void girar_parado_girar2(){
  //gira 80% pra frente, dps para, dps 80% pra tras
  delay(500);
  ESCL.write(18);
  ESCR.write(162);
  delay(500);
  ESCL.write(motorUnbiasedVel);
  ESCR.write(motorUnbiasedVel);
  delay(500);
  ESCL.write(162);
  ESCR.write(18);
  delay(500);
  ESCL.write(motorUnbiasedVel);
  ESCR.write(motorUnbiasedVel);
  delay(500);
}

void general_test(){
  delay(1000);
  digitalWrite(2, HIGH);
  motorL_test();
  delay(1000);
  motorR_test();
  delay(1000);
  motores_frente_parado_tras();
  delay(1000);
  girar_parado_girar2();
  digitalWrite(2, LOW);
  delay(1000);
}
