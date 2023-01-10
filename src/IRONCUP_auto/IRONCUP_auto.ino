//Bibliotecas internas
#include "estrategias.h"
#include "variables_declaration.h"

void setup(){
  
  Serial.begin(115200);
  
  //Iniciando o endereçamento dos sensores
  Wire.begin();
  irrecv.enableIRIn(); // Start the receiver
  pinMode(JCONTROLLER, INPUT);
  pinMode(2, OUTPUT);
   
  pinMode(SDIST_L, OUTPUT);
  pinMode(SDIST_C, OUTPUT);
  pinMode(SDIST_R, OUTPUT);

  digitalWrite(SDIST_L, LOW);
  digitalWrite(SDIST_C, LOW);
  digitalWrite(SDIST_R, LOW);
  
  pinMode(SDIST_L, INPUT);
  sensorL.init(true);
  sensorL.setAddress((uint8_t)0x21); //endereço do sensor da esquerda

  pinMode(SDIST_C, INPUT);
  sensorC.init(true);
  sensorC.setAddress((uint8_t)0x23); //endereço do sensor da frente

  pinMode(SDIST_R, INPUT);
  sensorR.init(true);
  sensorR.setAddress((uint8_t)0x25); //endereço do sensor da direita

  sensorL.setTimeout(100);
  sensorC.setTimeout(100);
  sensorR.setTimeout(100);
  
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
}

void loop() {
  //Armazena os valores lidos nas respectivas variáveis 
  distL = sensorL.readRangeSingleMillimeters();
  distC = sensorC.readRangeSingleMillimeters();
  distR = sensorR.readRangeSingleMillimeters(); 
  if(stage == 0){
    if((distL > 60000) or (distC > 60000) or (distR > 60000)) {
      digitalWrite(2, HIGH);
    }
    else{
      digitalWrite(2, LOW);
    }
  }
  
  if(distL > 600) distL = 600;
  if(distC > 600) distC = 600;
  if(distR > 600) distR = 600;
    
  juiz();

  if(stage == 1) {
    if((millis() - contador) < 300) digitalWrite(2, HIGH); // Acende o led do pino 2
    else digitalWrite(2, LOW); // Apaga o led do pino 2
    if((millis() - contador) > 600) contador = millis(); // Verifica se já passou 600 milisegundos
    speedL = 0;
    speedR = 0;
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  else if(stage == 2) {
    //inicio das decisões
    if(distC < 100 or (distL < distAtk and distR < distAtk)){
      Serial.print("ATACANDO \t\t");
      speedL = speedR = speedMax;
    }
    else if (distL < distAtk or distR < distAtk){
      (distL < distAtk) ? Serial.print("ESQ \t\t") : Serial.print("DIR \t\t");
      speedL = (distL < distAtk) ? speedMax*0.3 : speedMax;
      speedR = (distL < distAtk) ? speedMax : speedMax*0.3;
      flag = (distL < distAtk) ? -1 : 1;
    }
    else{
      (flag == -1) ? Serial.print("PROCURANDO ESQ \t\t") :  Serial.print("PROCURANDO DIR \t\t");;
      speedL = (flag == -1) ? -1*searchSpeed : searchSpeed;
      speedR = (flag == -1) ? searchSpeed : -1*searchSpeed;
    }
  }
  //fim das decisões
  /////////////////////////////////////////////////////////////////////////////////////////////
  
  else{
    speedL = 0;
    speedR = 0;
  }
  
  // Mostra o valor que será enviado ao map
  Serial.print("speedL: ");
  Serial.print(speedL);
  Serial.print("\t");
  Serial.print("speedR: ");
  Serial.print(speedR); 
  Serial.print("\t\t");
  
  speedL = map(speedL, -100, 100, 0, 180);
  speedR = map(speedR, -100, 100, 0, 180);
  ESCL.write(speedL); 
  ESCR.write(speedR);
  
 

  
  // Mostra o valor de cada sensor na tela e a decisão escolhida
  Serial.print("L: ");
  Serial.print(distL);
  Serial.print("\t");
  Serial.print("C: ");
  Serial.print(distC);
  Serial.print("\t");
  Serial.print("R: ");
  Serial.print(distR);
  Serial.println("\t\t");
}
