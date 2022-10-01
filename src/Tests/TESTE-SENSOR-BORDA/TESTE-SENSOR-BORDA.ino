#define SBORDER_L 34
#define SBORDER_R 39

int RBorder_L = 0;
int RBorder_R = 0;

void setup() {
  pinMode(SBORDER_L, OUTPUT);
  pinMode(SBORDER_R, OUTPUT);
  Serial.begin (9600);
}

void loop() {
  RBorder_L = analogRead (SBORDER_L);
  RBorder_R = analogRead (SBORDER_R);
  Serial.print("Esquerda: ");
  Serial.print(RBorder_L);
  Serial.print("    Direita: ");
  Serial.println(RBorder_R);
  delay(100);
}
