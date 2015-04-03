SmHodule

SmHodule is using the new Wifi enabled ESP8266 SoC.

SmHodule is a Smart Home module for the Internet of Things.

This module is based on a set of examples found in the Internet about ESP8266 and implementing a actor, sender, sensor and I2C OLED.

The Setup data is cached in the modules flash memory.

Feel free to expand SmHodule with your own ideas.

Compile:
 make clean
 make html
 make all
 make flashall
 
For first time setup keep GPIO15 pulled to ground for 35 seconds.
Afterwards connect to 192.168.4.1 and make the wifi connect to your local access point. Your local router need to be DHCP enabled.

The device MAC and IP is then shown on the setup page. Make this IP static and use the device name shown in setup.
