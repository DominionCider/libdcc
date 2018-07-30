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
