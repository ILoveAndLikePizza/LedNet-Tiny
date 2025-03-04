# LedNet Tiny
### A compact, ESP32-based DMX generator
<br>

## Firmware setup
1. Navigate to your sketchbook directory (typically `Arduino` in your home directory) and download/clone the repository there.
2. Run `set-ap-password.sh` to configure the password for the access point.
3. Launch the Arduino IDE and load the `LedNet-Tiny.ino` sketch.
4. Ensure that the ESP32 board is installed (if not, check [this tutorial](https://randomnerdtutorials.com/installing-esp32-arduino-ide-2-0/));
5. Ensure that the required libraries are installed:
    - **esp_dmx** *(by Mitch Weisbrod)*.
6. Go to **Tools** > **Board** and ensure that it is set to "**ESP32 Dev Module**";
7. Click the Upload button.

### Congratulations, the LedNet Tiny firmware has been flashed successfully!

## Known issues
- When encountering an error looking like `error: 'uart_signal_conn_t' has no member named 'module'`, try downgrading the ESP32 board to 3.0.4.

Next, you can use any device to connect to the LedNet Tiny, and access its web server on `lednet-tiny.local`.
