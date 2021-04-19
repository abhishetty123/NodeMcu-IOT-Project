

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

#ifndef APSSID
#define APSSID "the_ec_student"
#define APPSK  "password"
#define sensor A0
#endif
const char *ssid = APSSID;
const char *password = APPSK;
WiFiServer server(80);

void loop() {
   WiFiClient myclient  = server.available();    // server.available return to client object, it need to store into a variable myclient
 if(!myclient)  //  if there is no client then it should return back
 return;
  Serial.println("New Client");
 // now we can check client has some data available for us to read

 while(!myclient.available())  // if there is no data available from client
 {
         // wait
 }
  // put your main code here, to run repeatedly:
  int req = analogRead(sensor);

  String response = "HTTP/1.1 200 OK\r\n Content-type:text/html\r\n\r\n<!DOCTYPE HTML><HTML><BODY>sensorvalue is ";
  response += req;
  response += "</BODY></HTML>";
  myclient.print(response);
  delay(10);
}
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
//  serever.on("/", handleRoot);
  server.begin();
  Serial.println("HTTP server started");
}
