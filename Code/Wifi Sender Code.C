#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h>
SoftwareSerial VSerial(D0, D1); // RX, TX
int num = 0;
WiFiClient wifiClient;
const char* ssid ="jio";
const char* password ="11223344";
IPAddress ip(10,10,10,55); 
IPAddress gateway(10,10,10,1); 
IPAddress subnet(255,255,255,0);
String message = "10101010";
 ESP8266WebServer server(80);
void handleRoot() {
   server.send(200, "text/html", message);
   
}

//=========================================================================================
//                       SETUP 
//=========================================================================================
void setup() {
   server.on("/", handleRoot);

    server.begin();
    Serial.begin(115200);
    VSerial.begin(115200);
    delay(500);
    WiFi.config(ip,gateway,subnet);
    for(uint8_t t = 4; t > 0; t--) {
        Serial.printf("[SETUP] WAIT ...\n", t);
        Serial.flush();
        delay(1000);
    }
      WiFi.mode(WIFI_STA);
      WiFi.disconnect(true);
    WiFi.begin(ssid,password);
    if(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.println("Connected");
  }

   
}
//========================================================================================
//                 LOOP  
//=======================================================================================
void loop() {
  int state =0;
  if (Serial.available() > 0) { // Check if data is available on serial port
    num = Serial.parseInt(); // Read the integer variable from serial
    String payload = String(num);
    //Serial.print("Received payload: ");
    Serial.println(payload);
    
    // wait for WiFi connection
    if(WiFi.status() == WL_CONNECTED) { 
     // Serial.println("WiFi connected");
     // delay(10); // Add a delay of 1 second
      
      HTTPClient http;
      http.begin(wifiClient, "http://10.10.10.1/data"); // Replace with the receiving NodeMCU's IP address and endpoint URL
      http.addHeader("Content-Type", "text/plain");
      
      int httpResponseCode = http.POST(payload);
      //Serial.print("HTTP response code: ");
     // Serial.println(httpResponseCode);
      
      http.end();     
    }
    else {
      Serial.println("WiFi not connected");
    }
  }
}

