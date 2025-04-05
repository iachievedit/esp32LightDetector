#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Ping.h>

#include <Adafruit_NeoPixel.h>

#include "UrlEncode.h"

#include "config.h"

#define PIN        12
HTTPClient http;
Adafruit_NeoPixel pixels(6, PIN, NEO_GRB + NEO_KHZ800);

uint32_t white  = pixels.Color(100, 100, 100);
uint32_t red    = pixels.Color(100, 0,   0);
uint32_t green  = pixels.Color(0,   100, 0);
uint32_t yellow = pixels.Color(100, 100, 0);

unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return(0);
  }
  time(&now);
  return now;
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(500);
    pixels.setPixelColor(0, white);
    pixels.show();
    delay(500);
    pixels.setPixelColor(0, 0);
    pixels.show();
  }
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  configTime(0, 0, ntpServer);

  pixels.begin();
  
  

}


void loop() {
  
  if (Ping.ping("192.168.2.5")) {
    Serial.println("Ping successful!");
    pixels.setPixelColor(0, green);
    pixels.show();
  } else {
    Serial.println("Ping failed!");
    pixels.setPixelColor(0, red);
    pixels.show();
  }

  unsigned long acquired_at = getTime();
  char timestamp[20];
  struct tm* timeinfo = localtime((time_t*)&acquired_at);
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%S", timeinfo);
  Serial.print("Timestamp: ");
  Serial.println(timestamp);

  long value = random(20000, 21001);  // upper bound is exclusive
  Serial.println(value);

  String query = "INSERT INTO readings (acquired_at, lux) VALUES ('" + String(timestamp) + "', "+ value +")";
  Serial.println(query);
  String urlEncodedQuery = String(questdbExec) + urlEncode(query);

  Serial.print("URL: ");
  Serial.println(urlEncodedQuery);
  http.begin(urlEncodedQuery);

  unsigned short httpStatus = http.GET();

  Serial.print("HTTP status: ");
  Serial.println(httpStatus);

  if (httpStatus == 200) {
    Serial.println("Success!");
    pixels.setPixelColor(0, green);
  } else {
    pixels.setPixelColor(0, red);
  }
  pixels.show();

  delay(1000);
}

