# arduino sandbox

## Setup Hacks

### Due to ESP32-BLE being pretty big

The default partition size of 1.3MB wouldn't work.  Luckily we can resize them.

Had to manually modify:

 ~/Library/Arduino15/packages/esp32/hardware/esp32/1.0.0/boards.txt

 to adjust the partition type to "min_spiffs" and change the maximum upload size to
`2031616` so ArduinoIDE wouldn't complain that our build exceeds the size.

## TODO

1. Make chirp i2c sensor work I think we have this code in `ArduinoIDE/chirp_control`

2. Make WIFI work I think we've got this too

3. Get dev environment and project cleaned up

4. Store WIFI SSID and Password Securely

5. Figure out way to set SSID / Password via Screen?  Bluetooth?  iOS app?

Bluetooth seems to work, but i'm not sure if I can do bluetooth and wifi together.

6. Write some code to send message out

7. Write a service to receive and store message

8. Makefile for automated builds

We'll want to automate the builds at some point

Example scaffolding here:

    git clone https://github.com/plerup/makeEspArduino.git
