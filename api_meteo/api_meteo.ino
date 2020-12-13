//#include <LiquidCrystal_I2C.h>
//#include <Bridge.h>
//#include <ArduinoJson.h>
#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

//LiquidCrystal_I2C lcd(0x27, 20, 4);

const char kHostname[] = "arduino.cc";
const char kPath[] = "/";

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=London,uk&appid=";
const String key = "24fbe7c261f76fbfe30a2f466c740f5e";

bool alreadyConnected = false;

void setup() {
  Serial.begin(9600);
  //lcd.init();
  //lcd.backlight();
  Ethernet.begin(mac, ip);
}

void loop() {
  //lcd.setCursor (0,1);  // 1ère colonnes, 2ème ligne
  //lcd.print("Done ! ");

  int err = 0;
  
  EthernetClient c;
  HttpClient http(c);
  err = http.get(kHostname, kPath);
  Serial.println(err);
  delay(800);
}
