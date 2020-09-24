#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>
#include <WebSocketClient.h>

// Collecting sensor data
Ticker timer;

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int potdeger;
int sensorValue = 0;  // value read from the pot

// Connecting to the Internet

const char * ssid = "HUAWEI-CE7B";
const char * password = "7D866H39F97";
char path[] = "/ws/polData/";
char host[] = "localhost";

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to the websocket server
  if (client.connect("localhost", 80)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while (1) {
      // Hang on failure
    }
  }



  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  timer.attach(5, getData);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (!ws.isConnected()) {
    ws.connect("localhost", "/", 443);
  } else {
    webSocket.loop();

    String msg;
    if (ws.getMessage(msg)) {
      Serial.println(msg);
    }
  }
  delay(500);
}

void getData() {
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  String json = "{\"value\":";
  json += sensorValue;
  json += "}";
  webSocket.broadcastTXT(json.c_str(), json.length());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  // Do something with the data from the client
  if (type == WStype_TEXT) {
    float dataRate = (float) atof((const char *) &payload[0]);
    timer.detach();
    timer.attach(dataRate, getData);
  }
}
