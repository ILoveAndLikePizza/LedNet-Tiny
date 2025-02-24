/*
Copyright © 2025 ngkon

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#define VERSION "1.0.0"

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <Preferences.h>

#include "Authentication.h"
#include "DMXController.h"
#include "BuiltInLighting.h"
#include "Webpage.h"

WebServer server(80);
Preferences pref;

DMXController dmx;
BuiltInLighting lights;

void setup() {
  Serial.begin(115200);

  dmx.init();
  lights.init();

  pref.begin("lighting", true);
  pref.getBytes("channels", dmx.data, sizeof(byte) * DMX_PACKET_SIZE);
  lights.r = pref.getFloat("light-r", 0);
  lights.g = pref.getFloat("light-g", 0);
  lights.b = pref.getFloat("light-b", 0);
  pref.end();

  server.on("/", []() {
    server.send(200, "text/html", generateWebpage(dmx.data, lights));
  });
  server.on("/channel", HTTP_POST, []() {
    if (!server.hasArg("c") || !server.hasArg("v")) {
      server.send(400, "text/plain", "400 - Some arguments are missing!");
      return;
    }

    int channel = constrain(server.arg("c").toInt(), 1, 512);
    byte value = constrain(server.arg("v").toInt(), 0, 255);
    dmx.data[channel] = value;

    pref.begin("lighting", false);
    pref.putBytes("channels", dmx.data, sizeof(byte) * DMX_PACKET_SIZE);
    pref.end();

    server.send(200, "text/plain", "Channel successfully changed!");
  });
  server.on("/light", HTTP_POST, []() {
    if (!server.hasArg("r") || !server.hasArg("g") || !server.hasArg("b")) {
      server.send(400, "text/plain", "400 - Some arguments are missing!");
      return;
    }

    float r = constrain(server.arg("r").toFloat(), 0, 1);
    float g = constrain(server.arg("g").toFloat(), 0, 1);
    float b = constrain(server.arg("b").toFloat(), 0, 1);

    pref.begin("lighting", false);
    if (lights.r != r) pref.putFloat("light-r", r);
    if (lights.g != g) pref.putFloat("light-g", g);
    if (lights.b != b) pref.putFloat("light-b", b);
    pref.end();

    lights.write(r, g, b);
    server.send(200, "text/plain", "Light successfully updated!");
  });
  server.on("/list", HTTP_GET, []() {
    String output = "[";
    for (int i=1; i<=512; i++) {
      output.concat(dmx.data[i]);
      if (i < 512) output.concat(", ");
    }
    output.concat("]");

    server.send(200, "application/json", output);
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "404 Not Found");
  });

  if (!WiFi.softAP("LedNet Tiny", AP_PASSWORD)) {
    Serial.println("Failed to generate an access point.");
    while (1);
  }
  if (!MDNS.begin("lednet-tiny")) {
    Serial.println("Failed to initiate MDNS.");
    while (1);
  }
  MDNS.addService("http", "tcp", 80);

  server.begin();
  lights.write(lights.r, lights.g, lights.b);
}

void loop() {
  dmx.update();
  server.handleClient();
}
