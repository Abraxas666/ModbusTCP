#ifndef modbus_H
#define modbus_H

#include "Arduino.h"

void  PrintData (uint8_t *SlvReply,uint8_t *MsgLenght);

class modbus
{
   public:

      modbus (); 

      void	DigitalRead        (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, byte *Data);
      void  AnalogRead         (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data);
      void  DigitalWrite  	    (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, byte *Data);
      void  AnalogWrite        (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data);
      void  MultiCoilsWrite    (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, byte *Data);
      void  MultiRegsWrite     (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data);
      void  MaskWriteRegister  (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data);
      void  ReadWriteMultiReg  (uint8_t *Mstrquery,uint8_t *SlvReply,uint8_t *MsgLenght, word *Data);
  
   private:

      uint8_t WordDataLenght;
      uint8_t ByteDataLenght;
      uint8_t count;
      uint8_t FC;

      byte    Buffer[100];
      word    Start;
      word    DataByte;
      word    OrMask;
      word    AndMask;

};


#endif