// Motores da Esquerda e da direita
#define MOTOR_LEFT_PIN 14 // Pino da ESC1
#define MOTOR_RIGHT_PIN 27 // Pino da ESC2

// SCL, SDA para a comunicação I2C
// Como usamos os pinos padrões da I2C, a biblioteca Wire.h cuida
// disso automaticamente para nós. Então essas variáveis não seriam necessárias.
#define SCL_PIN 22
#define SDA_PIN 21

// Define o pino XSHUT do sensor de distância VL53L0X
// Quando LOW, esse pino desliga o sensor e permite o endereçamento dos outros
int VL53LOX_XSHUT_LIST_PINS[3] = {17, 16, 4};

// Pino que retorna o valor recebido pelo controle do Juiz
#define JUDGE_CONTROLLER_PIN 39

// Pinos do de Interrupt do MPU e Motion do Mouse
#define INT_MPU 32
#define MOT_MOUSE 13

// Pino do divisor de tensao
#define DDT 33

// Backup GPIOs pins
#define GPIO_1_PIN 34
#define GPIO_2_PIN 35