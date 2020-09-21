int led=2;
int buton=3;

void setup() {
 
pinMode(led,OUTPUT);
pinMode(buton,INPUT);

}

void loop() {  

int butondurumu=digitalRead(buton);
if (butondurumu==HIGH)
{
  digitalWrite(led,HIGH);
  }  
else {
    digitalWrite(led,LOW);
  }

}
