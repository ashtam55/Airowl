

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include<SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <SDS011.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

#define Serial Serial

#include <Adafruit_NeoPixel.h>

#define PIN 0

#define NUM_LEDS 5

int i;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

 float datap;


const int sensorPin = A0;
float P2, P1;
float p10, p25;
int error;

SDS011 my_sds;



void setup() {

  pinMode(sensorPin, INPUT);


  Serial.begin(115200);
  my_sds.begin(5,4);
  Serial.println("STARTING..");

  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();




  strip.setBrightness(80);
  strip.begin();
  strip.show();
  strip.setPixelColor(0, 0, 0, 255);
  strip.setPixelColor(1, 0, 0, 255);
  strip.show();
  strip.setPixelColor(2, 0, 0, 255);
  strip.setPixelColor(3, 0, 0, 255);
  strip.setPixelColor(4, 0, 0, 255);
  strip.show();
  WIFI();

}

void loop() {

  data();



  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  http.begin("http://139.59.64.163:1880/api?pm25=" + String(P2) + "&pm10=" + String(P1) + "&gas=" + String(datap) + "\""); //HTTP

  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);


  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();


  delay(1000);


}

void data() {
   datap = analogRead(A0);
  Serial.print("datap=");
  Serial.print(datap);

  delay(500);

  error = my_sds.read(&p25, &p10);
  if (! error) {
    Serial.println("P2.5: " + String(p25));
    Serial.println("P10:  " + String(p10));
  }

  P2 = p25;
  P1 = p10;

  float p3 = p10 + p25;
  if (p3 >= 120 || datap >= 360 ) {

    strip.setPixelColor(0, 255, 0, 0);
    strip.setPixelColor(1, 255, 0, 0);
    strip.show();
    strip.setPixelColor(2, 255, 0, 0);
    strip.setPixelColor(3, 255, 0, 0);
    strip.setPixelColor(4, 255, 0, 0);
    strip.show();

  }
  else {

    strip.setPixelColor(0, 0, 255, 0);
    strip.setPixelColor(1, 0, 255, 0);
    strip.show();
    strip.setPixelColor(2, 0, 255, 0);
    strip.setPixelColor(3, 0, 255, 0);
    strip.setPixelColor(4, 0, 255, 0);
    strip.show();
  }
  Serial.println("P1:");
  Serial.println(p10);

  Serial.println("P2:");
  Serial.println(p25);
  //gsmGetdata(p1, p2, gas);
}

void WIFI()
{
  WiFiManager wifiManager;
  delay(500);
  if ( wifiManager.autoConnect("NodeMCU-4")) {
    Serial.println("connected...yeey :)");
    WiFi.printDiag(Serial);

  }

  else {
    wifiManager.startConfigPortal("TweetCafe-4");

    if (!wifiManager.startConfigPortal("OnDemandAP")) {
      Serial.println("failed to connect and hit timeout");
      delay(3000);
      ESP.reset();
      delay(5000);
    }

    else {
      Serial.println("connected...yeey :");
      WiFi.printDiag(Serial);

    }
  }
}




