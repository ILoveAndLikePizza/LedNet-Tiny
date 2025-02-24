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

#define PIN_R 19
#define PIN_G 18
#define PIN_B 5

class BuiltInLighting {
  private:
    byte map(float x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

  public:
    float r;
    float g;
    float b;

    void init() {
      pinMode(PIN_R, OUTPUT);
      pinMode(PIN_G, OUTPUT);
      pinMode(PIN_B, OUTPUT);
    }

    void write(float r, float g, float b) {
      // map the values and invert them because of common anode
      this->r = r;
      this->g = g;
      this->b = b;

      analogWrite(PIN_R, this->map(this->r, 0, 1, 255, 0));
      analogWrite(PIN_G, this->map(this->g, 0, 1, 255, 0));
      analogWrite(PIN_B, this->map(this->b, 0, 1, 255, 0));
    }
};
