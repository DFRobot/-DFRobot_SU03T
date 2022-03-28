# coding=utf8

'''!
  @file smart_voice.py
  @brief Get the information recognized by the voice module and control the module to speak.
  @details When the voice module recognized the information, a data packet will be sent from serial port, and the main controller will parse the data packet 
  @n to get the recognized data and then send data through serial port to control the voice module to speak according to the recognized data.
  @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
  @license     The MIT license (MIT)
  @author [fengli](li.feng@dfrobot.com)
  @version  V1.0
  @date  2022-03-09
  @url https://github.com/DFRobot/DFRobot_SU03T
'''
import sys
sys.path.append("../")
from DFRobot_SU03T import *
#Construct device
#Baud rate is 9600
su03t = DFRobot_SU03T(9600)

print("Start to recognize")

while True:

   time.sleep(0.01)
   id = su03t.read_entry_ID(timeout = 1) #Read the ID recognized by the voice module
   if id != 0xffee:
      print("id =")
      print(id)
   if id == 1:
     su03t.send(id,25.5)
   elif id == 13:
     su03t.send(id,2022,chr(3),chr(11))
