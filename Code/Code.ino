//Prateek
//wwww.justdoelectronics.com
//https://www.youtube.com/c/JustDoElectronics/videos

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

String apiKey = "3GXEV3B944KWTGQU";
const char* ssid = "justDo";
const char* password = "pratik123";
const char* server = "api.thingspeak.com";

WiFiClient client;


void setup() {
  Serial.begin(115200);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Connecting to ");
  display.setTextSize(2);
  display.print(ssid);
  display.display();

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("WiFi connected");
  display.display();
  delay(4000);
}


void loop() {
  MQ135 gasSensor = MQ135(A0);
  float air_quality = gasSensor.getPPM();
  Serial.print("Air Quality: ");
  Serial.print(air_quality);
  Serial.println("  PPM");
  Serial.println();

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println("Air Quality Index");


  display.setCursor(0, 20);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print(air_quality);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(" PPM");
  display.display();


  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(air_quality);
    postStr += "r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Data Send to Thingspeak");
  }
  client.stop();
  Serial.println("Waiting...");

  delay(2000);
}
