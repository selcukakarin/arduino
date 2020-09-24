#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include <Ticker.h>

// Collecting sensor data
Ticker timer;

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int potdeger;
int sensorValue = 0;  // value read from the pot

// Connecting to the Internet

const char * ssid = "HUAWEI-CE7B";
const char * password = "7D866H39F97";

// Running a web server
ESP8266WebServer server;

// Adding a websocket to the server
WebSocketsServer webSocket = WebSocketsServer(81);

// Serving a web page (from flash memory)
char webpage[] PROGMEM = R"=====(
<html>
<head>
  <script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js'></script>
</head>
<body onload="javascript:init()">
<div>
<canvas id="line-chart" width="800" height="450"></canvas>
</div>
<!-- Adding a websocket to the client (webpage) -->
<script>
  var webSocket, dataPlot;
  function addData(label, data){
    dataPlot.data.labels.push(label);
    dataPlot.data.datasets[0].data.push(data);
    dataPlot.update();
  }
  function init(){
    webSocket = new WebSocket('ws://' + window.location.hostname + ':81/');
    dataPlot = new Chart(document.getElementById("line-chart"),{
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          data: [],
          label: "Temperature (C)",
          borderColor: "#3e95cd",
          fill: false
        }]
      }
    });
    webSocket.onmessage = function(event){
      var data = JSON.parse(event.data);
      console.log(data);
      var today = new Date();
      var t = today.getHours() + ":" + today.getMinutes() + ":" + today.getSeconds();
      addData(t, data.value);
      
    }
  }
</script>
</body>
</html>
)=====";

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid,password);
  Serial.begin(9600);
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  server.on("/",[](){
    server.send_P(200, "text/html", webpage);
  });

  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  timer.attach(1, getData);
}

void loop() {
  // put your main code here, to run repeatedly:
  webSocket.loop();
  server.handleClient();
}

void getData(){
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  String json = "{\"value\":";
  json += sensorValue;
  json += "}";
  webSocket.broadcastTXT(json.c_str(), json.length());
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length){
  
}
