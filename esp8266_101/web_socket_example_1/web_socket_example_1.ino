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
  <input type="range" min="1" max="10" value="5" id="dataRateSlider" oninput="sendDataRate()" />
  <label for="dataRateSlider" id="dataRateLabel">Rate: 0.2Hz</label>
</div>
<div style="height:600px;width:450px;">
<canvas id="line-chart" width="800" height="450"></canvas>
</div>
<div style="height:600px;width:450px;">
<canvas id="line-chart2" width="800" height="450"></canvas>
</div>
<!-- Adding a websocket to the client (webpage) --> 
<script>
  var webSocket, dataPlot, dataPlot2;
  var maxDataPoints = 20;
  function removeData(){
    dtaPlot.data.labels.shift();
    dataPlot.data.datasets[0].data.shift();
  }
  function addData(label, data){
    if(dataPlot.data.labels.length > maxDataPoints) removeData();
    dataPlot.data.labels.push(label);
    dataPlot.data.datasets[0].data.push(data);
    dataPlot.update();
    dataPlot2.data.labels.push(label);
    dataPlot2.data.datasets[0].data.push(data);
    dataPlot2.update();
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
    dataPlot2 = new Chart(document.getElementById("line-chart2"),{
      type: 'bar',
      data: {
        labels: [],
        datasets: [{
          data: [],
          label: "Temperature (C)",
          backgroundColor: 'rgba(255, 99, 132, 0.2)',
           borderColor: 'rgba(255, 99, 132, 1)',
           borderWidth: 1,
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
  function sendDataRate(){
    var dataRate = document.getElementById("dataRateSlider").value;
    webSocket.send(dataRate);
    dataRate = 1.0/dataRate;
    document.getElementById("dataRateLabel").innerHTML = "Rate: " + dataRate.toFixed(2) + "Hz";
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

  timer.attach(5, getData);
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
  // Do something with the data from the client
  if(type == WStype_TEXT){
    float dataRate = (float) atof((const char *) &payload[0]);
    timer.detach();
    timer.attach(dataRate, getData);
  }
}
