#include <IRremote.hpp>
 
#define JCONTROLLER 4
#define led 2

const int I1 = 16;
const int I2 = 2064;
const int I3 = 1040;
const int I4 = 3088;
const int I5 = 528;
const int I6 = 2576;

int stage = 0;
long value = 0;
IRrecv irrecv(JCONTROLLER);
decode_results results;
 
void setup() {
  pinMode(led, OUTPUT);
  pinMode(JCONTROLLER, INPUT);
  Serial.begin(115200);
  irrecv.enableIRIn(); // Start the receiver
}
 
void loop() {
  
  juiz();
  acao();
  
}

void juiz() {
  if (irrecv.decode(&results)) {
    value = (results.value);
    Serial.print("value: ");
    Serial.println(value);
    Serial.print("stage: ");
    Serial.println(stage);
    switch(value)
    {
      case I1:
        if(stage == 0)
        {
          Serial.println("primeiro");  
          stage = 1;
          Serial.println(stage);
        }
        break;
      case I2:
        if(stage == 1)
        {
          Serial.println("segundo");
          stage = 2;
          Serial.println(stage);
        }
        break;
      case I3:
            Serial.println("MORREU");
            digitalWrite(led, LOW);  
            stage = -10;
            Serial.println(stage);
        break;
    }
    irrecv.resume();
  }
}

void acao()
{
  switch(stage)
    {
      case 1:
          digitalWrite(led, HIGH);
          break;
      case 2:
          digitalWrite(led, HIGH);
          delay(400);
          digitalWrite(led, LOW);
          delay(400);
        break;
    }
}
