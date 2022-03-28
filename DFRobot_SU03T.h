/*!
 * @file DFRobot_SU03T.h
 * @brief Basic struct of DFRobot_SU03T class
 * @details The library communicates with SU03T voice module through serial port. It can read the entry recognized by the voice module and control the module to speak.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT license (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-09
 * @https://github.com/DFRobot/DFRobot_SU03T
*/
 
#ifndef DFRobot_SU03T_H
#define DFRobot_SU03T_H
#include "Arduino.h"
#include <Wire.h>
#include <stdarg.h>

//Open this macro to see the detailed runn...ing process of the program 

//#define ENABLE_DBG
#ifdef ENABLE_DBG
#define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif


class DFRobot_SU03T
{
public:
/**
 * @union uHexDouble_t
 * @brief Convert data of different types into arrays of char types to adapt to the protocol format for packaging and sending.
 */
typedef union
{   
    int    int_num;   /**<int data*/
    char   char_num;   /**<char data*/
    unsigned long data_long;/**<unsigned long data*/
    unsigned char uchar_num ;/**<unsigned char data*/
    double num;             /**<double data*/
    unsigned char bnum[8];/**<unsigned char data*/
}uHexDouble_t;

/**
 * @enum eDataState_t
 * @brief Data receiving status
 */
typedef enum{
 ePrefixH,   /**<Receive the high bit of start frame*/
 ePrefixL,   /**<Receive the low bit of start frame*/
 eDataH,     /**<Receive the high bit of data frame*/
 eDataL,     /**<Receive the low bit of data frame*/
 eUffixH,    /**<Receive the high bit of end frame*/
 eUffixL,    /**<Receive the low bit of end frame*/
 eSuccess,   /**<Received data packet successfully*/
} eDataState_t;

public:

  /**
   * @fn DFRobot_SU03T
   * @brief Constructor
   * @return None
   */
  DFRobot_SU03T();
  
  /**
   * @fn begin
   * @brief Init communication port
   * @param Software serial or hardware serial 
   * @return true or false
   */
  bool begin(Stream &s_);

  /**
   * @fn readEntryID
   * @brief Read the ID corresponding to the entry recognized by the voice recognition module
   * @param timeout Timeout detection
   * @return  ID
   */
  uint16_t readEntryID(int timeout);
  
  /**
   * @fn sendInfo
   * @brief Send single data of char type
   * @param id correspond to the entry
   * @param data Parameters required for the answer to the question of corresponding entry
   * @return 0 for success
   */
  uint8_t sendInfo(uint8_t id,char data);

  /**
   * @fn sendInfo
   * @brief Send single data of int type
   * @param id correspond to the entry
   * @param data Parameters required for the answer to the question of corresponding entry
   * @return 0 for success
   */
  uint8_t sendInfo(uint8_t id,int data);

  /**
   * @fn sendInfo
   * @brief Send single data of double type
   * @param id correspond to the entry
   * @param data Parameters required for the answer to the question of corresponding entry
   * @return 0 for success
   */
  uint8_t sendInfo(uint8_t id,double data);

  /**
   * @fn sendInfo
   * @brief Send single data of unsigned char type
   * @param id correspond to the entry
   * @param data Parameters required for the answer to the question of corresponding entry
   * @return 0 for success
   */
  uint8_t sendInfo(uint8_t id,unsigned char data);

  /**
   * @fn send
   * @brief Send multiple data, which can be in different data types
   * @param id correspond to the entry
   * @param s The data types of different data
   * @n (...) represents changeable parameter, i.e. the answer requires multiple parameters
   * @return 0 for success
   */
  uint8_t send(uint8_t id,char *s,...);
  
private:
  void  sendDataInt(int data);
  void  sendDataUChar(int8_t data);
  void  sendDataChar(uint8_t data);
  void  sendDataDouble(double data);
  uint8_t writeData(uint8_t * data,uint8_t len);
  uint8_t read();
  uint8_t * pack(uint8_t id ,uint8_t length) ;
  Stream *s;
};
#endif
