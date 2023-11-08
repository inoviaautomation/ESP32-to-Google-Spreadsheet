//ESP8266 with schneider meter EM6436
#define BLYNK_TEMPLATE_ID "TMPL3M_4k2HxS"
#define BLYNK_TEMPLATE_NAME "Inovia Automation"
#define BLYNK_AUTH_TOKEN "i8gRoo1xxeqv0AB22q1BLJpf4I_x-xKm"
//#include <Wire.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <ModbusRTU.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
#include "HTTPSRedirect.h"

// from Blynk.h
#ifndef BLYNK_TEMPLATE_ID
#error "Please specify id"
#endif
//Blynk.h end"
//#define BLYNK_FIRMWARE_VERSION        "0.1.0"
char auth[] = "i8gRoo1xxeqv0AB22q1BLJpf4I_x-xKm";
char ssid[] = "Archer C20";
char pass[] = "Aaba1234";
//const char* host = "script.google.com";
//const int httpsPort = 443;
const char *c_str();
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
ModbusRTU mb;
 
//String GAS_ID = "AKfycbwDby4s3sZapLTNSFK8fIYE5J_sDigAkMbeZ3LqDQLLZuHBHANtPMaChjPFCwn2e3BH"; //--> spreadsheet script ID
 //https://script.google.com/macros/s/AKfycbx4ybVgOoBH_I87Zs2rwQPdyuiPJ9TRrvPSjMSqDbxzWh8BbY3CnK3K5aFosxt3QDyZ/exec
String GOOGLE_SCRIPT_ID = "AKfycbx4ybVgOoBH_I87Zs2rwQPdyuiPJ9TRrvPSjMSqDbxzWh8BbY3CnK3K5aFosxt3QDyZ";    // change Gscript ID
 //AKfycbwDby4s3sZapLTNSFK8fIYE5J_sDigAkMbeZ3LqDQLLZuHBHANtPMaChjPFCwn2e3BH
 WiFiClientSecure s;
float InttoFloat(uint16_t Data0,uint16_t Data1) {
  float x;
  unsigned long *p;
  p = (unsigned long*)&x;
  *p = (unsigned long)Data0 << 16 | Data1; //Big-endian
  return(x);
}
SoftwareSerial S(0, 4); //0FOR TX, 4for RX  
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS) {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
  }
  return true;
  client.setInsecure();
}
void setup() {
  Serial.begin(9600);
  S.begin(9600, SWSERIAL_8N1);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  mb.begin(&S,16); // RE/DE connected to D0 of ESP8266
  mb.master();
}
uint16_t val[2];
void loop() {
  //vtgL_N-140, vtgL_L-132, amp-148, Watt-100,freq-156 ,wh-158
  if (!mb.slave()) {
    mb.readHreg(1, 140, val, 2, cb); // Slave id is 1 and register address is 40141 for Voltage line to neutral
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(10);
    }
    float voltage= InttoFloat(val[1],val[0]);
    
    mb.readHreg(1, 142, val, 2, cb); // Slave id is 1 and register address is 40133 for Voltage line to line
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(10);
    }
    float voltageL= InttoFloat(val[1],val[0]);
    
    mb.readHreg(1, 148, val, 2, cb); // Slave id is 1 and register address is 40149 for Avg Ampre
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(10);
    }
     float amp= InttoFloat(val[1],val[0]);
     mb.readHreg(1, 100, val, 2, cb); // Slave id is 1 and register address is 40100 for watts
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(10);
    }
     float wt= InttoFloat(val[1],val[0]);
     mb.readHreg(1, 156, val, 2, cb); // Slave id is 1 and register address is 40157 for frequency
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(10);
    }
     float frq= InttoFloat(val[1],val[0]);
     mb.readHreg(1, 158, val, 2, cb); // Slave id is 1 and register address is 401599 for Wh
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(10);
    }
     float wh= InttoFloat(val[1],val[0]);
    //Serial.println("Register Values ");
   // Serial.println(val[0]);
    //Serial.println(val[1]);
   float f = (voltage);
   String temp =String(f);
    Blynk.virtualWrite(V3,voltage);
    Blynk.virtualWrite(V1,voltageL);
    Blynk.virtualWrite(V2,amp);
    Blynk.virtualWrite(V3,wt);
    Blynk.virtualWrite(V4,frq);
    Blynk.virtualWrite(V5,wh);
    delay (1000);
  //String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"vtg="+ String(voltage)+ "&vtg1=" + String(voltageL)+ "&amp=" + String(amp)+ "&wt=" + String(wt)+ "&frq=" + String(frq)+ "&wh=" + String(wh);
   String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?"+"voltage="+ String(temp);
   //  Serial.print("POST data to spreadsheet:");
    Serial.println(urlFinal);
    HTTPClient http;
    {
      WiFiClient s;
      http.begin(s, urlFinal.c_str());
      }
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET(); 
    Serial.println(httpCode);
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        Serial.println("Payload: "+payload);  
        payload = http.getString();
    }
    //---------------------------------------------------------------------
    http.end();
  delay(1000);

  Serial.println("Voltage= ");
  Serial.print(voltage);
  Serial.println("V");
  Serial.println("Wh= ");
  Serial.print(wh);
  Serial.println("Wh");
  Serial.println("frequency= ");
  Serial.print(frq);
  Serial.println("Hz");
   }
      
  delay(1000);
  }
