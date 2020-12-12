//#include <LiquidCrystal_I2C.h>
//#include <Bridge.h>
//#include <ArduinoJson.h>
#include <HttpClient.h>
#include <SPI.h>
#include <Ethernet.h>

//LiquidCrystal_I2C lcd(0x27, 20, 4);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);
EthernetServer server(80); 

const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=London,uk&appid=";
const String key = "24fbe7c261f76fbfe30a2f466c740f5e";

bool alreadyConnected = false;

void setup() {
  Serial.begin(9600);
  //lcd.init();
  //lcd.backlight();
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  //lcd.setCursor (0,1);  // 1ère colonnes, 2ème ligne
  //lcd.print("Done ! ");

  EthernetClient client = server.available();
 
     // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }
    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      server.write(thisChar);
      // echo the bytes to the server as well:
      Serial.write(thisChar);
    }
  }

}

/*  
  HttpClient http;
  http.begin(endpoint + key);
  int httpCode = http.get(endpoint + key);

  if (httpCode > 0) { //Check for the returning code
   
          //String payload = http.get(endpoint + key);
          Serial.println(httpCode);
          //Serial.println(payload);
          
          lcd.setCursor (0,0);  // 1ère colonnes, 2ème ligne
          lcd.print(httpCode);
  }
   
  else {
        Serial.println("Error on HTTP request");
  }
  delay(800);*/
