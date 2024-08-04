
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>
SoftwareSerial VSerial(D2, D3); // RX, TX
//=============================ESP_AP CREDENTIALS======================================
const char* ssid = "jio";
const char* password = "11223344";
int max_connection = 8;

//=====================================================================================

//===============================STATIC IP DECLARATION=================================
IPAddress staticIP(10,10,10,1); // IP address from Admin pannel 
IPAddress gateway(10,10,10,1);
IPAddress subnet(255,255,255,0);
//=====================================================================================

int httpCode;

HTTPClient http;

ESP8266WebServer server(80); //Server on port 80

//==============================================================
//     This rutine is exicuted when you open its IP in browser
//==============================================================
void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");
}

void handleIncomingData() {
  int vibration = server.arg("plain").toInt();
  //Serial.print("Received vibration: ");
  Serial.println(vibration);
  VSerial.println(vibration);

}
//===============================================================
//                  SETUP
//===============================================================

void setup(void){
 
  Serial.begin(115200);
  VSerial.begin(115200);
  Serial.println("");
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password, 1, false, max_connection); 
  WiFi.softAPConfig(staticIP, gateway, subnet);//Start HOTspot removing password will disable security
  Serial.printf("Stations connected to soft-AP = %d\n", WiFi.softAPgetStationNum());

  IPAddress myIP = WiFi.softAPIP(); //Get IP address
  Serial.print("HotSpt IP:");
  Serial.println(myIP);

  server.on("/data", handleIncomingData);
  server.begin();
}

//===============================================================
//                     LOOP
//===============================================================
void loop(void){

      server.handleClient();          //Handle client requests
} 