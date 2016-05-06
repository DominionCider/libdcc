#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

#define REQUIRE_AUTH \
  if (!server.authenticate(WEBSERVER_USERNAME, WEBSERVER_PASSWORD)) { \
    server.requestAuthentication(); \
    return; \
  }

void handleRestart() {
  REQUIRE_AUTH;

  server.send(200, "text/plain", "Restarting ESP");
  server.client().stop();
  Serial.println("Restart in 500ms");
  delay(500);
  ESP.restart();
}

void handleStatus() {
  REQUIRE_AUTH;

  String body = String("DCC ESP8266 Status\n\n");

  body += "ESP.getChipId(): " + String(ESP.getChipId(), HEX) + "\n";
  body += "ESP.getFreeHeap(): " + String(ESP.getFreeHeap()) + " B\n";
  body += "ESP.getResetReason(): " + String(ESP.getResetReason()) + "\n";
  body += "ESP.getFlashChipId(): " + String(ESP.getFlashChipId(), HEX) + "\n";
  body += "ESP.getFlashChipSize(): " + String(ESP.getFlashChipSize()) + " B\n";
  body += "ESP.getFlashChipSpeed(): " + String(ESP.getFlashChipSpeed()) + " Hz\n";
  body += "millis() Uptime: " + String(millis()) + "\n";
  body += "\n";

  body += "WiFi.SSID(): " + WiFi.SSID() + "\n";
  body += "WiFi.BSSIDstr(): " + WiFi.BSSIDstr() + "\n";
  body += "WiFi.RSSI(): " + String(WiFi.RSSI()) + "\n";
  body += "WiFi.macAddress(): " + WiFi.macAddress() + "\n";
  body += "WiFi.hostname(): " + WiFi.hostname() + "\n";
  body += "WiFi.localIP(): " + WiFi.localIP().toString() + "\n";
  body += "WiFi.gatewayIP(): " + WiFi.gatewayIP().toString() + "\n";
  body += "WiFi.subnetMask(): " + WiFi.subnetMask().toString() + "\n";

  server.send(200, "text/plain", body.c_str());
}

void handleNotFound() {
  server.send(404, "text/plain", "404 not found");
}
