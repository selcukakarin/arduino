int kpin = 3;
int ypin = 5;
int mpin = 6;
void setup() {
  pinMode(kpin, OUTPUT);
  pinMode(ypin, OUTPUT);
  pinMode(mpin, OUTPUT);
}

void loop() {
 rgbRenk(0,10,255);
}

void rgbRenk(int kirmizi, int yesil, int mavi)
{
  analogWrite(kpin, kirmizi);
  analogWrite(ypin, yesil);
  analogWrite(mpin, mavi);
}
