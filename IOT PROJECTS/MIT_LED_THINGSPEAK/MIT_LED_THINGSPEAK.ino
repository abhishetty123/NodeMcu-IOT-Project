/*******
   THE_EC_STUDENT
   Follow for more project @the_ec_student
   DO Message for iot and embedded projct and code
*/


#include <ThingSpeak.h>               // add librery
#include <ESP8266WiFi.h>

WiFiClient  client;
unsigned long counterChannelNumber = 1091959;                // Channel ID
const char * myCounterReadAPIKey = "OX6OGUWRA2BTIHD0";      // Read API Key
const int FieldNumber1 = 1;                                 // The field you wish to read
const int FieldNumber2 = 2;                                 // The field you wish to read

void setup()
{
  pinMode(D0, OUTPUT);                                  // D0 set as a outpin
  Serial.begin(115200);                                 //Serial begin at 115200 boudRate
  Serial.println();                                     // print in next line

  WiFi.begin("shettynet", "password");                 // write wifi name & password

  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)                  // check the status ofn wifi connected or not
  {
    delay(500);
    Serial.println(".");
  }
  Serial.println();
  Serial.println("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop()
{
  int A = ThingSpeak.readLongField(counterChannelNumber, FieldNumber1, myCounterReadAPIKey);
//  Serial.println(A);
  if (A == 0) {
    digitalWrite(D0, 1);
    Serial.println("upload");
  }
  else if (A == 1) {
    digitalWrite(D0, 0);
    Serial.println("unload");
  }
  else if (A == 2) {
    digitalWrite(D0, 0);
    Serial.println("right plus");
  }
  else if (A == 3) {
    digitalWrite(D0, 0);
    Serial.println("right min");
  }
  else if (A == 4) {
    digitalWrite(D0, 0);
    Serial.println("left plus");
  }
  else if (A == 5) {
    digitalWrite(D0, 0);
    Serial.println("left min");
  }


}
