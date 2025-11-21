#include "modbusTCP.h"

modbus::modbus () {}

void modbus::DigitalRead(uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, byte *Data)
{
	  for ( count = 0; count < 5; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}   	 // Prepare the first 5 bytes of the slave reply             
    for ( count = 0; count < 2; count++){*Mstrquery = 0; Mstrquery++;} 
   	FC = *Mstrquery;
    for ( count = 0; count < 6; count++){Buffer[count] = *Mstrquery; *Mstrquery = 0; Mstrquery++;}				// Extrapolate relevant data from the modbus query

    Start = word(Buffer[3],Buffer[2]);															// Calculate the offset from zero (First byte of data to read)
    WordDataLenght = word(Buffer[5],Buffer[4])-1;												// Number of inputs or coils to read
    ByteDataLenght = (WordDataLenght/8)+1;														// Number of bytes needed to store the inputs or coils data, one byte is 8 digital status
    *SlvReply = ByteDataLenght + 3;																// Number of total bytes transmitted after this one
    SlvReply++; 
    *SlvReply = 1;																				// Set the slave address
    SlvReply++; 
    *SlvReply = FC;																				// Set the function code 
    SlvReply++; 
    *SlvReply = ByteDataLenght;																	// Number of total bytes transmitted after this one, containing inputs or coils data

    for( count = 0; count < Start; count++) {Data++;}											// Shift the data array start position acording to the offset requested by the query
    for( count = 0; count < ByteDataLenght; count++){SlvReply++; *SlvReply = *Data; Data++;}	// Copyt the inputs or coils data into the slave reply array                                    
      
    *MsgLenght = ByteDataLenght + 9 ;															// Lenght of the slave reply ( Total number of bytes )                                                        

    PrintData (SlvReply,MsgLenght);
}

void modbus::AnalogRead (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data)
{
	  for ( count = 0; count < 5; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}    	  // Prepare the first 5 bytes of the slave reply                                        
    for ( count = 0; count < 2; count++){*Mstrquery = 0; Mstrquery++;} 
   	FC = *Mstrquery;
    for ( count = 0; count < 6; count++){ Buffer[count] = *Mstrquery; *Mstrquery = 0; Mstrquery++;}				// Extrapolate relevant data from the modbus query
    
    Start = word(Buffer[3],Buffer[2]);															// Calculate the offset from zero (First byte of data to read)
    WordDataLenght = word(Buffer[5],Buffer[4]);													// Number of holding or input registers to read
    ByteDataLenght = WordDataLenght * 2;														// Number of bytes needed to store the holding or input registers, one byte is 8 digital status

    *SlvReply = ByteDataLenght + 3;																// Number of total bytes transmitted after this one, containing inputs or holding registers data
    SlvReply++; 
    *SlvReply = 1;																				// Set the slave address
    SlvReply++; 
    *SlvReply = FC;																				// Set the function code                                              			
    SlvReply++; 
    *SlvReply = ByteDataLenght;																	// Number of total bytes transmitted after this one, containing inputs or hoding registers data

    for( count = 0; count < Start; count++) {Data++;}											// Shift the data array start position acording to the offset requested by the query
    for( count = 0; count < WordDataLenght; count++){											// Copyt the inputs or holding registers data into the slave reply array   
      SlvReply++;
      *SlvReply = highByte(*Data);																// Add the highbyte of the input or holdning register data in the slave reply array
      SlvReply++;
      *SlvReply = lowByte(*Data);																// Add the lowbyte of the input or holdning register data in the slave reply array
      Data++; }

    *MsgLenght = ByteDataLenght + 9 ;															// Lenght of the slave reply ( Total number of bytes )                                                			                                                        

    PrintData (SlvReply,MsgLenght);
}

void modbus::DigitalWrite (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, byte *Data)
{
  for ( count = 0; count < 8; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}
  for ( count = 0; count < 2; count++){*SlvReply = *Mstrquery;Buffer[count] = *SlvReply;*Mstrquery = 0; Mstrquery++; SlvReply++;}
  Start= word(Buffer[0],Buffer[1]);
  for ( count = 0; count < Start; count++){Data++;}
  *Data = *Mstrquery ;
  *SlvReply = *Mstrquery;
  SlvReply++;
  *SlvReply = 0;
  
  *MsgLenght = 12;

  PrintData (SlvReply,MsgLenght);
}

void modbus::AnalogWrite (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data)
{
  for ( count = 0; count < 8; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}
  for ( count = 0; count < 4; count++){*SlvReply = *Mstrquery; Buffer[count] = *SlvReply;*Mstrquery = 0; Mstrquery++; SlvReply++;}
  
  Start = word(Buffer[0],Buffer[1]);
  DataByte = word(Buffer[2],Buffer[3]);

  for ( count = 0; count < Start; count++){Data++;}
  *Data = DataByte;
 
  *MsgLenght = 12;

  SlvReply--; 

  PrintData (SlvReply,MsgLenght);
}

void modbus::MultiCoilsWrite (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, byte *Data)
{
  for ( count = 0; count < 5; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}                              
  
  *MsgLenght= *Mstrquery + 6;
                              
  for ( count = 0; count < 8; count++){*SlvReply = *Mstrquery; Buffer[count] = *SlvReply; *Mstrquery = 0; Mstrquery++; SlvReply++;}      

  Start = word(Buffer [5],Buffer [4]);
  ByteDataLenght = Buffer [7];

  for ( count = 0; count < Start; count++){Data++;}
  for ( count = 0; count < ByteDataLenght; count++){*SlvReply = *Mstrquery; *Data = *SlvReply; *Mstrquery = 0; Mstrquery++; SlvReply++; Data++;}
  
  SlvReply--; 

  PrintData (SlvReply,MsgLenght);  
}


void modbus::MultiRegsWrite (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data)
{

  for ( count = 0; count < 5; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}
  *MsgLenght= *Mstrquery + 6;
  for ( count = 0; count < 8; count++){*SlvReply = *Mstrquery; Buffer[count] = *SlvReply; *Mstrquery = 0; Mstrquery++; SlvReply++;}      

  Start = word(Buffer [3],Buffer [4]);
  ByteDataLenght = Buffer [6]*2;

  for ( count = 0; count < Start; count++){Data++;}
  
  for ( count = 0; count < ByteDataLenght; count++){*SlvReply = *Mstrquery; Buffer[count] = *SlvReply; *Mstrquery = 0; Mstrquery++; SlvReply++;}
  for ( count = 0; count < ByteDataLenght; count++){ DataByte = word (Buffer[count],Buffer[count+1]);  *Data = DataByte; Data++; count++;} 
  for ( count = 0; count < 5; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;} 
  for ( count = 0; count < 6; count++){SlvReply--;}                              

  PrintData (SlvReply,MsgLenght);
}

void  modbus::MaskWriteRegister  (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data)
{
  for ( count = 0; count < 5; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}

  *MsgLenght= *Mstrquery + 6;

  for ( count = 0; count < 9; count++){*SlvReply = *Mstrquery; Buffer[count] = *SlvReply; *Mstrquery = 0; Mstrquery++; SlvReply++;} 

  Start   = word(Buffer [3],Buffer [4]);
  AndMask = word(Buffer [5],Buffer [6]);
  OrMask  = word(Buffer [7],Buffer [8]);

  for ( count = 0; count < Start; count++){Data++;}

  *Data = (*Data | OrMask ) & AndMask;

  SlvReply--; 

  PrintData (SlvReply,MsgLenght);
}


void  modbus::ReadWriteMultiReg (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data)
{
  for ( count = 0; count < 8; count++){*SlvReply = *Mstrquery; *Mstrquery = 0; Mstrquery++; SlvReply++;}
  for ( count = 0; count < 9; count++){Buffer[count] = *Mstrquery; *Mstrquery = 0; Mstrquery++;}

  Start = word(Buffer [0],Buffer [1]);
  ByteDataLenght = word(Buffer [2],Buffer [3]);
  WordDataLenght = ByteDataLenght * 2;
  *MsgLenght = WordDataLenght + 9;

  for( count = 0; count < Start; count++) {Data++;}                     // Shift the data array start position acording to the offset requested by the query
  *SlvReply =  WordDataLenght;
  SlvReply++;
  for( count = 0; count < WordDataLenght; count++){                     // Copyt the inputs or holding registers data into the slave reply array   
      *SlvReply = highByte(*Data);                                // Add the highbyte of the input or holdning register data in the slave reply array
      SlvReply++;
      *SlvReply = lowByte(*Data);                               // Add the lowbyte of the input or holdning register data in the slave reply array
      SlvReply++;
      Data++; }
 
  for( count = 0; count < Start; count++) {Data--;}
  for( count = 0; count < WordDataLenght; count++) {Data--;}

  Start = word(Buffer [4],Buffer [5]);
  ByteDataLenght = Buffer [8];

  for ( count = 0; count < Start; count++){Data++;}
  for ( count = 0; count < ByteDataLenght; count++){ Buffer[count] = *Mstrquery; *Mstrquery = 0; Mstrquery++; }
  for ( count = 0; count < ByteDataLenght; count++){ DataByte = word (Buffer[count],Buffer[count+1]);  *Data = DataByte; Data++; count++;} 
  for ( count = 0; count < WordDataLenght + 1; count++){SlvReply--;}
  
  PrintData (SlvReply,MsgLenght);
}
//////////////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////////////////////////////

void PrintData (uint8_t *SlvReply,uint8_t *MsgLenght)
{

  for( int count = 1; count < *MsgLenght; count++) {SlvReply--;}  // Inizialize the slave reply pointer
    
  Serial.print("Slave Reply = ");                                 // Print the slave reply data
  for( int count = 0; count < *MsgLenght ; count++){  
    Serial.print ((*SlvReply) + String (" "));
    SlvReply++;}
  Serial.println ("");
  Serial.print   ("MessageLenght = ");
  Serial.println (*MsgLenght);
  Serial.println ("Ready for the net query!");
  Serial.println ("");

}