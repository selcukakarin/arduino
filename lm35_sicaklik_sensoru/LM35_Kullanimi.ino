int buzzer = 2;
float sicaklik;
float deger;
int lm35 = A0;

void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  deger = analogRead(lm35);
  deger = (deger / 1024) * 5000;  // milivolt cinsinden degeri bulduk
  sicaklik = deger / 10;  // sicaklik degerini bulduk. 10'a bolmemizin sebebi her 10 mv'a 1c(santigrat) lik deger geliyor
  Serial.print("Sıcaklık:");
  Serial.println(sicaklik);
  delay(1000);
  if (sicaklik > 27)
  {
    digitalWrite(buzzer, HIGH);
  }
  else
  {
    digitalWrite(buzzer, LOW);
  }
}
