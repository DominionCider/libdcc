#define TIMEOUT 5000

// Make HTTP POST Request to InfluxDB
void postRequestAsync(String body, WiFiClient client) {
  Serial.print("POST ");
  Serial.println(INFLUX_PATH);
  Serial.println(body);

  String out = String("POST ") + INFLUX_PATH + " HTTP/1.1\r\n";
  out += String("Host: ") + INFLUX_HOSTNAME + ":" + INFLUX_PORT + "\r\n";
  out += "User-Agent: ESP8266 Arduino\r\n";
  out += "Accept: */*\r\n";
  out += String("Content-Length: ") + body.length() + "\r\n\r\n";
  out += body;

  client.print(out);

  Serial.println("DONE POST");
}

void postRequest(String body, WiFiClient client) {
  postRequestAsync(body, client);
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
