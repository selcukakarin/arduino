/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
//#define SERVER_IP "10.0.1.7:9080" // PC address with emulation on host
#define SERVER_IP "3.121.155.177"

#ifndef STASSID
#define STASSID "HUAWEI-CE7B"
#define STAPSK  "7D866H39F97"
#endif

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
int potdeger;
int sensorValue = 0;  // value read from the pot

const byte numChars = 60;
char receivedChars[numChars];
char tempChars[numChars];
char messageFromArd[numChars] = {0};
boolean newData = false;

int coppb = 0;
int cotmp = 0;
int corh = 0;
int so2ppb = 0;
int so2tmp = 0;
int so2rh = 0;
int co2ppm = 0;
double co2tmp = 0.0;
double co2rh = 0.0;
double dust = 0.0;
double no2ppm = 0.0;

void setup() {

  Serial.begin(9600);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http;

    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        /*
         * 
         * SEND DATA TO SERVER HERE
         * 
         */
        newData = false;
    }
    
    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://3.121.155.177/"); //HTTP
    http.addHeader("Content-Type", "application/json");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    sensorValue = analogRead(analogInPin);
    String json = "{\"val\":";
    json += sensorValue;
    json += "}";
    int httpCode = http.POST(json);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(3000);
}

///----- RECEIVE WITH MARKERS -----\\\

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++; 
                if (ndx >= numChars) {     //IF DATA IS LARGER THAN 32 CHARS TRIM IT
                    ndx = numChars - 1;
                } 
            }
            else { //THE STRING HAS REACHED THE END
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        } //END IF

        else if (rc == startMarker) {
            recvInProgress = true;
        } //END ELSE IF
    } //END WHILE
} //END recvWithStartEndMarkers

///----- PARSE DATA -----\\\

void parseData() {        // split the data into its parts

    char * strtokIndx;    // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,","); // get the first part - the string
    strcpy(messageFromArd, strtokIndx); // copy it to messageFromArd
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    coppb = atoi(strtokIndx);       // convert this part to an integer

    strtokIndx = strtok(NULL, ",");
    cotmp = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    corh = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    so2ppb = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    so2tmp = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    so2rh = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    co2ppm = atoi(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    co2tmp = atof(strtokIndx);        // convert this part to a float/double

    strtokIndx = strtok(NULL, ",");
    co2rh = atof(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    dust = atof(strtokIndx);

    strtokIndx = strtok(NULL, ",");
    no2ppm = atof(strtokIndx);
}
