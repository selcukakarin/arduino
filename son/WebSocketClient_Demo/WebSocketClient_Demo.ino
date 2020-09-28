#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>
#include <Ticker.h>

// Collecting sensor data
Ticker timer;

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int potdeger;
int sensorValue = 0;  // value read from the pot

const char * ssid = "HUAWEI-CE7B";
const char * password = "7D866H39F97";
//IPAddress ip(192, 168, 4, 10), gateway(192, 168, 4, 1), subnet(255, 255, 255, 0); // fixed IP
WebSocketsClient webSocket;

void setup() {
  Serial.begin(9600); // this baudrate also prints module startup data
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    webSocket.begin("http://3.121.155.177/", 80); // IP/port for Ws

    //timer.attach(5, getData);
  } else {
    Serial.println("Connection failed");
  }
}



void loop() {
  webSocket.loop();
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  String json = "{\"value\":";
  json += sensorValue;
  json += "}";
  //webSocket.broadcastTXT(json.c_str(), json.length());
  webSocket.sendTXT(json.c_str(), json.length());
}
