int alevsensor=A0;
int buzzer=2;
void setup() {

 pinMode(buzzer,OUTPUT);
 Serial.begin(9600);

}

void loop() {
 int alevdeger=analogRead(alevsensor);
 Serial.println(alevdeger);
 delay(500);
 if (alevdeger==0)
 {
  digitalWrite(buzzer,HIGH);
  }
  else{
    digitalWrite(buzzer,LOW);
    }

}
