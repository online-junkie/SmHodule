SmHodule

SmHodule is using the new Wifi enabled ESP8266 SoC.

SmHodule is a Smart Home module for the Internet of Things.

This module is based on a set of examples found in the Internet about ESP8266 and implementing a actor, sender, sensor and I2C OLED.

The Setup data is cached in the modules flash memory.

Feel free to expand SmHodule and add your own ideas.

Compile the code and then make clean, make html, make all, make flashall
 
For first time setup keep GPIO15 pulled to ground for 35 seconds.
Afte this configuration reset connect to the defualt IP: 192.168.4.1 and have the wifi connect to your local access point. Your local router has to be DHCP enabled.

The device MAC and also the IP the module receives is shown on the setup page. Make this IP static DHCP by using the modules name as the device name for your DHCP configuration.

Enjoy
