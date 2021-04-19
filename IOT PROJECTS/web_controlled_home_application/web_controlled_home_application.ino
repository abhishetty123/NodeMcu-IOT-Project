/******************************************
   THE_EC_STUDENT
   Follow for more similar project @the_ec_student
   do message for more code based on iot and embedded system
*/



//************************web controlled home application***************************
#include <ESP8266WiFi.h>
// Add wifi access point credentiaals
const char* ssid     = "redmi";
const char* password = "redmihot";
WiFiServer server(80);// Set port to 80
String header; // This storees the HTTP request
// Declare the pins to which the appliances are connected via relays
int app1 = D1; //appliance 1
int app2 = D2; //appliance 2
int app3 = D3; //appliance 3
int app4 = D4;
//you can add more more appliances below.
String app1state = "off";// state of appliance1
String app2state = "off";
String app3state = "off";// state of appliance2
String app4state = "off";
//**********************************************************************************
void setup() {
  Serial.begin(115200);
  // Set the pinmode of the pins to which the LEDs are connected and turn them low to prevent flunctuations
  pinMode(app1, OUTPUT);
  pinMode(app2, OUTPUT);
  pinMode(app3, OUTPUT);
    pinMode(app4,OUTPUT);
  digitalWrite(app1, LOW);
  digitalWrite(app2, LOW);
  digitalWrite(app3, LOW);  
  digitalWrite(app4, LOW);
  //connect to access point
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());// this will display the Ip address of the Pi which should be entered into your browser
  server.begin();
}

//*************************************************************************************
void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            //************************ turns the GPIOs on and off**************************************
            if (header.indexOf("GET /app1/on") >= 0) {
              Serial.println("App 1 on");
              app1state = "on";
              digitalWrite(app1, HIGH);
            } else if (header.indexOf("GET /app1/off") >= 0) {
              Serial.println("App 1 off");
              app1state = "off";
              digitalWrite(app1, LOW);
            } else if (header.indexOf("GET /app2/on") >= 0) {
              Serial.println("App 2 on");
              app2state = "on";
              digitalWrite(app2, HIGH);
            } else if (header.indexOf("GET /app2/off") >= 0) {
              Serial.println("App 2 off");
              app2state = "off";
              digitalWrite(app2, LOW);
            }
            else if (header.indexOf("GET /app3/on") >= 0) {
              Serial.println("App 3 on");
              app3state = "on";
              digitalWrite(app2, HIGH);
            } else if (header.indexOf("GET /app3/off") >= 0) {
              Serial.println("App 3 off");
              app3state = "off";
              digitalWrite(app3, LOW);
            }
            
            else if (header.indexOf("GET /app4/on") >= 0) {
              Serial.println("App 4 on");
              app4state = "on";
              digitalWrite(app2, HIGH);
            } else if (header.indexOf("GET /app4/off") >= 0) {
              Serial.println("App 4 off");
              app4state = "off";
              digitalWrite(app4, LOW);
            }

            // ********************************Display the HTML web page********************************************
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // ******************************CSS to style the on/off buttons*****************************************
            //******* Feel free to change the background-color and font-size attributes to fit your preferences******
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // *******************************Web Page Heading*********************************************************
            client.println("<body><h1>home application</h1>");

            // ***************************Display current state, and ON/OFF buttons for GPIO 5************************
            client.println("<p>Water Pump - State " + app1state + "</p>");
            // If Appliance 1 is off, it displays the ON button
            if (app1state == "off") {
              client.println("<p><a href=\"/app1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/app1/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            //********************* Display current state, and ON/OFF buttons for GPIO 4*****************************//
            client.println("<p>TubeLight - State " + app2state + "</p>");
            // If Appliance 2 is off, it displays the ON button
            if (app2state == "off") {
              client.println("<p><a href=\"/app2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/app2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            //**************************** Display current state, and ON/OFF buttons for GPIO 0**********************/
            client.println("<p>Fan - State " + app3state + "</p>");
            // If Appliance 2 is off, it displays the ON button
            if (app3state == "off") {
              client.println("<p><a href=\"/app3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/app3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            //******************************************************************************************************************//
            client.println("<p>Bulb - State " + app4state + "</p>");
            // If Appliance 2 is off, it displays the ON button
            if (app4state == "off") {
              client.println("<p><a href=\"/app3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/app3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
  String msg = "";

}
