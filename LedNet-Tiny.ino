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

#define PIN_AP_MODE 15
#define PIN_DMX_OUT 25
#define PIN_R 19
#define PIN_G 18
#define PIN_B 5

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

DMXController dmx(PIN_DMX_OUT);
BuiltInLighting led(PIN_R, PIN_G, PIN_B);

String Wifi_SSID;
String Wifi_PW;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_AP_MODE, INPUT_PULLUP);

  dmx.init();
  led.init();

  pref.begin("lighting", true);
  pref.getBytes("channels", dmx.data, sizeof(byte) * DMX_PACKET_SIZE);
  float originR = pref.getFloat("light-r", 0);
  float originG = pref.getFloat("light-g", 0);
  float originB = pref.getFloat("light-b", 0);
  Wifi_SSID = pref.getString("wifi-ssid", "");
  Wifi_PW = pref.getString("wifi-pw", "");
  pref.end();

  server.on("/", []() {
    server.send(200, "text/html", generateWebpage(dmx.data, led, Wifi_SSID, Wifi_PW));
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

    server.send(200, "text/plain", "200 - OK");
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
    if (led.r != r) pref.putFloat("light-r", r);
    if (led.g != g) pref.putFloat("light-g", g);
    if (led.b != b) pref.putFloat("light-b", b);
    pref.end();

    led.write(r, g, b);
    server.send(200, "text/plain", "200 - OK");
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
  server.on("/connect", HTTP_POST, []() {
    if (!server.hasArg("ssid") || !server.hasArg("pw")) {
      server.send(400, "text/plain", "400 - Some arguments are missing!");
      return;
    }

    pref.begin("lighting", false);
    pref.putString("wifi-ssid", server.arg("ssid"));
    pref.putString("wifi-pw", server.arg("pw"));
    pref.end();

    server.send(200, "text/plain", "200 - OK");
  });
  server.on("/connect/clear", HTTP_DELETE, []() {
    pref.begin("lighting", false);
    pref.remove("wifi-ssid");
    pref.remove("wifi-pw");
    pref.end();

    server.send(200, "text/plain", "200 - OK");
  });
  server.on("/reset", HTTP_DELETE, []() {
    pref.begin("lighting", false);
    pref.clear();
    pref.end();

    server.send(200, "text/plain", "200 - OK");
  });
  server.onNotFound([]() {
    server.send(404, "text/plain", "404 - Not Found");
  });

  // button NOT pressed and credentials saved?
  if (digitalRead(PIN_AP_MODE) && Wifi_SSID != "" && Wifi_PW != "") {
    WiFi.begin(Wifi_SSID, Wifi_PW);

    // blue flash to indicate connection attempt
    while (WiFi.status() != WL_CONNECTED) {
      led.write(0, 0, 1);
      delay(100);
      led.write(0, 0, 0);
      delay(100);
    }
    // go green to indicate successfully established connection
    led.write(0, 1, 0);
  } else { // button pressed or nothing saved?
    if (!WiFi.softAP("LedNet Tiny", AP_PASSWORD)) {
      Serial.println("Failed to generate an access point.");
      while (1);
    }

    // 4x magenta flash to indicate AP mode
    for (int i=0; i<4; i++) {
      led.write(1, 0, 1);
      delay(500);
      led.write(0, 0, 0);
      delay(500);
    }
  }
  if (!MDNS.begin("lednet-tiny")) {
    Serial.println("Failed to initiate MDNS.");
    while (1);
  }
  MDNS.addService("http", "tcp", 80);

  server.begin();
  delay(1000); // to read the LED state
  led.write(0, 0, 0);
  delay(500);
  led.write(originR, originG, originB);
}

void loop() {
  dmx.update();
  server.handleClient();
}
