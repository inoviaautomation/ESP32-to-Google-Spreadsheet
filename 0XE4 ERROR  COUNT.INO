//ESP8266 with schneider meter EM6436
#include <ModbusRTU.h>
#include <SoftwareSerial.h>
float InttoFloat(uint16_t Data0,uint16_t Data1) {
  float x;
  unsigned long *p;
  p = (unsigned long*)&x;
  *p = (unsigned long)Data0 << 16 | Data1; //Big-endian
  return(x);
}
SoftwareSerial S(0, 4); //0FOR TX, 4for RX
ModbusRTU mb;
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data) { // Callback to monitor errors
  if (event != Modbus::EX_SUCCESS) {
    Serial.print("Request result: 0x");
    Serial.print(event, HEX);
  }
  return true;
}
void setup() {
  Serial.begin(9600);
  S.begin(9600, SWSERIAL_8N1);
  mb.begin(&S,16); // RE/DE connected to D0 of ESP8266
  mb.master();
}
uint16_t val[2];
void loop() {
  
  if (!mb.slave()) {
    mb.readHreg(1, 140, val, 2, cb); // Slave id is 1 and register address is 3926 and 
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(100);
    }
    float voltage= InttoFloat(val[1],val[0]);
    
    mb.readHreg(1, 156, val, 2, cb); // Slave id is 1 and register address is 3926 and 
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(100);
    }
    float frequency= InttoFloat(val[1],val[0]);
    
    mb.readHreg(1, 158, val, 2, cb); // Slave id is 1 and register address is 3926 and 
    //we are reading 2 bytes from the register and saving in val
    while(mb.slave()) { // Check if transaction is active
      mb.task();
      delay(100);
    }
    //Serial.println("Register Values ");
   // Serial.println(val[0]);
    //Serial.println(val[1]);
  float Wh= InttoFloat(val[1],val[0]);
  Serial.println("Voltage= ");
  Serial.print(voltage);
  Serial.println("V");
  Serial.println("Wh= ");
  Serial.print(Wh);
  Serial.println("Wh");
  Serial.println("frequency= ");
  Serial.print(frequency);
  Serial.println("Hz");
   }
      
  delay(1000);
  }
