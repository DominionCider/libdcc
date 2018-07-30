/***************************************************
  This is a library for the AM2315 Humidity & Temp Sensor

  Designed specifically to work with the AM2315 sensor from Adafruit
  ----> https://www.adafruit.com/products/1293

  These displays use I2C to communicate, 2 pins are required to interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.

  Additional improvements added by Daniel Sandoval/EasternStarGeek
  8 July, 2013.  (Parent Library: Adafruit_AM2315)

  Heavily rewritten for Craft Metrics, Luke Cyca, Jan 2018

  BSD license, all text above must be included in any redistribution
 ****************************************************/
#include "Wire.h"

#define AM2315_I2CADDR       0x5C
#define AM2315_READREG       0x03

void setup_AM2315() {
  Wire.begin(4, 5);   // some ESP8266 devices require 5, 4 instead of 4,5
  //Wire.begin(5, 4);   // some ESP8266 devices require 5, 4 instead of 4,5

  Wire.setClock(400000L);
}

boolean read_AM2315(float *temperature, float *humidity) {
  uint8_t data[8];

  noInterrupts();

  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.write(AM2315_READREG);
  Wire.endTransmission();

  delayMicroseconds(50000);

  Wire.beginTransmission(AM2315_I2CADDR);
  Wire.write(AM2315_READREG);
  Wire.write(0x00);  // start at address 0x0
  Wire.write(4);  // request 4 bytes data
  Wire.endTransmission();

  delayMicroseconds(50000);

  Wire.requestFrom(AM2315_I2CADDR, 8);
  for (uint8_t i = 0; i < 8; i++) {
    data[i] = Wire.read();
    //Serial.println(data[i], HEX);
  }

  interrupts();
  yield();

  if ((data[0] == AM2315_READREG) && (data[1] == 4))  {

    (*humidity) = data[2];
    (*humidity) *= 256;
    (*humidity) += data[3];
    (*humidity) /= 10;

    (*temperature) = data[4] & 0x7F;
    (*temperature) *= 256;
    (*temperature) += data[5];
    (*temperature) /= 10;

    // Temperature is negative
    if (data[4] & 0x80) {
      (*temperature) = -(*temperature);
    }

    return true;

  } else  {
    (*humidity) = NAN;
    (*temperature) = NAN;
    return false;

  }
}
