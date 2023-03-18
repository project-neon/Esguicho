#include <esp_now.h>
#include <WiFi.h>

uint8_t broadcastAddress[] = {0xB8, 0xD6, 0x1A, 0xAA, 0x2D, 0xFC}; //COLOQUE os valores do endereço MAC do receptor

//Estrutura da mensagem que será enviada
//DEVE SER A MESMA ESTRUTURA NO RECEPTOR
typedef struct struct_message {
  int spdRight;
  int spdLeft;
  String dir;
  int spdInt;
} struct_message;


struct_message mySpd;			//Cria um objeto chamado mySpd

esp_now_peer_info_t peerInfo;	//Cria um objeto chamado peerInfo

// Função callback chamada ao enviar algum dado
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


#define direction 32	// está marcado como D32 no DevKit
#define speed 39		// está marcado como VN no DevKit

int valorDir = 0;
int valorSpd = 0;

/*
PINOS UTILIZADOS

ENTRADAS
PINO 32 -- DIREITA OU ESQUERDA (potenciômetro)
PINO 39 -- FRENTE OU TRÁS (potenciômetro)

*/
 
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
  esp_now_register_send_cb(OnDataSent);
  
  // Registra o dispositivo que receberá os dados (peer)
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Adiciona o dispositivo que receberá os dados (peer)  
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

}
 
void loop() {

  valorDir = analogRead(direction);
  valorSpd = analogRead(speed);

  mySpd.spdInt = map(valorSpd, 0, 4095, 0, 100);
  valorDir = map(valorDir, 0, 4095, -100, 100);


  if (valorDir > 10){
      mySpd.spdLeft = mySpd.spdInt;
      mySpd.spdRight = mySpd.spdInt - mySpd.spdInt*valorDir/100;
      mySpd.dir = "RIGHT";
  }else if(valorDir < -10){
      mySpd.spdLeft = mySpd.spdInt + mySpd.spdInt*valorDir/100;
      mySpd.spdRight = mySpd.spdInt;
      mySpd.dir = "LEFT";
  }else{
      mySpd.spdLeft = mySpd.spdInt;
      mySpd.spdRight = mySpd.spdInt;
      mySpd.dir = "CENTRO";
  }
 
  Serial.print("VD: ");
  Serial.print(mySpd.spdRight);
  Serial.print("\t");
  Serial.print("VE: ");
  Serial.print(mySpd.spdLeft);
  Serial.print("\t");
  Serial.print("|INT: ");
  Serial.print(mySpd.spdInt);
  Serial.print("\t");
  Serial.print("DIR: ");
  Serial.print(mySpd.dir);
  Serial.print("\t");
  
  // Envia os dados via ESP-NOW a cada 200ms
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &mySpd, sizeof(mySpd));
   
  if (result == ESP_OK) {
    Serial.println("Success");
  }
  else {
    Serial.println("Error");
  }
}
