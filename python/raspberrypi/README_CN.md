## DFRobot_SU03T
- [English Version](./README.md)

SU03-T是一款离线语音模组，支持语音识别和数字的合成。本库用于arduino主控与SU03-T之间的串口和I2C通讯。

![正反面svg效果图](../../resources/images/SU03T.png)


## 产品链接（https://www.dfrobot.com.cn/）
    SKU: DFR0987

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

   1.合成语音<br>



## 库安装

该模块应该下载DFRobot_SU03T库使用
在终端运行：
```python
smart_voice.py
```

## Methods

```python

  def read_entry_ID(self ,timeout):
    '''!
      @fn read_entry_ID
      @brief 读取语音识别模块识别到的词条对应的ID
      @param timeout 超时检测
      @return  ID
    '''

  def send(self,id,*args):
    '''!
      @fn send
      @brief 发送多个数据,可以是不同的数据类型
      @param id 对应词条
      @param *args 表示可变参数,即答案需要多个参数
      @return 0表示成功
    '''
```


## 兼容性



| 主板         | 通过 | 未通过 | 未测试 | 备注 |
| ------------ | :--: | :----: | :----: | :--: |
| RaspberryPi2 |      |        |   √    |      |
| RaspberryPi3 |      |        |   √    |      |
| RaspberryPi4 |  √   |        |        |      |

* Python 版本

| Python  | 通过 | 未通过 | 未测试 | 备注 |
| ------- | :--: | :----: | :----: | ---- |
| Python2 |  √   |        |        |      |
| Python3 |  √   |        |        |      |

## History

- 2022/03/11 - 1.0.0 版本


## Credits

Written by fengli(li.feng@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))
