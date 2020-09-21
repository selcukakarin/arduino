int ldr = A0;
int led = 2;
void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int ldrdeger = analogRead(ldr);
  Serial.println(ldrdeger);
  delay(1000);
  if (ldrdeger < 3000) {
    digitalWrite(led, HIGH);
  }
  else {
    digitalWrite(led, LOW);
  }
}
