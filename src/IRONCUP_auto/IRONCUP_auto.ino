//Bibliotecas internas
#include "_config.h"
#include "estrategias.h"
#include "variables_declaration.h"
#include "distance_sensors.h"
#include "utils.h"

void setup(){
  
  Serial.begin(115200);
  
  //Iniciando o endereçamento dos sensores
  Wire.begin();
  irrecv.enableIRIn(); // Start the receiver
  pinMode(JCONTROLLER, INPUT);
  pinMode(2, OUTPUT);
   
  sensorsInit();
  
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

    EscSpeedL = map(speedL, -100, 100, 0, 180);
    EscSpeedR = map(speedR, -100, 100, 0, 180);

    //TODO: Ver se funciona
    ESCL.write(EscSpeedL); 
    ESCR.write(EscSpeedR);
  }
  //fim das decisões
  /////////////////////////////////////////////////////////////////////////////////////////////

  printSpeed();
  printDistances();
  
}
