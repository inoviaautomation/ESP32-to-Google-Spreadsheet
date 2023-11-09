//ESP8266 with schneider meter EM6436
#define BLYNK_TEMPLATE_ID "TMPL3M_4k2HxS"
#define BLYNK_TEMPLATE_NAME "Inovia Automation"
#define BLYNK_AUTH_TOKEN "i8gRoo1xxeqv0AB22q1BLJpf4I_x-xKm"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <ModbusRTU.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <ESP8266HTTPClient.h>
// from Blynk.h
#ifndef BLYNK_TEMPLATE_ID
#error "Please specify id"
#endif
//Blynk.h end"
char auth[] = "i8gRoo1xxeqv0AB22q1BLJpf4I_x-xKm";
const char* ssid = "Archer C20"; //--> Your wifi name or SSID.
const char* pass = "Aaba1234"; //--> Your wifi password.
 
 
//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------
 
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
 
String GAS_ID = "AKfycbzr72yNDS6KW2qI1-JDilUr4lUR0VR-wReDiWR_1vvDuU2RYsFocn6ax9Z7TcuumMHX"; //--> spreadsheet script ID

ModbusRTU mb;
// https://script.google.com/macros/s/AKfycbzr72yNDS6KW2qI1-JDilUr4lUR0VR-wReDiWR_1vvDuU2RYsFocn6ax9Z7TcuumMHX/exec

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
  
}

// void initWiFi() {
//   WiFi.mode(WIFI_STA);
//   WiFi.begin(ssid, pass);
//   // Serial.print("Connecting to WiFi ..");
//   while (WiFi.status() != WL_CONNECTED) {
//     // Serial.print('.');
//     delay(3000);
//   }
//   // Serial.println(WiFi.localIP());
//   //The ESP8266 tries to reconnect automatically when the connection is lost
//   WiFi.setAutoReconnect(true);
//   WiFi.persistent(true);
// }

void setup() {
  Serial.begin(9600);
  S.begin(9600, SWSERIAL_8N1);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  mb.begin(&S,16); // RE/DE connected to D0 of ESP8266
  mb.master();
   client.setInsecure();
}

uint16_t val[2];

void loop() {
  //vtgL_N-140, vtgL_L-132, amp-148, Watt-100,freq-156 ,wh-158

  if (!mb.slave()) 
    {
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

    Blynk.virtualWrite(V0,voltage);
    Blynk.virtualWrite(V1,voltageL);
    Blynk.virtualWrite(V2,amp);
    Blynk.virtualWrite(V3,wt);
    Blynk.virtualWrite(V4,frq);
    Blynk.virtualWrite(V5,wh);
    //delay (1000);
//String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?voltage="+ String(voltage)+ "&voltageLL=" + String(voltageL)+ "&current=" + String(amp)+ "&watts=" + String(wt)+ "&frq=" + String(frq)+ "&wh=" + String(wh);
    // String urlFinal = "https://script.google.com/macros/s/"+GOOGLE_SCRIPT_ID+"/exec?voltage="+ String(voltage) "&voltageLL="+ String(voltageL) "&current="+ String(amp) "&watts="+ String(wt) "&frq="+ String(frq) "&wh="+ String(wh);
    //  Serial.print("POST data to spreadsheet:");

  Serial.println("Voltage= ");
  Serial.print(voltage);
  Serial.println("V");
  Serial.println("Wh= ");
  Serial.print(wh);
  Serial.println("Wh");
  Serial.println("frequency= ");
  Serial.print(frq);
  Serial.println("Hz");
  delay(10000);
  sendData(voltage, voltageL, amp, wt ,frq, wh);
  }
  }

  
// Subroutine for sending data to Google Sheets
void sendData(float v, float v2, float a, float wt, float fr, float kwh) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------
 
  //----------------------------------------Processing data and sending data
  String voltage =  String(v);
  String voltageLL =  String(v2);
  String current =  String(a);
  String watts =  String(wt);
  String frq =  String(fr);
  String wh =  String(kwh);
  
  String url = "/macros/s/" + GAS_ID + "/exec?voltage=" + voltage + "&voltageLL=" + voltageLL +"&current=" + current + "&watts=" + watts + "&frq=" + frq +"&wh=" + wh;
  Serial.print("requesting URL: ");
  Serial.println(url);
 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
 
  Serial.println("request sent");
  //----------------------------------------
 
  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
} 
