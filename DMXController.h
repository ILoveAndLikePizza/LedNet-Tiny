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

#include <esp_dmx.h>

class DMXController {
  private:
    unsigned long lastUpdated;
    int pin;

  public:
    byte data[DMX_PACKET_SIZE];

    DMXController(int dmx_pin) {
      this->pin = dmx_pin;
    }

    void init() {
      dmx_config_t conf = DMX_CONFIG_DEFAULT;
      dmx_personality_t _ps[] = {};

      dmx_driver_install(1, &conf, _ps, 0);
      dmx_set_pin(1, this->pin, -1, -1);
    }

    void update() {
      unsigned long now = millis();
      if (now - this->lastUpdated >= 25) {
        dmx_write(1, this->data, DMX_PACKET_SIZE);

        this->lastUpdated = now;
      }

      dmx_send_num(1, DMX_PACKET_SIZE);
      dmx_wait_sent(1, DMX_TIMEOUT_TICK);
    }
};
