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

class BuiltInLighting {
  private:
    int pin_r;
    int pin_g;
    int pin_b;

    byte map(float x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

  public:
    float r;
    float g;
    float b;

    BuiltInLighting(int r_pin, int g_pin, int b_pin) {
      this->pin_r = r_pin;
      this->pin_g = g_pin;
      this->pin_b = b_pin;
    }

    void init() {
      pinMode(this->pin_r, OUTPUT);
      pinMode(this->pin_g, OUTPUT);
      pinMode(this->pin_b, OUTPUT);
    }

    void write(float r, float g, float b) {
      // map the values and invert them because of common anode
      this->r = r;
      this->g = g;
      this->b = b;

      analogWrite(this->pin_r, this->map(this->r, 0, 1, 255, 0));
      analogWrite(this->pin_g, this->map(this->g, 0, 1, 255, 0));
      analogWrite(this->pin_b, this->map(this->b, 0, 1, 255, 0));
    }
};
