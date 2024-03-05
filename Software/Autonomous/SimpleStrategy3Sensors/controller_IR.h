#include <IRremote.hpp>  // Version 4.2.0

int stage = 0;

void controllerInit() {
  IrReceiver.begin(JUDGE_CONTROLLER_PIN, ENABLE_LED_FEEDBACK);
}

void controllerIR() {
  if (IrReceiver.decode()) {
    Serial.println(IrReceiver.decodedIRData.command, HEX);

    switch (IrReceiver.decodedIRData.command) {
      // caso 1: liga o led p sabermos que o IR tá funcionando
      case 0:
        Serial.println("Apertou o botão 1");
        stage = 1;
        break;
      case 0x1:
      case 0x11:
        Serial.println("Apertou o botão 2");
        stage = 2;
        break;
      case 0x2:
      case 0x12:
        Serial.println("Apertou o botão 3");
        stage = -10;
        while (1) {
          speedL = 0;
          speedR = 0;
          motorsOutput();
        }
        break;
    }

    IrReceiver.resume();
  }
}
