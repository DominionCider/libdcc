#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const uint8_t charBitmap[][8] = {
  {0x0, 0xE, 0x11, 0x4, 0xA, 0x0, 0x4, 0x0},      // Wifi Connected
  {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4, 0x0},       // Wifi Disconnected
  {0x1C, 0x14, 0x1C, 0x10, 0x15, 0x5, 0x7, 0x0},  // Pasteurization Unit
  {0xE, 0xA, 0xE, 0x0, 0x0, 0x0, 0x0, 0x0},       // Degrees
  {0x0, 0x0, 0x0, 0x1C, 0x4, 0x1C, 0x10, 0x1C}    // Subscript 2
};

void loadCustomChars(LiquidCrystal_I2C lcd) {
  int charBitmapSize = (sizeof(charBitmap ) / sizeof (charBitmap[0]));
  for ( int i = 0; i < charBitmapSize; i++ ) {
    lcd.createChar ( i, (uint8_t *)charBitmap[i] );
  }
}

String leftPad(String s, int len) {
  int inLen = s.length();
  String out;
  out.reserve(len);
  if (inLen > len) {
    for (int i=0; i<len; i++) {
      out += "#";
    }
    return out;
  }
  for (int i=0; i<len-inLen; i++) {
    out += " ";
  }
  return out + s;
}
