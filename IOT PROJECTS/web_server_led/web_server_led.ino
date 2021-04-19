/*
   THE_EC_STUDENT
   follow for more related project @the_ec_student
*/


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "redmi";
const char* password = "redmihot";
const int LedPin = D0;
WiFiServer server(80);
void setup() {
  pinMode(LedPin, OUTPUT);
  Serial.begin(9600);
  connectToWiFi();
  server.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
}

void connectToWiFi() {

  Serial.print("\n\nConnecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void handleRoot() {

  Serial.println("Got a Request");

  if (server.arg(0)[0] == '1') {
    digitalWrite(LedPin, LOW);
  }

  else
  {
    digitalWrite(LedPin, HIGH);
  }
  String msg = "";

  msg += "<html><body>\n";
  msg += "<h1>LED Remote</h1>";
  msg += "<h2><a href='?a=1'/>ON</a></h2>";
  msg += "<h2><a href='?a=0'/>OFF</a></h2>";
  msg += "</body></html>";

  server.send(200, "text/html", msg);
}
