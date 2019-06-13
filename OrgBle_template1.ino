/*

Copyright (c) 2012-2014 RedBearLab

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

/*
 *    HelloWorld
 *
 *    HelloWorld sketch, work with the Chat iOS/Android App.
 *    It will send "Hello World" string to the App every 1 sec.
 *
 */

//"RBL_nRF8001.h/spi.h/boards.h" is needed in every new project
#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include "Arduino.h"
#include "Wire.h"
#include <hackair.h>      // https://github.com/hackair-project/hackAir-Arduino
#include <DHT.h>

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

hackAIR sensor(SENSOR_SDS011);
struct hackAirData data;

float dust1; //Stores pm2.5 value
float dust2; //Stores pm10 value
float hum;  //Stores humidity value
float temp; //Stores temperature value

char buf[20], buf1[20];
String message, message1;

void setup()
{
  //  
  // For BLE Shield and Blend:
  //   Default pins set to 9 and 8 for REQN and RDYN
  //   Set your REQN and RDYN here before ble_begin() if you need
  //
  // For Blend Micro:
  //   Default pins set to 6 and 7 for REQN and RDYN
  //   So, no need to set for Blend Micro.
  //
  //ble_set_pins(3, 2);
  
  // Set your BLE advertising name here, max. length 10
  ble_set_name("NOVAsds011");
  
  // Init. and start BLE library.
  ble_begin();
  
  // Enable serial debug
  Serial.begin(57600);

  Serial1.begin(9600);
  while (!Serial1); // needed only for Leonardo
  
  // Initialize the PM sensor
  sensor.begin();
  dht.begin();
}

void loop()
{

  //Send bluetooth message
  if ( ble_connected() )
  {
//      if ( ble_available() )
//  {
//    while ( ble_available() )
//      Serial.write(ble_read());
//      
//    Serial.println();
//  }
    mainjob();
    ble_write_bytes((unsigned char *)buf, strlen(buf));
    ble_do_events();
    ble_write_bytes((unsigned char *)buf1, strlen(buf1));
  }
  ble_do_events();

}

void mainjob() {
  
  //Measure data
  delay(10000);
  ble_do_events();
  delay(10000);
  ble_do_events();
  delay(10000);
  ble_do_events();
  delay(10000);
  ble_do_events();
  sensor.clearData(data);
  delay(8000);
  ble_do_events();
  sensor.readAverageData(data, 10); // 10 averages
  dust1 = data.pm25;
  dust2 = data.pm10;
  delay(500);
  hum = dht.readHumidity(); 
  delay(1000);
  temp = dht.readTemperature();
  delay(500);

    //Prepare message to send
  message = "a";
  message += String(dust1);
  message += " ";
  message += String(dust2);
  message += " ";
  message.toCharArray(buf, 20);

  message1 = "b";
  message1 += String(hum);
  message1 += " ";
  message1 += String(temp);
  message1.toCharArray(buf1, 20);
}

