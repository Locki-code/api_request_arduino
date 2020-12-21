#include <SPI.h>
#include <HttpClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 100);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  if (!Ethernet.begin(mac)) {
    Serial.println(F("Failed to configure Ethernet"));
    return;
  }
}

void loop() {
  //lcd.setCursor (0,1);  // 1ère colonnes, 2ème ligne
  //lcd.print("Done ! ");
  delay(10000);

  Serial.println(F("Connecting..."));
  // Connect to HTTP server
  EthernetClient client;
  client.setTimeout(1000);
  if (!client.connect("api.openweathermap.org", 80)) {
  //if (!client.connect("arduinojson.org", 80)) {
    Serial.println(F("Connection failed"));
    return;
  }
  Serial.println(F("Connected!"));

  // Send HTTP request
  client.println(F("GET /data/2.5/weather?q=London&appid=<key> HTTP/1.0"));
  client.println(F("Host: api.openweathermap.org"));
  //client.println(F("GET /example.json HTTP/1.0"));
  //client.println(F("Host: arduinojson.org"));
  client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }
  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Allocate JsonBuffer
  // Use arduinojson.org/assistant to compute the capacity.
  const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 21;
  DynamicJsonDocument doc(700);

  // Parse JSON object
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.println(F("DeserializeJson failed!"));
    Serial.println(error.c_str());
    return;
  }

  // Extract values

  //Serial.println(doc["LifetimeStatistics"][0][0].as<int>() + doc["LifetimeStatistics"][1][5].as<int>() + doc["LifetimeStatistics"][2][0].as<int>());

  // Extract values
  Serial.println(F("Response:"));
  const char* town = doc["name"].as<char*>();
  float temp = (doc["main"]["temp"].as<float>())-(273.15);
  Serial.println(town);
  Serial.println(temp);
  lcd.setCursor (0,0);
  lcd.print("Ville : "+String(town));
  lcd.setCursor (0,1);
  lcd.print("Temp : "+String(temp));

  // Disconnect
  client.stop();
  delay(2000);
}
