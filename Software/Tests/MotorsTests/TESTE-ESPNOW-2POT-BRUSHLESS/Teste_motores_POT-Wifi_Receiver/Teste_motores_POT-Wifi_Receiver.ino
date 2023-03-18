#include <esp_now.h>
#include <WiFi.h>
#include <ESP32Servo.h> 

//Estrutura damensagem que será enviada
//DEVE SER A MESMA ESTRUTURA NO EMISSOR
typedef struct struct_message {
  int rightSpd; 		//recebe o valor da velocidade da direita
  int leftSpd; 		//recebe o valor da velocidade da esquerda
	String Dir;	//recebe o valor da direção
	int intSpd;			//recebe o valor da intensidade da velocidade
} struct_message;


struct_message myData;		//Cria um objeto chamado myData

//Função de callback chamada ao receber algum dado
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.print(len);
  Serial.print("\t");
  Serial.print("VD: ");
  Serial.print(myData.rightSpd);
  Serial.print("\t");
  Serial.print("VE: ");
  Serial.print(myData.leftSpd);
  Serial.print("\t");
  Serial.print("|INT: ");
  Serial.print(myData.intSpd);
  Serial.print("\t");
  Serial.print("DIR: ");
  Serial.print(myData.Dir);
  Serial.println();
}

#define ESCPinRight 17	// está marcado como TX2 no DevKit
#define ESCPinLeft 16	// está marcado como RX2 no DevKit


Servo myESCRight;      //Cria um objeto para controlar o Servo, no caso irá controlar a velocidade do motor e sentido de rotação
Servo myESCLeft;       //Cria um objeto para controlar o Servo, no caso irá controlar a velocidade do motor e sentido de rotação
 
 
/*
PINOS UTILIZADOS

SAÍDAS
PINO 16 - RODA ESQUERDA
PINO 17 - RODA DIREITA

*/

int SpdRight = 0;
int SpdLeft = 0;

void setup() {
  // Inicia o monitor Serial
  Serial.begin(115200);
 
  // Configura o ESP32 como um Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Inicia o ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  //Configura a função de callback que será chamada ao enviar algum dado
  esp_now_register_recv_cb(OnDataRecv);
  
  myESCRight.setPeriodHertz(50);            	// Estabelece a frequência do PWM (50Hz)
  myESCLeft.setPeriodHertz(50);
  myESCRight.attach(ESCPinRight, 1000, 2000);
  myESCLeft.attach(ESCPinLeft, 1000, 2000);  	// Estrutura: (Pino onde será enviado o sinal, largura de pulso mínima, largura de pulso máxima)
												// É necessário estabelecer uma faixa de largura de pulso para que o ESC
												// responda corretamente a variação do sinal analógico lido do potenciômetro
												// O valor da largura de pulso deve ser informado na escala de micro-segundos
												// No caso, a largura de pulso do PWM mínima é de 1000us e a máxima é 2000us
}
 
void loop() {
	
  SpdRight = map(myData.rightSpd, 0, 100, 0, 180); 	// Realiza a conversão para valores entre 0 e 180 para o motor da direita
  SpdLeft = map(myData.leftSpd, 0, 100, 0, 180);		// Realiza a conversão para valores entre 0 e 180 para o motor da esquerda

  myESCRight.write(SpdRight); 	// Envia o valor convertido para o ESC da direita
  myESCLeft.write(SpdLeft); 	// Envia o valor convertido para o ESC da esquerda
}
