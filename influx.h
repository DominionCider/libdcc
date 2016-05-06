#define TIMEOUT 5000

// Make HTTP POST Request to InfluxDB
void postRequest(String body, WiFiClient client) {
  Serial.print("POST ");
  Serial.println(INFLUX_PATH);
  Serial.println(body);

  client.print("POST ");
  client.print(INFLUX_PATH);
  client.print(" HTTP/1.1\r\n");

  client.print("Host: ");
  client.print(INFLUX_HOSTNAME);
  client.print(":");
  client.print(INFLUX_PORT);
  client.print("\r\n");

  client.print("User-Agent: ESP8266 Arduino\r\n");
  client.print("Accept: */*\r\n");

  client.print("Content-Length: ");
  client.print(body.length());
  client.print("\r\n");

  client.print("\r\n");
  client.print(body);

  for (int i=0; i<TIMEOUT; i+=50) {
    if (client.available()) {
      break;
    }
    delay(50);
  }

  while (client.available()) {
    Serial.print(client.readStringUntil('\r'));
  }
}
