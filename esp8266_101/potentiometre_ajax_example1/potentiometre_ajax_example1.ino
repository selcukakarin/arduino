#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80); // 80 is the port number

const char * ssid = "HUAWEI-CE7B";
const char * password = "7D866H39F97";
int inc=0;
String Website,data,XML,Javascript;
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

int sensorValue = 0;  // value read from the pot

void javascriptContent(){
  Javascript = "<SCRIPT>\n";
  //Javascript += "alert('hello');\n";
  
  Javascript += "</SCRIPT>\n";
}

void WebsiteContent(){
  javascriptContent();
  Website="<html>"+data+"</html>";
  Website+=Javascript;
  server.send(200,"text/html",Website );
}
void XMLcontent(){
  XML = "<?xml version='1.0'?>";
  XML+="<data>";
  XML+=data;
  XML+="</data>";
  server.send(200,"text/xml",XML);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  Serial.print(WiFi.localIP());
  server.on("/",WebsiteContent);
  server.on("/xml",XMLcontent);
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  inc = inc + 1;
  sensorValue = analogRead(analogInPin);
  data=(String)sensorValue;
  delay(1000);
  //data=(String)inc;
  server.handleClient();
  
}
