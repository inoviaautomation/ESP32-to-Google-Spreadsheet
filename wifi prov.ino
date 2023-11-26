#define BLYNK_TEMPLATE_ID "TMPL3M_4k2HxS"
#define BLYNK_TEMPLATE_NAME "Inovia Automation"
#define BLYNKv_AUTH_TOKEN "i8gRoo1xxeqv0AB22q1BLJpf4I_x-xKm"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp8266.h>
#include <ModbusRTU.h>
#include <Modbus.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>

const char* softAP_ssid = "Gateway Wifi Setup";
const char* softAP_password = "admin";
const int provisioningTimeout = 120; // SoftAP provisioning timeout (in seconds)

// Blynk and WiFi credentials

char auth[] = "i8gRoo1xxeqv0AB22q1BLJpf4I_x-xKm";
const char* ssid = "Archer C20";
const char* pass = "Aaba1234";

// Define other variables, pins, and objects here
//----------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;

int myGlobalVariable; // Declare a global variable
// timer for wifi reconnect Function
unsigned long previousMillis = 0;
const long interval = 120000; // Interval set to 5 minutes (300,000 milliseconds)
// timer for indivisual Function
unsigned long loopTimer = 0;
unsigned long sendDataTimer = 0;
const unsigned long loopInterval = 50; // Time interval for the loop function (in milliseconds)
const unsigned long sendDataInterval = 30000; // Time interval for the sendData function (in milliseconds)

//----------------------------------------
WiFiClientSecure client; //--> Create a WiFiClientSecure object.
String GAS_ID = "AKfycbzr72yNDS6KW2qI1-JDilUr4lUR0VR-wReDiWR_1vvDuU2RYsFocn6ax9Z7TcuumMHX"; //--> spreadsheet script ID
// modbus declaration
ModbusRTU mb;

float InttoFloat(uint16_t Data0,uint16_t Data1) {
  float x;
  unsigned long *p;
  p = (unsigned long*)&x;
  *p = (unsigned long)Data0 << 16 | Data1; //Big-endian
  return(x);
}
// pin define

SoftwareSerial S(0, 4); //0FOR d2 TX, 4for d3 RX d0 fo RE DE  

bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS ) {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
    myGlobalVariable = event; //----------------------------------------global varible
    Serial.println("Wrong");
    
  }else{
     Serial.print(event,HEX);
    // Serial.println("Right");
    myGlobalVariable = event;
    }
  // if(event != 228){
  return true;
}

void setup() {
  Serial.begin(9600);
  // SoftAP setup
  WiFiManager wifiManager;
  wifiManager.setConfigPortalTimeout(provisioningTimeout); // Set provisioning timeout
  if (!wifiManager.autoConnect(softAP_ssid, softAP_password)) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
    delay(5000);
  }
  Serial.println("Connected to Wi-Fi");
  // Blynk and other setup
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  S.begin(9600, SWSERIAL_8N1); // modbus protocol
  // WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  // initWiFi();  // wifi reconnect
  mb.begin(&S,16); // RE/DE connected to D0 of ESP8266
  mb.master(); // modbus master
  client.setInsecure(); // http server
}
uint16_t val[2];// int to float
  // Initialize Modbus, initialize pins, or other setup procedures here

void loop() {
  // SoftAP loop - handles provisioning if necessary
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run(); // Ensure Blynk communication is ongoing if connected
    // Your existing loop logic
    unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi disconnected. Reconnecting...");
      WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi reconnected");
        // Add actions or functions to perform when reconnected
      }
    } else {
      Serial.println("WiFi still connected");
    }
  }
  Blynk.run();
  //vtgL_N-140, vtgL_L-132, amp-148, Watt-100,freq-156 ,wh-158   modbus address

  // unsigned long currentMillis = millis();//loop timer
  // Execute the loop code when the time interval is reached
  if (currentMillis - loopTimer >= loopInterval) {
  loopTimer = currentMillis;

  if (!mb.slave()) //&& myGlobalVariable != 228
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

if(myGlobalVariable != 228){
  Blynk.virtualWrite(V0,voltage);// blynk writing
  Blynk.virtualWrite(V1,voltageL);
  Blynk.virtualWrite(V2,amp);
  Blynk.virtualWrite(V3,wt);
  Blynk.virtualWrite(V4,frq);
  Blynk.virtualWrite(V5,wh);

  Serial.println("Voltage= ");// serial writing
  Serial.print(voltage);
  Serial.println("V");
  Serial.println("Wh= ");
  Serial.print(wh);
  Serial.println("Wh");
  Serial.println("frequency= ");
  Serial.print(frq);
  Serial.println("Hz");
}
   // send value to senddata function
  sendData(voltage, voltageL, amp, wt ,frq, wh);

  delay(150);
  }
  }
    // ...
  } else {
    // SoftAP provisioning if not connected
    WiFiManager wifiManager;
    wifiManager.setConfigPortalTimeout(provisioningTimeout); // Set provisioning timeout
    if (!wifiManager.autoConnect(softAP_ssid, softAP_password)) {
      Serial.println("Failed to connect and hit timeout");
      delay(3000);
      ESP.restart();
      delay(5000);
    }
    Serial.println("Connected to Wi-Fi");
    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80); // Re-initialize Blynk after provisioning
  }
  
}
// Subroutine for sending data to Google Sheets
void sendData
 (float v, float v2, float a, float wt, float fr, float kwh) {

  unsigned long currentMillis = millis();
  // Execute the sendData code when the time interval is reached
  if (currentMillis - sendDataTimer >= sendDataInterval) 
  {
    sendDataTimer = currentMillis;
  if(myGlobalVariable != 228){
  delay(50);

  Serial.println("==========");
  // Serial.print("connecting to ");
  // Serial.println(host);
  // Serial.println(myGlobalVariable);
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
  Serial.println(url);// final url print
 
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");
 
  Serial.println("request sent");// succesfully data sending

  //----------------------------------------Checking whether the data was sent successfully or not
  // while (client.connected()) {
  //   String line = client.readStringUntil('\n');
  //   if (line == "\r") {
  //     Serial.println("headers received");
  //     break;
  //   }
  // }
  // String line = client.readStringUntil('\n');
  // if (line.startsWith("{\"state\":\"success\"")) {
  //   Serial.println("esp8266/Arduino CI successfull!");
  // } else {
  //   Serial.println("esp8266/Arduino CI has failed");
  // }
  // Serial.print("reply was : ");
  // Serial.println(line);
  // Serial.println("closing connection");
  // Serial.println("==========");
  // Serial.println();
  //----------------------------------------
  }
}
} 
