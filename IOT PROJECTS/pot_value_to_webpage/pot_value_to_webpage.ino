
#include<ESP8266WiFi.h>   //   include ESP8266 library
const char* ssid= "shettynet";    // wifi name
const char* pass= "password";  // wifi password

#define sensor A0

WiFiServer server(80);    //  WiFiServer is a class , creat a server object, here server to be active on PORT 80

void setup() {
   
  Serial.begin(115200);   // Initialaztion Serial monitor  with 115200 bps
  WiFi.begin(ssid,pass);  // Wifi Initialization with SSID and PASSWORD
  Serial.print("Connecting to ");
  Serial.print(ssid);  // Print connecting to Wifi name
 // here is waiting to connect internet
  while(WiFi.status()!=WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println(" ");
  Serial.println("connected succesfully");


  server.begin();   //  begin the server
  Serial.print("Server begin at IP ");
  Serial.println(WiFi.localIP());  // print IP address of Node MCU
  

}

void loop() {
  // now here the server has to keep listening for clients

 WiFiClient myclient  = server.available();    // server.available return to client object, it need to store into a variable myclient
 if(!myclient)  //  if there is no client then it should return back
 return;

 // if there is client then come inside
 Serial.println("New Client");
 // now we can check client has some data available for us to read

 while(!myclient.available())  // if there is no data available from client
 {
         // wait
 }

 // if there is avaailable data from client then we have to read and store in String variable

 String request = myclient.readStringUntil('\n'); //  it will read untill \n 
 myclient.flush();  // flush out extra line from request
 Serial.println(request);

 int req = analogRead(sensor);

 String response ="HTTP/1.1 200 OK\r\n Content-type:text/html\r\n\r\n<!DOCTYPE HTML><HTML><BODY>sensorvalue is ";
 response+= req;
 response+= "</BODY></HTML>";
myclient.print(response);
delay(10);  
//
  

}
