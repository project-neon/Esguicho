//TODO: Mudar o nome do arquivo pra pinout.h?
#include <Arduino.h>

//motors (left, right)
#define MOTOR_L 16
#define MOTOR_R 17

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
