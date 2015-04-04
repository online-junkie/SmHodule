# SmHodule

SmHodule is using the Wifi SoC ESP8266, the ESP8266 is a System on Chip and is as powerful as an Arduino.

SmHodule = Smart Home module for the Internet of Things.

This module is based on a set of examples found in the Internet about ESP8266 and implementing a actor, sender, sensor and I2C OLED.

The Setup data is cached in the modules flash memory.

Feel free to expand SmHodule and add your own ideas.

## Compile

Compile the code and then make clean, make html, make all, make flashall
 
For first time setup keep GPIO15 pulled to ground for 35 seconds.
After first time configuration the module reboots and starts as an Access Point. Connect your wifi network to the new AP and use the default AP IP: 192.168.4.1 in your browser to access the module. Note: Your local router has to be DHCP enabled.

The device MAC and also the IP the module receives is shown on the SmHodule setup page. Make this IP static DHCP by using the modules name as the device name for your DHCP configuration.

Enjoy
