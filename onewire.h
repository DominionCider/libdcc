#include <OneWire.h>

OneWire  ds(ONE_WIRE_PIN);

// Instruct all ds18b20 devices to take a measurement
void takeAllMeasurementsAsync() {
  ds.reset();
  ds.skip();
  ds.write(0x44, 1); // Start conversion
}

void takeAllMeasurements() {
  takeAllMeasurementsAsync();
  delay(800);
}

// Reads data from sensor addr `a` and puts the temperature in temp (in celsius)
// Returns 1 on success, 0 on failure
int readTemperature(byte a[8], float *temp) {
  ds.reset();
  ds.select(a);
  ds.write(0xBE); // Read Scratchpad

  byte data[9];
  for (int i=0; i<9; i++) {
    data[i] = ds.read();
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  byte calculatedCRC = OneWire::crc8(data, 8);
  if (calculatedCRC != data[8]) {
    Serial.print("CRC Failed: ");
    Serial.print(calculatedCRC, HEX);
    Serial.print(" != ");
    Serial.println(data[8], HEX);
    return 0;
  }

  uint8_t reg_msb = data[1];
  uint8_t reg_lsb = data[0];
  uint16_t TReading = reg_msb << 8 | reg_lsb;

  int SignBit, Whole, Fract;
  SignBit = TReading & 0x8000;  // test most sig bit
  if (SignBit) {
    // negative
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }

  Whole = TReading >> 4;  // separate off the whole and fractional portions
  Fract = (TReading & 0xf) * 100 / 16;

  *temp = Whole + (TReading & 0xf) / 16.;
  if (SignBit) {
    *temp *= -1;
  }

  // The power-on value of the DS18B20's internal register
  // is 85. If we read that or higher, let's just say it
  // failed.
  if (*temp >= 85.) {
    return 0;
  }

  return 1;
}

String addrToString(byte addr[8]) {
  char addr_hex[17];
  os_sprintf(
    addr_hex,
    "%02X %02X %02X %02X %02X %02X %02X %02X",
    addr[0],
    addr[1],
    addr[2],
    addr[3],
    addr[4],
    addr[5],
    addr[6],
    addr[7]
  );
  return String(addr_hex);
}

// HTTP server handler which finds all sensors on the 1Wire bus
// and takes their measurements.
void handleSensors() {
  REQUIRE_AUTH;

  float f;
  byte addr[8];
  String body = String("");

  takeAllMeasurements();

  ds.reset();
  while (ds.search(addr) == true) {
    if (readTemperature(addr, &f)) {
      body += addrToString(addr) + ": " + String(f, 3) + "\n";
    } else {
      body += addrToString(addr) + ": failed to read\n";
    }
  }

  server.send(200, "text/plain", body);
}
