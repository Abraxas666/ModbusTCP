 // Arduino uno + ethernet shield W5100 connected

#include <SPI.h>
#include <Ethernet.h>
#include <modbusTCP.h>

#define CSLenght 13                                  // length of Coil Status Data array
#define ISLenght 13                                  // length of Input Status Data array
#define HRLenght 100                                 // length of Holding registers Data array
#define IRLenght 100                                 // length of Input registers Data array
#define MQLenght 100                                 // length of Master query Data array
#define SRLenght 260                                 // length of Slave reply Data array
#define FC Masterquery[7]                            // Define Function Code
#define MB_PORT 502

uint8_t Masterquery[MQLenght];                       // Master query array
uint8_t Slavereply[SRLenght];                        // Slave reply array
uint8_t ModbusRead;                                  // Used to concat the mudbus query bytes
uint8_t MessageLenght;                               // Used by the Slave Reply function

byte    CSData[CSLenght];                            // Coil Status Data array
byte    ISData[ISLenght];                            // Input Status Data array
word    HRData[HRLenght];                            // Holding registers Data array
word    IRData[IRLenght];                            // Input registers Data array

uint8_t *SRpoint;                                    // Slave reply array pointer
uint8_t *MQpoint;                                    // Master query array pointer
uint8_t *Lenght;                                     // Message Lenght pointer

byte    *CSpoint;                                    // Coil Status array pointer
byte    *ISpoint;                                    // Input Status array pointer
word    *HRpoint;                                    // Holding Registers array pointer
word    *IRpoint;                                    // Input Registers array pointer

byte mac[] = {0x90, 0xA2, 0xDA, 0x0E, 0x94, 0xB5 };  // Define network parameters
IPAddress ip(192, 168, 1, 15);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);                  // End of nework parameters definition

modbus mb;

void setup()
{
  Serial.begin(9600);                                // Initialize serial port
  Ethernet.begin(mac, ip, gateway,subnet);           // Initialize etherenet connection
  
  Lenght = &MessageLenght;                           // Inizialize all pointers
  MQpoint = Masterquery;
  SRpoint = Slavereply;

  CSpoint = CSData;
  ISpoint = ISData;
  HRpoint = HRData;
  IRpoint = IRData;                                  // End of pointers inizialization

  CSData[0] = 0xFF;                                  // Write the values of 8 coil status,address 1 to 8
  CSData[1] = 0x00;                                  // Write the values of 8 coil status,address 9 to 16
  CSData[2] = 0xAA;                                  // Write the values of 8 coil status,address 17 to 24
  
  ISData[0] = 0xAA;                                  // Write the values of 8 input status,address 10001 to 10008
  ISData[1] = 0xF0;                                  // Write the values of 8 input status,address 10009 to 10016
  ISData[2] = 0x0F;                                  // Write the values of 8 input status,address 10017 to 10024
 
//  for ( int x = 0; x <= 100;  x++ ){ HRData[x] = 0x8000; }  // Set all Holding Registers to -32768
  
  HRData[1]  = 0XAE41;                               // Write the value -20927 in the address 40002
  HRData[2]  = 0X0000;                               // Write the value      0 in the address 40003
  HRData[3]  = 0X8000;                               // Write the value -32768 in the address 40004
  HRData[4]  = 0X1000;                               // Write the value   4096 in the address 40005
  HRData[5]  = 0XAE41;                               // Write the value -20927 in the address 40006
  
  IRData[7]  = 0XAE41;                               // Write the value -20927 in the address 30008
  IRData[8]  = 0XFFFF;                               // Write the value     -1 in the address 30009
  IRData[10] = 0X8000;                               // Write the value -32768 in the address 30011
  IRData[11] = 0X1000;                               // Write the value   4096 in the address 30012

  Serial.println("Ready for the first query!");
}

void loop()
{
  HRData[0]  = random(0xFFFF);                       // Write a random value in between 32767 and -32768 in the address 40001
  HRData[40] = random(0xFFFF);                       // Write a random value in between 32767 and -32768 in the address 40041
  IRData[9] =  random(0xFFFF);                       // Write a random value in between 32767 and -32768 in the address 30010
  IRData[40] = random(0xFFFF);                       // Write a random value in between 32767 and -32768 in the address 30041
   
  //HRData[20] = analogRead (A2); NOT WORKING WITH THE THERNET SHIELD, W5100 TO BE VERIFIEDN

  EthernetServer ModbusServer(MB_PORT);              // Initialize modbus server
  EthernetClient client = ModbusServer.available();  // Wait for client query
    
  while(client.available()){Masterquery[ModbusRead] = client.read(); ModbusRead++;} // While query available store the master query in the array 
  ModbusRead = 0;
 
  if (FC == 1) {                                       // If function code is 1 ( read coils status )
    mb.DigitalRead (MQpoint,SRpoint,Lenght,CSpoint);   // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query

  if (FC == 2) {                                       // If function code is 2 ( read inputs status )
    mb.DigitalRead (MQpoint,SRpoint,Lenght,ISpoint);   // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query
    
  if (FC == 3) {                                       // If function code is 3 ( read holding registers )
    mb.AnalogRead (MQpoint,SRpoint,Lenght,HRpoint);    // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query

  if (FC == 4)  {                                      // If function code is 4 ( read inputs registers )
    mb.AnalogRead (MQpoint,SRpoint,Lenght,IRpoint);    // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query

  if (FC == 5)  {                                      // If function code is 5 ( write single coil )
    mb.DigitalWrite (MQpoint,SRpoint,Lenght,CSpoint);  // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query

  if (FC == 6) {                                       // If function code is 6 ( write single holding register )
    mb.AnalogWrite (MQpoint,SRpoint,Lenght,HRpoint);   // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query

  if (FC == 15)  {                                     // If function code is 15 ( write multiple coils )
    mb.MultiCoilsWrite (MQpoint,SRpoint,Lenght,CSpoint);    // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query

  if (FC == 16) {                                      // If function code is 16 ( write multiple holding registers )
    mb.MultiRegsWrite (MQpoint,SRpoint,Lenght,HRpoint);   // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query

  if (FC == 22) {                                      // If function code is 22 ( mask write register )
    mb.MaskWriteRegister (MQpoint,SRpoint,Lenght,HRpoint);   // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query
    
  if (FC == 23) {                                      // If function code is 23 ( read write multiple registers )
    mb.ReadWriteMultiReg (MQpoint,SRpoint,Lenght,HRpoint);   // Prepare the slave reply
    client.write(Slavereply,MessageLenght);}           // Reply to master query
      
delay(150);
}




