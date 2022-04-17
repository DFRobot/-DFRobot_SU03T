# coding=utf8
'''!
  @file DFRobot_SU03T.py
  @brief Basic struct of DFRobot_SU03T class
  @details The library communicates with SU03T voice module through serial port. It can read the entry recognized by the voice module and control the module to speak.
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license   The MIT license (MIT)
  @author [fengli](li.feng@dfrobot.com)
  @version  V1.0
  @date  2022-03-09
  @https://github.com/DFRobot/DFRobot_SU03T
'''
import serial
import smbus
import time
import array
import numpy as np
import struct
from sys import version_info

import logging
from ctypes import *

logger = logging.getLogger()
#logger.setLevel(logging.INFO)  #Display all print information
logger.setLevel(logging.FATAL)  #If you don’t want to display too many prints, only print errors, please use this option
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter) 
logger.addHandler(ph)

class DFRobot_SU03T(object):

  def __init__(self):
    pass

  def read_entry_ID(self):
    '''!
      @fn read_entry_ID
      @brief Read the ID corresponding to the entry recognized by the voice recognition module
      @return  ID
    '''
    return self._read_id()

  def send(self,id,*args):
    '''!
      @fn send
      @brief Send multiple data, which can be in different data types
      @param id correspond to the entry
      @param *args represents changeable parameter, i.e. the answer requires multiple parameters
      @return 0 for success
    '''
    self._write_cmd([0xAA,0x55,id])
    for i in args:
      if type(i) == float:
        self._write_cmd(struct.pack('<d', i))
      elif type(i) == int:
        self._write_cmd(struct.pack('<i', i))
      elif type(i) == str:
        value = struct.pack('<b',ord(i))
        self._write_cmd([ord(i)])
      else:
        break
    self._write_cmd([0x55,0xAA])
    return 0
  
  def _write_cmd(self, data):
    '''!
      @brief writes data to a register
      @param data written data
    '''
    raise NotImplementedError()

  def _read_id(self):
    '''!
      @brief read the data from the register
    '''
    raise NotImplementedError()

class DFRobot_SU03T_I2C(DFRobot_SU03T):
  '''!
    @brief define DFRobot_SU03T_I2C class
    @details to use I2C drive SU03T 
  '''
  SU03T_DEFAULT_I2C_ADDR    = 0x34
  SU03T_MODULE_ID_REG       = 0x00
  SU03T_MODULE_I2C_ADDR_REG = 0x01
  SU03T_VOICE_ID_MSB_REG    = 0x05
  SU03T_SEND_INFO_REG       = 0x09

  def __init__(self, i2c_addr=SU03T_DEFAULT_I2C_ADDR, bus=1):
    '''!
      @brief Module I2C communication init
      @param i2c_addr I2C communication address
      @param bus I2C communication bus
    '''
    self._i2c_addr = i2c_addr
    self._i2c = smbus.SMBus(bus)
    super(DFRobot_SU03T_I2C, self).__init__()

  def _write_cmd(self, data):
    '''!
      @brief writes data to a register
      @param data written data
    '''
    # logger.info(type(data))
    # logger.info(data)
    if isinstance(data, int):
      data = [data]
    elif isinstance(data, str):
      data = map(ord, list(data))
    elif isinstance(data, bytes):
      data = list(data)
    # logger.info(data)
    self._i2c.write_i2c_block_data(self._i2c_addr, self.SU03T_SEND_INFO_REG, data)

  def _read_id(self):
    '''!
      @brief read the data from the register
    '''
    # ID = 0xffff
    recv = self._i2c.read_i2c_block_data(self._i2c_addr, self.SU03T_VOICE_ID_MSB_REG, 2)
    ID = recv[0]
    ID = (ID<<8) + recv[1]
    return ID


class DFRobot_SU03T_UART(DFRobot_SU03T):
  '''!
    @brief define DFRobot_SU03T_UART class
    @details to use UART drive SU03T
  '''

  def __init__(self, Baud=9600):
    '''!
      @brief Module I2C communication init
      @param i2c_addr I2C communication address
      @param bus I2C communication bus
    '''
    self._ser = serial.Serial("/dev/ttyAMA0" ,baudrate=Baud,stopbits=1, timeout=0.5)
    if self._ser.isOpen == False:
      print("isOpen")
      self._ser.open()
    super(DFRobot_SU03T_UART, self).__init__()

  def _write_cmd(self, data):
    '''!
      @brief writes data to a register
      @param data written data
    '''
    self._ser.write(data)

  def _read_id(self):
    '''!
      @brief read the data from the register
    '''
    ID = 0xffff
    # ticks = time.time()
    # while True:
    #   data = self.__read()
    #   print(data)
    #   if data == [0xAA]:
    #     self.__read()
    #     ID = self.__read()[0]
        
    #     ID = (ID<<8) + self.__read()[0]
    
    #     self.__read()
    #     self.__read()
    #     break
    #   if (time.time() - ticks) > 1:
    #     return 0xffee
    recv_flag = 0
    while self._ser.in_waiting:
      receive_char = ord(self._ser.read(1))
      if 0 == recv_flag:
        if receive_char == 0xAA:
          recv_flag = 1
        else:
          recv_flag = 0
      elif 1 == recv_flag:
        if receive_char == 0x55:
          recv_flag = 2
        else:
          recv_flag = 0
      elif 2 == recv_flag:
        ID = receive_char
        recv_flag = 3
      elif 3 == recv_flag:
        ID = (ID<<8) + receive_char
        recv_flag = 4
      elif 4 == recv_flag:
        if receive_char == 0x55:
          recv_flag = 5
        else:
          recv_flag = 0
      elif 5 == recv_flag:
        if receive_char == 0xAA:
          return ID
        recv_flag = 0
      # if (time.time() - ticks) > 1:
      #   return 0xffee
    return ID

  # def __read(self):
  #   '''!
  #     @brief read the data from the register
  #   '''
  #   #recv=[0x00,0x00,0x00]
  #   if version_info.major == 2:
  #     recv=[0x00,0x00,0x00]
  #     recv = self._ser.read(1)
  #     recv =[ord(c) for c in recv]
  #     return recv
  #     # data = self._ser.read(1)
  #     # recv = map(ord, list(data))
  #     # return recv
  #   else :
  #     data =  self._ser.read(1)
  #     # recv = int.from_bytes(data, byteorder='big')
  #     recv = list(data)
  #     return recv
