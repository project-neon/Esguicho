#include <Arduino.h>

//motors (left, right)
#define MOTOR_L 16
#define MOTOR_R 17

//TODO: Definir sensor left, middle, right
//Distance sensors (left, middle, right)
#define SDIST_L 23
#define SDIST_C 19
#define SDIST_R 18

//Border sensors (left, right)
#define SBORDER_L 34
#define SBORDER_R 39

//JUDGE CONTROLLER
#define JCONTROLLER 4

// TODO: Defenir melhor SCL e SCA
//SCL, SDA FOR DISTANCE SENSORS
#define SCL_SDIST 22
#define SDA_SDIST 21

//Backup analogs pins
#define ANALOG_1 25
#define ANALOG_2 26
#define ANALOG_3 32
#define ANALOG_4 33

//Variáveis que vão receber os valores lidos dos pinos dos sensores
int distL;  //Valor lido pelo sensor da esquerda
int distC;  //Valor lido pelo sensor da frente
int distR;  //Valor lido pelo sensor da direita

int distMax = 200;  //Distância máxima permitida como referência para os sensores (em milímetros)

//Variáveis que receberão o valor da velocidades que serão enviadas para os ESC's
int speedL; //Valor de velocidade para o ESC da esquerda
int speedR; //Valor de velocidade para o ESC da direita