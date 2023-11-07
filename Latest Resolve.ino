#include <ModbusRTU.h>
//Definitions/
#define BAUD_RATE 9600
#define RS485_MODE 15
#define SLAVE_ID 1
//Instances/
ModbusRTU mb;
//SoftwareSerial mySerial(RS485_RX, RS485_TX);
//Instances END/
//Variables START/
uint16_t myWord;
//Variables END/
//Modbus Call back function for ERROR/
bool cb(Modbus::ResultCode event, uint16_t transactionId, void* data)
{
Serial.printf_P("Request result: 0x%02X, Mem: %d\n", event, ESP.getFreeHeap());
return true;
}
//Void Setup START/
void setup()
{
Serial.begin(BAUD_RATE);
Serial2.begin(BAUD_RATE, SERIAL_8E1);
Serial.println();
Serial.println("START");
Serial.println();
mb.begin(&Serial2, RS485_MODE);
mb.master();
}
//Void Setup END/
//Void Loop START/
void loop()
{
mb.readHreg(SLAVE_ID , 7, &myWord, 1, cb);//Address 40007 for displaying SLAVE ID (check the energy meter datasheet above)
Serial.print("DATA: ");
Serial.println(myWord);
mb.task();
//yield();
delay(500);
}
//Void Loop END/`