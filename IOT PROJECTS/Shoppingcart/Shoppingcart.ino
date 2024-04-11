#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
// #include <Wire.h>

#include <Arduino.h>
#include "HX711.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 5;
const int LOADCELL_SCK_PIN = 4;
HX711 scale;


#define SS_PIN D4   //D2
#define RST_PIN D3  //D1
#include <SPI.h>
#include <MFRC522.h>
MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance.
int count = 0;
String api_url_1;
const char* ssid = "Tinker";
const char* password = "password";
float weight = 0;
int default_wifi = 0;
void setup() {
  //Serial.begin(9600);  // Initiate a serial communication
  // scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  SPI.begin();         // Initiate  SPI bus
  mfrc522.PCD_Init();  // Initiate MFRC522
  Wire.begin(1, 3);
  lcd.begin();  // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    Welcome    ");
  lcd.setCursor(0, 1);
  lcd.print("trolly0001");
  delay(2000);
}


void wifiConnect() {
  default_wifi = 1;
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    // //Serial.print(".");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" No WiFi Found  ");
    delay(1000);
    count++;
    if (count == 8) {
      break;
    }
  }
  if (default_wifi == 0) {

    // //Serial.print("    WiFi Failed     ");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" No WiFi Found  ");
    delay(3000);
  }

  else {

    //Serial.println("Wifi connected");
  }
  delay(1000);
}



void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    wifiConnect();
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   SCAN HERE ->  ");
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //Show UID on serial monitor
  //Serial.println();
  //Serial.print(" UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  //Serial.println();
  if (content.substring(1) == "83 A4 3B C8")  //change UID of the card that you want to give access
  {
    //Serial.println(" RICE ");
    lcd.clear();          // clear display
    lcd.setCursor(1, 0);  // move cursor to   (0, 0)
    lcd.print("Item : Rice");
    lcd.setCursor(1, 1);
    lcd.print("MRP : 100/Kg");
    setWeight();
    delay(2000);
    lcd.clear();          // clear display
    lcd.setCursor(0, 0);  // move cursor to   (0, 0)
    lcd.print("Calculate Weight");


    for (int i = 0; i < 10; i++) {
      delay(1000);
      //Serial.println(i);
      lcd.setCursor(8, 1);
      lcd.print(i);
      // delay(2000);
    }
    calculate();
    lcd.setCursor(1, 1);
    lcd.print("Weight : ");
    lcd.print(weight / 100);
    lcd.print("Kg")
    String rupees = String((weight / 100) * 40);
    String kg = String(weight / 100);
    api_url_1 = "http://db.tinkerspace.in/billingtrolly/insertandupdate.php?item=Rice&qty=1&kg=" + kg + "&rupees=" + rupees + "&user_id=trolly0003";
    upload_data();
    delay(1000);
    // lcd.print("Rice")
  } else if (content.substring(1) == "71 9A 78 24") {
    //Serial.println("oil");
    lcd.clear();          // clear display
    lcd.setCursor(1, 0);  // move cursor to   (0, 0)
    lcd.print("Item : Oil");
    lcd.setCursor(1, 1);
    lcd.print("MRP : 120/ltr");
    api_url_1 = "http://db.tinkerspace.in/billingtrolly/insertandupdate.php?item=Oil&qty=1&kg=0&rupees=120&user_id=trolly0003";
    upload_data();
    delay(1000);
  } else if (content.substring(1) == "30 5B 06 22") {
    //Serial.println("soap");
    lcd.clear();          // clear display
    lcd.setCursor(1, 0);  // move cursor to   (0, 0)
    lcd.print("Item : soap");
    lcd.setCursor(1, 1);
    lcd.print("MRP : 50/Qty");
    api_url_1 = "http://db.tinkerspace.in/billingtrolly/insertandupdate.php?item=Soap&qty=1&kg=0&rupees=50&user_id=trolly0003";
    upload_data();
    delay(1000);
  }

  else {
    //Serial.println(" Access Denied ");
    delay(3000);
  }
  delay(2000);
}


void upload_data() {
  //Serial.println("\n\nConnecting to DB");
  HTTPClient http;  //Declare an object of class HTTPClient
  api_url_1.replace("\n", "");
  api_url_1.replace("\r", "");
  api_url_1.replace(" ", "%20");
  http.begin(api_url_1);
  //Serial.println(api_url_1);
  int httpCode = http.GET();  //Send the request

  if (httpCode > 0) {
    String server_response_1 = http.getString();  //Get the request response payload
    //Serial.println(server_response_1);
  }
  http.end();  //Close connection
}

void setWeight() {
  // scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  //
  //Serial.println("Before setting up the scale:");
  //Serial.print("read: \t\t");
  //Serial.println(scale.read());  // print a raw reading from the ADC

  //Serial.print("read average: \t\t");
  //Serial.println(scale.read_average(20));  // print the average of 20 readings from the ADC

  //Serial.print("get value: \t\t");
  //Serial.println(scale.get_value(5));  // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  //Serial.print("get units: \t\t");
  //Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  // by the SCALE parameter (not set yet)

  scale.set_scale(-478.507);
  //scale.set_scale(-471.497);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();  // reset the scale to 0

  //Serial.println("After setting up the scale:");

  //Serial.print("read: \t\t");
  //Serial.println((scale.read() / 380));  // print a raw reading from the ADC

  //Serial.print("read average: \t\t");
  //Serial.println((scale.read_average(20) / 380));  // print the average of 20 readings from the ADC

  //Serial.print("get value: \t\t");
  //Serial.println(scale.get_value(5));  // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  //Serial.print("get units: \t\t");
  //Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight, divided
  // by the SCALE parameter set with set_scale
}
void calculate() {
  //Serial.print("one reading:\t");
  weight = (scale.get_units() / 380) * 100, 1;
  //Serial.print((scale.get_units() / 380) * 100, 1);
  //Serial.print("\t| average:\t");
  //Serial.println((scale.get_units(10) / 380) * 100, 5);

  scale.power_down();  // put the ADC in sleep mode
  delay(5000);
  scale.power_up();
}