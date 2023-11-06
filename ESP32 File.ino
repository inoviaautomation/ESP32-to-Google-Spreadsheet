#define BLYNK_TEMPLATE_ID "TMPL3M_4k2HxS"
#define BLYNK_TEMPLATE_NAME "ESP32"
#define BLYNK_AUTH_TOKEN "cfiydRFWusDq4UZ8a5q0sMRZEYZNxiRP"
#include <Wire.h>
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <Blynk.h>
#include <BlynkSimpleEsp32.h>
#define echoPin 2               // CHANGE PIN NUMBER HERE IF YOU WANT TO USE A DIFFERENT PIN
#define trigPin 14               // CHANGE PIN NUMBER HERE IF YOU WANT TO USE A DIFFERENT PIN
long duration, distance, value1, value2;
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HTTPClient.h>
// from Blynk.h
#ifndef BLYNK_TEMPLATE_ID
#error "Please specify id"
#endif
//Blynk.h end"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
char auth[] = "cfiydRFWusDq4UZ8a5q0sMRZEYZNxiRP";
char ssid[] = "Archer C20";
char pass[] = "Aaba1234";
//https://script.google.com/macros/s/AKfycbwhqAa8JgOXtc5Ad7G78_D9q9pp9JgfTOz3ROvc-C1D1GWipvkTyu5wfPU-p9Dy9tPv/exec
//https://script.google.com/macros/s/AKfycbw9UwW2rsvuzE6Go5IXCbUs-pqy4VR_1Zd9bQQH7Oc/dev
String GOOGLE_SCRIPT_ID = "AKfycbwhqAa8JgOXtc5Ad7G78_D9q9pp9JgfTOz3ROvc-C1D1GWipvkTyu5wfPU-p9Dy9tPv";    // change Gscript ID
 
//ROOT CERTIFICATION
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
void setup(){
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Wire.begin(5, 4);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }

  // Clear the buffer
  display.clearDisplay();
  display.display();
}

void loop(){

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 58.2;
  value1 = duration / 68.2;
  value2 = duration / 48.2;
  String disp = String(distance);

  Serial.print("Distance" );
  Serial.print(disp);
  Serial.print(value1);
  Serial.print(value2);
  Serial.println("cm");
  delay(500);


  float mainTemp =(distance);
  float decimalTemp = (11);
  float temp = (distance);
  displayTemp(temp);
  delay(500);
  Blynk.virtualWrite(V0,disp);

  String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"voltage="+ String(temp)+ "&current=" + String(value1)+ "&power=" + String(value2)+ "&units=" + String(temp);
   //  Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    http.begin(urlFinal.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    //Serial.print("HTTP Status Code: ");
    //Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);  
    }
    //---------------------------------------------------------------------
    http.end();
  delay(1000);
}
void displayTemp(float temp) {
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(13,0);
  display.println(F("DISTANCE"));
  display.println(F("---------------------"));
  display.setTextSize(1);
  display.setCursor (10,57);
  display.setTextColor(SSD1306_WHITE);
  display.println(F("INOVIA AUTOMATION"));
  display.setCursor(28,30);
  display.setTextSize(3);
  display.setTextColor(SSD1306_WHITE);
  display.print(temp,1);
  display.display();
  delay(500);
}
