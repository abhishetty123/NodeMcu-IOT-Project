/* library used*/
#include <ESP8266WiFi.h>  
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "shettynet"
#define WIFI_PASS "password"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "AbhilashShetty"
#define MQTT_PASS "aio_qNkc03qRKk7nCvL41Epqhr45juTL"

int Relay1 = D0;
bool a;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/ONOF");
Adafruit_MQTT_Publish LightsStatus = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/LightsStatus");


void setup()
{
  a = true;
  Serial.begin(9600);     // set baudRate as 9600


  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi>");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(">");
    delay(50);
  }

  Serial.println("OK!");
  mqtt.subscribe(&onoff);                   //Subscribe to the onoff topic

  pinMode(Relay1, OUTPUT);                 // declaring Relay1 as a output pin of esp8266
  pinMode(D1, INPUT_PULLUP);               // declaring buttonPin as a input pullup pin 
  digitalWrite(Relay1, LOW);               // set led in off state
 // Stop if already connected
  if (mqtt.connected() && mqtt.ping())
  {
    //    mqtt.disconnect();
    return;
  }

  int8_t ret;

  mqtt.disconnect();

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0)            // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      ESP.reset();
    }
  }
  Serial.println("MQTT Connected!");
}

void loop()
{
  //Connect/Reconnect to MQTT
  Button();                       // Button function to triger bulb
  googleAssistant();               //google assistant function to trigger bulb

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
   //  if (!mqtt.ping())
  //  {
  //    mqtt.disconnect();
  //  }
}


void Button() {
  if (digitalRead(D1) == 0) {
    a = !a;
    if ( a == true) {
      Serial.println("Relay1low");
      digitalWrite(Relay1, LOW);
    }
    else if ( a == false) {
      Serial.println("Relay1high");
      digitalWrite(Relay1, HIGH);
      //delay(100);
    }
    while (digitalRead(D1) == 0);
  }
}
void googleAssistant(){
   Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &onoff)
    {

      //Print the new value to the serial monitor
      Serial.print("onoff: ");
      Serial.println((char*) onoff.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) onoff.lastread, "ON"))
      {
        //active low logic
        digitalWrite(Relay1, HIGH);
        LightsStatus.publish("ON");
      }
      else if (!strcmp((char*) onoff.lastread, "OFF"))
      {
        digitalWrite(Relay1, LOW);
        LightsStatus.publish("OFF");

      }
      else
      {
        LightsStatus.publish("ERROR");
      }
    }
    else
    {
      //LightsStatus.publish("ERROR");
    }
  }

}
