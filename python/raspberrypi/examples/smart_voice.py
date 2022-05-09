# -*- coding: utf-8 -*
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
import sys, os
sys.path.append("../")
sys.path.append(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))   # 包含库文件所在路径
from DFRobot_SU03T import *

#Construct device
su03t = DFRobot_SU03T_UART()   # UART PORT
# su03t = DFRobot_SU03T_I2C()   # I2C PORT

print("Start to recognize")

'''!
  @fn auto_exit_wakeup
  @brief Set whether to automatically exit the wakeup state. By default, the
  @n system automatically exits the wakeup state in 10 seconds.
  @param mode - True : Automatically exits the wake up state;
  @n            False : Do not exit the wake up state
'''
su03t.auto_exit_wakeup(True)

while True:

  time.sleep(0.01)
  id = su03t.read_entry_ID()   #Read the ID recognized by the voice module
  if id != 0xffff:   # Invalid ID
    print("id = %d" % id)
  if id == 1:
    su03t.send(id,25.5)
  elif id == 13:
    su03t.send(id,2022,chr(3),chr(11))
