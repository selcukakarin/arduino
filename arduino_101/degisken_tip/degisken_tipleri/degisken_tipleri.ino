bool led = HIGH;
//Veya
//boolean led=0;
char karakter1 = 'a';
char karakter2 = 97;
byte a = 79;
float deger = 3.2476;
int sayi = 596;
unsigned int sayim = 378695;
long uzunsayi = 6545447;
unsigned long cokuzunsayi = 9765145;
String metin = "Hasari KAŞKAN";
void setup() {
  Serial.begin(9600);

}

void loop() {

  Serial.print("Boolean Tipi:");
  Serial.println(led);
  delay(1000);
  Serial.print("a:");
  Serial.println(a);
  delay(1000);
  Serial.print("Karakter Verisi:");
  Serial.println(karakter1);
  delay(1000);
  Serial.print("Karakter Verisi ACII :");
  Serial.println(karakter2);
  delay(1000);
  Serial.print("Ondalık Sayı:");
  Serial.println(deger);
  delay(1000);
  Serial.print("İntiger sayi:");
  Serial.println(sayi);
  delay(1000);
  Serial.print("Unsigned sayı:");
  Serial.println(sayim);
  delay(1000);
  Serial.print("Uzun Sayı:");
  Serial.println(uzunsayi);
  delay(1000);
  Serial.print("Çok Uzun Sayı:");
  Serial.println(cokuzunsayi);
  delay(1000);
  Serial.print("Metin:");
  Serial.println(metin);
  delay(1000);
}
