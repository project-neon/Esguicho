#include <ESP32Servo.h>
Servo ESCL;
Servo ESCR;

//Variáveis que receberão o valor da velocidades que serão enviadas para os ESC's
int speedL = 0; //Valor de velocidade da esquerda
int speedR = 0; //Valor de velocidade da direita

int EscSpeedL = 0; //Valor de velocidade para o ESC da esquerda
int EscSpeedR = 0; //Valor de velocidade para o ESC da direita

void motorsInit() {
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
    ESCL.write(92); 
    ESCR.write(92);
}


void motorsOutput() {
    EscSpeedL = map(speedL, -100, 100, 0, 180);
    EscSpeedR = map(speedR, -100, 100, 0, 180);

    //TODO: Ver se funciona
    ESCL.write(EscSpeedL); 
    ESCR.write(EscSpeedR);
}