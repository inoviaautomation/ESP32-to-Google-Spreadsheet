#include <ModbusMaster.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial (3, 11); // RX, TX #define MODBUS_DATA_TRANSACTION_PIN 13
ModbusMaster node;
void preTransmission()
｛
digital Write (MODBUS_DATA_TRANSACTION_PIN, 1);
void postTransmission()
{
digital Write (MODBUS_DATA_TRANSACTION_PIN, O);
｝
void setup() {
pinMode (MODBUS_DATA_TRANSACTION_PIN,OUTPUT);
// Init in receive mode
digita|Write (MODBUS_DATA_TRANSACTION_PIN,O);
Serial.begin (9600);
// Modbus communication runs at 9600 baud
mySerial.begin (9600);
// Modbus slave ID 1
node.begin(1,mySerial);
/ Callbacks allow us to configure the RS485 transceiver correctly
node.preTransmission (preTransmission); node.post Transmission (postTransmission);
Serial.printIn( selec Tc id :");
}
void loop() {
node.read nputRegisters (21, 1);
Serial. print ("VOLT ");
float prosess_value = node.getResponseBuffer (0);
Serial.printIn(prosess_value/10.0); node.clearResponseBuffer;
delay (500);
node.readHoldingRegisters (23,1);
Serial.print ("AMP ");
Serial.printIn(node.getResponseBuffer (0)/10.0); node.clearResponseBuffer;
delay (500);
}