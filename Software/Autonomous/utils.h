void printSpeed() {
  // Mostra o valor que será enviado ao map
  Serial.print("speedL: ");
  Serial.print(speedL);
  Serial.print("\t");
  Serial.print("speedR: ");
  Serial.print(speedR);
  Serial.print("\t\t");
}


void printDistances() {
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
