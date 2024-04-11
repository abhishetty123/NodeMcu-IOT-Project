#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <EEPROM.h>
#define mot_r_p 2
#define mot_r_m 0
#define mot_l_p 4
#define mot_l_m 5
String get_dir;
const char* ssid = "Tinker";
const char* password = "password";
int count = 0;
int default_wifi = 0;
String api_url;
String api_url_1;
int state_1 = 1;
int state_2 = 1;
String server_response;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(14, INPUT);
  pinMode(mot_l_m,OUTPUT);
  pinMode(mot_l_p,OUTPUT);
  pinMode(mot_r_m,OUTPUT);
  pinMode(mot_r_p,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
  }
  GetApi();
  if (server_response == "F")
  {
    Serial.print("Front"); 
    move_forward();  
  }
  if (server_response == "B")
  {
    Serial.print("Back");   
    move_backward();
  }
  if (server_response == "L")
  {
    Serial.print("Left");  
    move_left();
  }
  if (server_response == "R")
  {
    Serial.print("Right");  
    move_right(); 
  }
  if (server_response == "S")
  {
    Serial.print("Stop");
    robot_stop();
  }
  delay(500);
  Serial.print(digitalRead(D5));
  if (((digitalRead(14)) == 0) && (state_1 == 1)) {
    api_url_1 = "http://db.tinkerspace.in/spyrobot/udatefire.php?data=1";
    upload_data();
    state_2 = 1;
    state_1 = 0;
  } else if (((digitalRead(14)) == 1) && (state_2 == 1)) {
    api_url_1 = "http://db.tinkerspace.in/spyrobot/udatefire.php?data=0";
    upload_data();
    state_1 = 1;
    state_2 = 0;
  }
}


void wifiConnect() {
  default_wifi = 1;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
    count++;
    if (count == 8) {
      break;
    }
  }
  if (default_wifi == 0) {

    Serial.print("    WiFi Failed     ");
    delay(3000);
  }

  else {

    Serial.println("Wifi connected");
  }
  delay(1000);
}


void GetApi() {
  // for (int i = 0; i <= 10; i++)
  // {
  //   digitalWrite(wifi_led, 1);
  //   delay(100);
  //   digitalWrite(wifi_led, 0);
  //   delay(100);
  // }
  api_url = "http://db.tinkerspace.in/spyrobot/getdir.php?";
  // Serial.println("\n\nCode downloading..........");
  HTTPClient http;
  api_url.replace("\n", "");
  api_url.replace("\r", "");
  api_url.replace(" ", "%20");
  http.begin(api_url);
  Serial.println(api_url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    server_response = http.getString();
    Serial.print(server_response);
  } else {
    Serial.println("Code upload success ");
  }
  http.end();
}


void upload_data() {
  Serial.println("\n\nConnecting to DB");
  HTTPClient http;  //Declare an object of class HTTPClient
  api_url_1.replace("\n", "");
  api_url_1.replace("\r", "");
  api_url_1.replace(" ", "%20");
  http.begin(api_url_1);
  Serial.println(api_url_1);
  int httpCode = http.GET();  //Send the request

  if (httpCode > 0) {
    String server_response_1 = http.getString();  //Get the request response payload
    Serial.println(server_response_1);
  }
  http.end();  //Close connection
}


void move_forward() {
  analogWrite(mot_l_p, 1020);
  analogWrite(mot_l_m, 0);
  analogWrite(mot_r_p, 0);
  analogWrite(mot_r_m, 1020);
}

void move_backward() {
  analogWrite(mot_l_p, 0);
  analogWrite(mot_l_m, 1020);
  analogWrite(mot_r_p, 1020);
  analogWrite(mot_r_m, 0);
}

void move_left() {
  analogWrite(mot_l_p, 0);
  analogWrite(mot_l_m, 1020);
  analogWrite(mot_r_p, 0);
  analogWrite(mot_r_m, 1020);
}

void move_right() {
  analogWrite(mot_l_p, 1020);
  analogWrite(mot_l_m, 0);
  analogWrite(mot_r_p, 1020);
  analogWrite(mot_r_m, 0);
}

void robot_stop() {
  analogWrite(mot_r_p, 0);
  analogWrite(mot_r_m, 0);
  analogWrite(mot_l_p, 0);
  analogWrite(mot_l_m, 0);
}

