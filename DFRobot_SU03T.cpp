/*!
 * @file DFRobot_SU03T.cpp
 * @brief Basic struct of DFRobot_SU03T class
 * @details The library communicates with SU03T voice module through serial(and I2C) port. It can read the entry recognized by the voice module and control the module to speak.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT license (MIT)
 * @author [fengli](li.feng@dfrobot.com) and [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-04-14
 * @https://github.com/DFRobot/DFRobot_SU03T
*/
 
#include "DFRobot_SU03T.h"


DFRobot_SU03T::DFRobot_SU03T()
{
}

bool DFRobot_SU03T::begin()
{
  return true;
}

uint16_t DFRobot_SU03T::readEntryID()
{
  return readID();
}

void DFRobot_SU03T::autoExitWakeup(bool mode)
{
  uint8_t *sendPack  = pack(SU03T_AUTO_EXIT_WAKEUP_ID, 6);

  uint8_t id = 0;
  if(mode) {
    sendPack[3] = 0;
  } else {
    sendPack[3] = 1;
  }

  writeData(sendPack,6);
  free(sendPack);
}

uint8_t DFRobot_SU03T::sendInfo(uint8_t id,char data){
  uint8_t *sendPack  = pack(id,6);
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.char_num = data;
  sendPack[3] = conver.bnum[0]  ;
  
  writeData(sendPack,6);
  free(sendPack);
  return 0;
}
uint8_t DFRobot_SU03T::sendInfo(uint8_t id,int data){
  uint8_t *sendPack  = pack(id,9);
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.int_num = data;
  
  unsigned long value = conver.data_long;
  unsigned long sign = (value >> 15) & 1;
  unsigned long v = value;
  if (sign)
    v = 0xFFFF0000 | value;
  uHexDouble_t p;
  p.data_long = v;
  sendPack[3] = p.bnum[0];
  sendPack[4] = p.bnum[1];
  sendPack[5] = p.bnum[2];
  sendPack[6] = p.bnum[3];
  writeData(sendPack,9);
  free(sendPack);
  return 0;
}
uint8_t DFRobot_SU03T::sendInfo(uint8_t id,double data){
  uint8_t *sendPack  = pack(id,13);
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.num = data;
  

  if(sizeof(double) == 8){
    for(uint8_t i = 0;i<8;i++){
       sendPack[3+i] = conver.bnum[i];
    }
  } else {
    unsigned long value = conver.data_long;
    unsigned long sign = value >> 31;
    unsigned long M = value & 0x007FFFFF;
    unsigned long e =  ((value >> 23 ) & 0xFF) - 127 + 1023;
    uHexDouble_t p0, p1;  
    p1.data_long = ((sign & 1) << 31) | ((e & 0x7FF) << 20) | (M >> 3);
    p0.data_long = (M & 0x7) << 29;
    sendPack[3] = p0.bnum[0];
    sendPack[4] = p0.bnum[1];
    sendPack[5] = p0.bnum[2];
    sendPack[6] = p0.bnum[3];
    sendPack[7] = p1.bnum[0];
    sendPack[8] = p1.bnum[1];
    sendPack[9] = p1.bnum[2];
    sendPack[10] = p1.bnum[3];
  
  }
  writeData(sendPack,13);
  free(sendPack);

  return 0;
}

uint8_t * DFRobot_SU03T::pack(uint8_t id ,uint8_t length)
{

   uint8_t * data = (uint8_t *)malloc(sizeof(uint8_t)*length);
   memset(data,0,sizeof(data));
   data[0] = 0xaa;
   data[1] = 0x55;
   data[length-1] = 0xaa;
   data[length-2] = 0x55;
   data[2] = id;
   
   return data;
}

uint8_t DFRobot_SU03T::sendInfo(uint8_t id,unsigned char data)
{
  uint8_t *sendPack  = pack(id,6);
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.uchar_num = data;
  sendPack[3] = conver.bnum[0]  ;
  
  writeData(sendPack,6);
  free(sendPack);
  return 0;

}

uint8_t DFRobot_SU03T::send(uint8_t id, const char *s,...)
{
    int i = 0;
    uint8_t head[3] = {0xAA,0x55,id};
    uint8_t uffix[2] = {0x55,0xAA};
    writeData(head,3);
    va_list va_ptr;
    va_start(va_ptr, s);
    while (s[i] != '\0')
    {
        if (s[i] != '%')
        {
               
               continue;
        }
        

        switch (s[++i])   
        {
            case 'd': sendDataInt(va_arg(va_ptr,int));           
                     break; 

            case 'c': sendDataChar(va_arg(va_ptr,int));
                      break;

            case 'f': sendDataDouble(va_arg(va_ptr,double));      
                      break;
            default : break;
        }
        
        i++; 
    }
    writeData(uffix,2);
    va_end(va_ptr);
    return 0;
}

void DFRobot_SU03T::sendDataInt(int data)
{
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.int_num = data;
  
  unsigned long value = conver.data_long;
  unsigned long sign = (value >> 15) & 1;
  unsigned long v = value;
  if (sign)
    v = 0xFFFF0000 | value;
  uHexDouble_t p;
  p.data_long = v;

  writeData(p.bnum,4);
}

void DFRobot_SU03T::sendDataUChar(int8_t data)
{
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.uchar_num = data;

  writeData(conver.bnum+7,1);
}

void DFRobot_SU03T::sendDataChar(uint8_t data)
{
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.char_num = data;
  DBG("char");
  DBG(conver.bnum[0]);
  DBG(conver.bnum[1]);
  DBG(conver.bnum[2]);
  DBG(conver.bnum[3]);
  DBG(conver.bnum[4]);
  DBG(conver.bnum[5]);
  DBG(conver.bnum[6]);
  DBG(conver.bnum[7]);
 
  DBG("-------------------");
  writeData(conver.bnum,1);
}

void DFRobot_SU03T::sendDataDouble(double data)
{
  uHexDouble_t conver ;
  memset(conver.bnum,0,8);
  conver.num = data;
  uint8_t sendPack[8];

  if(sizeof(double) == 8){
     writeData(conver.bnum,8);
  } else {
    unsigned long value = conver.data_long;
    unsigned long sign = value >> 31;
    unsigned long M = value & 0x007FFFFF;
    unsigned long e =  ((value >> 23 ) & 0xFF) - 127 + 1023;
    uHexDouble_t p0, p1;  
    p1.data_long = ((sign & 1) << 31) | ((e & 0x7FF) << 20) | (M >> 3);
    p0.data_long = (M & 0x7) << 29;
    sendPack[0] = p0.bnum[0];
    sendPack[1] = p0.bnum[1];
    sendPack[2] = p0.bnum[2];
    sendPack[3] = p0.bnum[3];
    sendPack[4] = p1.bnum[0];
    sendPack[5] = p1.bnum[1];
    sendPack[6] = p1.bnum[2];
    sendPack[7] = p1.bnum[3];
    
    writeData(sendPack,8);
  }
}

/***********************************************************
 * initialization, read and write of I2C interface 
 ***********************************************************/

DFRobot_SU03T_I2C::DFRobot_SU03T_I2C(uint8_t i2cAddr, TwoWire *pWire)
{
  _deviceAddr = i2cAddr;
  _pWire = pWire;
}

bool DFRobot_SU03T_I2C::begin(void)
{
  _pWire->begin();   // Wire.h（I2C）library function initialize wire library
  delay(50);

  return DFRobot_SU03T::begin();   // Use the initialization function of the parent class
}

void DFRobot_SU03T_I2C::writeData(uint8_t* data, uint8_t len)
{
  if(data == NULL){
    DBG("pBuf ERROR!! : null pointer");
  }
  uint8_t * _pBuf = (uint8_t *)data;

      _pWire->beginTransmission(_deviceAddr);
      _pWire->write(SU03T_SEND_INFO_REG);

      for(uint8_t i=0; i<len; i++){
        DBG(_pBuf[i], HEX);
        _pWire->write(_pBuf[i]);
        // delay(10);
      }

      _pWire->endTransmission();


  // uint8_t i = 0;
  // for(uint8_t j=1; j<6; j++){
  //   if(i<len) {
  //     _pWire->beginTransmission(_deviceAddr);
  //     _pWire->write(SU03T_SEND_INFO_REG);

  //     uint8_t count = j*3;
  //     if(count > len) count = len;
  //     for(i; i<count; i++){
  //       DBG(_pBuf[i], HEX);
  //       _pWire->write(_pBuf[i]);
  //       // delay(10);
  //     }

  //     _pWire->endTransmission();
  //     // DBG(i);
  //   }
  // }


  // free(_pBuf);
}

uint16_t DFRobot_SU03T_I2C::readID(void)
{
  uint16_t ID = 0xFFFF;

  _pWire->beginTransmission(_deviceAddr);
  _pWire -> write(SU03T_VOICE_ID_MSB_REG);
  if(0 != _pWire->endTransmission()){
    // Used Wire.endTransmission() to end a slave transmission started by beginTransmission() and arranged by write().
    DBG("endTransmission ERROR!!");
  }else{
    // Master device requests size bytes from slave device, which can be accepted by master device with read() or available()
    _pWire->requestFrom( _deviceAddr, (uint8_t)2 );

    while (_pWire->available()){
      ID = (ID<<8) | _pWire->read();   // Use read() to receive and put into buf
      // DBG(_pBuf[count-1], HEX);
    }
  }
  if(0xFFFF != ID)
    DBG(ID);
  delay(100);

  return ID;
}

/***********************************************************
 * initialization, read and write of UART interface 
 ***********************************************************/

DFRobot_SU03T_UART::DFRobot_SU03T_UART()
{
}

bool DFRobot_SU03T_UART::begin(Stream &s_)
{
  if(&s_ == NULL) {
    return false;
  }
  s = &s_;
  return DFRobot_SU03T::begin();   // Use the initialization function of the parent class
}

uint16_t DFRobot_SU03T_UART::readID(void)
{
  long time1 = millis();
  uint16_t ID = 0xFFFF;
  eDataState_t state = ePrefixH;
  while(1){
    uint8_t data = read();
    switch(state){
    case ePrefixH:
       if(data == 0xAA){
         state = ePrefixL;
       }
       break;
    case ePrefixL:
       if(data == 0x55){
         state = eDataH;
       } else {
         state = ePrefixH;
       }
       break;
    case eDataH:
       ID = data;
       state = eDataL;
       break;
    case eDataL :
       ID = (ID<<8) | data;
       state = eUffixH;
       break;
    case eUffixH:
       if(data == 0x55){
         state = eUffixL;
      } else {
        state = ePrefixH;
      }
       break;
    case eUffixL:
       if(data == 0xAA) {
         state = eSuccess;
       } else {
         state = ePrefixH;
       }
       break;
    default : break;
    }
    if(state == eSuccess) {
      break;
    }
    if((millis() - time1) > 1000) {
        DBG("time out !!!");
        break;
    }
  }
  return ID;
}

uint8_t DFRobot_SU03T_UART::read(void)
{
  long long current = millis();
  
  while(true) {
    if(s->available()) {
      uint8_t data = s->read();
      return data;
    }
    if((millis() - current) > 1000) break;
  }
  DBG("read error");
  return 0xFF;

}

void DFRobot_SU03T_UART::writeData(uint8_t * data, uint8_t len)
{
  for(uint8_t i = 0; i < len ;i++){
     s->write(data[i]);
     DBG(data[i],HEX);
  }
  // s->write((uint8_t *)data,len);
  // free(data);
}
