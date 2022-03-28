# coding=utf8
'''!
  @file DFRobot_SU03T.py
  @brief Basic struct of DFRobot_SU03T class
  @details The library communicates with SU03T voice module through serial port. It can read the entry recognized by the voice module and control the module to speak.
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT license (MIT)
  @author [fengli](li.feng@dfrobot.com)
  @version  V1.0
  @date  2022-03-09
  @https://github.com/DFRobot/DFRobot_SU03T
'''
import serial
import time
import array
import numpy as np
import struct
from sys import version_info

  
class DFRobot_SU03T(object):

  def __init__(self ,Baud):
    self.__Baud = Baud
    self.ser = serial.Serial("/dev/ttyAMA0" ,baudrate=Baud,stopbits=1, timeout=0.5)
    if self.ser.isOpen == False:
      print("isOpen")
      self.ser.open()

  def read_entry_ID(self ,timeout):
      '''!
        @fn read_entry_ID
        @brief Read the ID corresponding to the entry recognized by the voice recognition module
        @param timeout timeout detection
        @return  ID
      '''
      ID = 0xffff
      ticks = time.time()
      while True:
        if self.__read(1) == [0xAA]:
          self.__read(1)
          ID = self.__read(1)[0]
          
          ID = (ID<<8) + self.__read(1)[0]
      
          self.__read(1)
          self.__read(1)
          break
        if (time.time() - ticks) > 1:
          return 0xffee
      return ID

  def send(self,id,*args):
     '''！
       @fn send
       @brief Send multiple data, which can be in different data types
       @param id correspond to the entry
       @param *args represents changeable parameter, i.e. the answer requires multiple parameters
       @return 0 for success
     '''
     self.__write_cmd([0xAA,0x55,id])
     for i in args:
       if type(i) == float:
         self.__write_cmd(struct.pack('<d', i))
       elif type(i) == int:
         self.__write_cmd(struct.pack('<i', i))
       elif type(i) == str:
         value = struct.pack('<b',ord(i))
         self.__write_cmd([ord(i)])
       else:
         break
     self.__write_cmd([0x55,0xAA])
     return 0
  
  def __write_cmd(self,data):
    '''!
      @brief writes data to a register
      @param data written data
    '''
    self.ser.write(data)
    return

  def __read(self,len):
    '''!
      @brief read the data from the register
      @param len lenght of data
    '''
    #recv=[0x00,0x00,0x00]
    if version_info.major == 2:
      recv=[0x00,0x00,0x00]
      recv = self.ser.read(1)
      recv =[ord(c) for c in recv]
      return recv
    else :
      data =  self.ser.read(1)
      dd = int.from_bytes(data, byteorder='big')
      return [dd]
