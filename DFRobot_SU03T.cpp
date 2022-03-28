/*!
 * @file DFRobot_SU03T.cpp
 * @brief Basic struct of DFRobot_SU03T class
 * @details The library communicates with SU03T voice module through serial port. It can read the entry recognized by the voice module and control the module to speak.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT license (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-09
 * @https://github.com/DFRobot/DFRobot_SU03T
*/
 
#include "DFRobot_SU03T.h"


DFRobot_SU03T::DFRobot_SU03T(){
}
bool DFRobot_SU03T::begin(Stream &s_){
  if(&s_ == NULL){
    return false;
  }
  s = &s_;
  return true;
}
uint16_t DFRobot_SU03T::readEntryID(int timeout){
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
       if(data == 0xAA){
         state = eSuccess;
       } else {
         state = ePrefixH;
       }
       break;
    default : break;
    }
     if(state == eSuccess) break;
     if((millis() - time1) > timeout) {
         DBG("time out !!!");
         return 0xffee;
     }
  }
  return ID;
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

uint8_t DFRobot_SU03T::send(uint8_t id,char *s,...){


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
void DFRobot_SU03T::sendDataChar(uint8_t data){
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
void DFRobot_SU03T::sendDataDouble(double data){

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

uint8_t DFRobot_SU03T::writeData(uint8_t * data,uint8_t len){
  for(uint8_t i = 0; i < len ;i++){
     s->write(data[i]);
     DBG(data[i],HEX);
     //Serial.println(data[i],HEX);
  }
  //s->write((uint8_t *)data,len);
  return 0;
}
uint8_t DFRobot_SU03T::read(){
   long long current = millis();
   
   while(true){
     
     if(s->available()){
       uint8_t data = s->read();
       //Serial.println(data);
       return data;
     }
     if((millis() - current) > 1000) break;
   }
   DBG("read error");
   return 0xFF;



}
