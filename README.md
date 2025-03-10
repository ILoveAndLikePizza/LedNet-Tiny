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

## Operating the device
- When plugging in the power for the very first time, the built-in LED will flash magenta 4 times.
- This is an indication that the LedNet Tiny will generate an access point named `LedNet Tiny`, with the password you set earlier.
- Use another device to connect to the LedNet Tiny.
- Navigate to its webpage on `lednet-tiny.local`.
- Here, you can change the DMX channel values, the built-in LED color, and optionally, the Wi-Fi network to connect to.

### Connecting to a Wi-Fi network
- Enter an SSID and the corresponding password and click 'Connect'.
- Changes to DMX channels and the built-in LED will be applied instantly, but now, the LedNet Tiny has to be reconnected to power.
- Now, the built-in LED will rapidly flash blue, meaning that it tries to connect to the specified Wi-Fi network.
- When successful, the built-in LED will turn green, starting DMX transmission shortly after.

### Resetting the connection
- Once credentials are set, the LedNet Tiny will always try to connect to those.
- To remove the saved credentials, click on 'Reset connection' in the web portal.
- Another override can be achieved by disconnecting the power, holding the AP mode button, then plugging the power back in.
- Now, the LedNet Tiny will behave like it is plugged in for the first time again.

## Known issues
- When encountering an error looking like `error: 'uart_signal_conn_t' has no member named 'module'`, try downgrading the ESP32 board to 3.0.4.

# Enjoy!