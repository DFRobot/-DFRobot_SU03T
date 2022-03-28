/*!
 * @file smartVoice.ino
 * @brief Get the information recognized by the voice module and control the module to speak.
 * @details When the voice module recognized the information, a data packet will be sent from serial port, and the main controller will parse the data packet 
 * @n to get the recognized data and then send data through serial port to control the voice module to speak according to the recognized data.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT license (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-10
 * @https://github.com/DFRobot/DFRobot_SU03T
 */
 

#include "DFRobot_SU03T.h"

#define ASKTEMPERATURE          1 ///<Query the current temperature
#define ASKHUMIDITY             2 ///<Query the current relative humidity
#define ASKPRESSURE             3 ///<Query atmospheric pressure
#define ASKCARBONDIOXIDE        4 ///<Query CO2 concentration 
#define ASKFORMALDEHYDE         5 ///<Query formaldehyde concentration
#define ASKAIRQUALITY           6 ///<Query air quality
#define ASKPARTICLECONCENTR     7 ///<Query PM concentration
#define ASKNOISEINTENSITY       8 ///<Query noise intensity
#define ASKHLIGHTINTENSITY     9 ///<Query light intensity
#define ASKUVRAYS              10 ///<Query UV intensity
#define ASKSOILMOISTURE        11 ///<Query soil moisture
#define ASKCURRENTTIME         12 ///<Query the current time
#define ASKCURRENTDATE         13 ///<Query the current date

#if ((defined ARDUINO_AVR_UNO) || (defined ARDUINO_AVR_NANO))
    #include <SoftwareSerial.h>
    SoftwareSerial Serial1(2, 3);  //RX, TX
    #define FPSerial Serial1

#else
    #define FPSerial Serial1
#endif
//Construct the voice control device
DFRobot_SU03T su03t;
void setup(void)
{
  Serial.begin(115200);
  #if (defined ESP32)
  Serial1.begin(9600, SERIAL_8N1, /*rx =*/P11, /*tx =*/P12);
  #else
  Serial1.begin(9600);
  #endif
  
  su03t.begin(Serial1);

}

void loop(void){
  
  uint8_t id = su03t.readEntryID(/*timeout detection*/1000);
  switch(id){
  
   case ASKTEMPERATURE:
     //You can add the code for measuring temperature here. Fill the function in the position of the second parameter of the following function to broadcast the current temperature.
     su03t.sendInfo(id,(double)25.76);
     break;
   case ASKHUMIDITY:
    //You can add the code for relative humidity here. Fill the function in the position of the second parameter of the following function to broadcast the current relative humidity and temperature.
     su03t.sendInfo(id,(double)53.7);
     break;
   case ASKPRESSURE:
     //You can add the code for measuring atmospheric pressure here.
     su03t.sendInfo(id,(double)101.5);
     break;
   case ASKCARBONDIOXIDE:
     //You can add the code for measuring CO2 concentration here.
     su03t.send(id,"%d",-50);
     break;
   case ASKFORMALDEHYDE:
     //You can add the code for measuring formaldehyde concentration here.
     su03t.send(id,"%f",(double)-20);
     break;
   case ASKAIRQUALITY:
     //You can add the code for measuring air quality here.
     su03t.sendInfo(id,160);
     break;
   case ASKPARTICLECONCENTR:
     //You can add the code for measuring PM concentration here.
     su03t.sendInfo(id,(double)16);
     break;
   case ASKNOISEINTENSITY:
     //You can add the code for measuring noise intensity here.
     su03t.sendInfo(id,(double)46);
     break;
   case ASKHLIGHTINTENSITY:
     //You can add the code for measuring light intensity here.
     //su03t.sendInfo(id,460);
     su03t.send(id,"%d",50);
     break;
   case ASKUVRAYS:
     //You can add the code for measuring UV intensity here.
     su03t.sendInfo(id,46);
     break;
   case ASKSOILMOISTURE:
     //You can add the code for measuring soil moisture here.
     su03t.sendInfo(id,(double)46.6);
     break;
   case ASKCURRENTTIME:
     //You can add the code for measuring the current time here.
     su03t.send(id,"%c%c",13,30);
     break;
   case ASKCURRENTDATE:
     //You can add the code for measuring the current date here.
     su03t.send(id,"%d%c%c",2022,3,10);
     break;
   default:break;
  }
  
  delay(100);
}
