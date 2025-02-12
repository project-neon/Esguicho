void printSpeed() {
  // Mostra o valor que será enviado ao map
  Serial.print("speedL: ");
  Serial.print(speedL);
  Serial.print("\t");
  Serial.print("speedR: ");
  Serial.print(speedR);
  Serial.print("\t\t");
}


void printDistanceSensorsValues() {
  // Mostra o valor de cada sensor na tela
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
