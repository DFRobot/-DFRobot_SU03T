## DFRobot_SU03T
- [中文版](./README_CN.md)

The SU03-T offline voice module supports voice recognition and number synthesis. This library is used for serial(and I2C) communication between Arduino and SU03-T.


![正反面svg效果图](../../resources/images/SU03T.png)

## Product Link（https://www.dfrobot.com/）
  SKU: DFR0987

## Table of Contents

* [Summary](#summary)
* [Feature](#feature)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)  
* [History](#history)
* [Credits](#credits)

## Summary

   1.smart_voice <br>

## Installation

This Sensor should work with DFRobot_SpeechSynthesis on Raspberry Pi. <br>
Run the program:

```python
smart_voice.py
```

## Methods

```python

  def auto_exit_wakeup(self, mode):
    '''!
      @fn auto_exit_wakeup
      @brief Set whether to automatically exit the wakeup state. By default, the
      @n system automatically exits the wakeup state in 10 seconds.
      @param mode - True : Automatically exits the wake up state;
      @n            False : Do not exit the wake up state
    '''

  def read_entry_ID(self ,timeout):
    '''!
      @fn read_entry_ID
      @brief Read the ID corresponding to the entry recognized by the voice recognition module
      @param timeout timeout detection
      @return  ID
    '''

  def send(self,id,*args):
    '''!
      @fn send
      @brief Send multiple data, which can be in different data types
      @param id correspond to the entry
      @param *args represents changeable parameter, i.e. the answer requires multiple parameters
      @return 0 for success
    '''
```

## compatibility  

* Raspberry Pi Version

| Board        | Work Well | Work Wrong | Untested | Remarks |
| ------------ | :-------: | :--------: | :------: | ------- |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python Version

| Python  | Work Well | Work Wrong | Untested | Remarks |
| ------- | :-------: | :--------: | :------: | ------- |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |

## History

- 2022/3/11 - Version 1.0.0 released.
## Credits

Written by fengli(li.feng@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))
