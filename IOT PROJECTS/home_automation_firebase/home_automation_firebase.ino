/*
   Created by Pi BOTS MakerHub
   Email: pibotsmakerhub@gmail.com
   Github: https://github.com/pibotsmakerhub
   Copyright (c) 2020 Pi BOTS MakerHub
*/


//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
/// Install DHT11 Library and Adafruit Unified Sensor Library


#define FIREBASE_HOST "fir-iot-bbe05.firebaseio.com" //Without http:// or https:// schemes
#define FIREBASE_AUTH "Kwy3nPckcV993tBcdg0tm0v751ipUWYo4EMsA46l"
#define WIFI_SSID "shettynet"
#define WIFI_PASSWORD "password"

//#define DHTPIN 2 // Connect Data pin of DHT to D2
# define potPin A0 
int led = D0;     // Connect LED to D5

//#define DHTTYPE    DHT11
//DHT dht(DHTPIN, DHTTYPE);

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;

FirebaseJson json;


void setup()
{

  Serial.begin(9600);

//  dht.begin();
  pinMode(led, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void sensorUpdate() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float a = analogRead(potPin);

  Serial.print("humidity");
  Serial.println(a);
  
//  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t))
//  {
//    Serial.println("PASSED");
//    Serial.println("PATH: " + firebaseData.dataPath());
//    Serial.println("TYPE: " + firebaseData.dataType());
//    Serial.println("ETag: " + firebaseData.ETag());
//    Serial.println("------------------------------------");
//    Serial.println();
//  }
//  else
//  {
//    Serial.println("FAILED");
//    Serial.println("REASON: " + firebaseData.errorReason());
//    Serial.println("------------------------------------");
//    Serial.println();
//  }

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/humidity", a))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
}
void loop() {
  sensorUpdate();

  if (Firebase.getString(ledData, "/FirebaseIOT/led")) {
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
      digitalWrite(led, HIGH);
    }
    else if (ledData.stringData() == "0") {
      digitalWrite(led, LOW);
    }
  }
  delay(100);
}
