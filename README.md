Prometheus
==========

The Prometheus is a robotic arm project created by the [Glendale Community College Robotics Academy][0].

This repository currently contains the software for the Arduino and the the software and the setup guide for the Raspberry Pi.

Sistine Chapel - Arduino Software
---------------------------------

The __sistineChapel__ subdirectory contains the Arduino software for controlling the Prometheus.

### Setup

To compile and upload to the Arduino use the free [Arduino IDE][1]. Once you have installed the IDE the following steps will let you upload and test the Arduino software.

* Start Arduino IDE
* Select Board -> Arduino Pro Mini 5V
* Select Processor
* Open [sistineChapel/sistineChapel.ino][2] 
* File -> Upload
* Open Serial Monitor if you have the debugger enabled

Narthex - Raspberry Pi Software
-------------------------------

The __narthex__ subdirectory contains the software to be run on the Raspberry Pi. 

The software runs a simple web server to serve a static web page with controls for Prometheus enabling remote control.

### Todo

- [x] Make simple web server to server the remote control interface and act on json requests
- [ ] Write class for Arduino to process the commands from the web server
- [ ] Test the serial communication between the server script and the Arduino
- [ ] Add WebGL rendering of the arm based on the set points
- [ ] Send information back to the remote control interface about the current status of the arm

### Links

* Connect RPI and Arduio using RPI GPIO http://blog.oscarliang.net/raspberry-pi-and-arduino-connected-serial-gpio/
* Connect RPI and Arduio using USB http://blog.oscarliang.net/connect-raspberry-pi-and-arduino-usb-cable/
* RPI to Arduino Serial Communication using Python http://www.raspberrypi.org/phpBB3/viewtopic.php?f=32&t=60746
* RPI to Arduino Serial Communication using C++ http://www.raspberrypi.org/phpBB3/viewtopic.php?t=14150&p=153257
* 222MB Raspbian Image http://www.raspbian.org/HexxehImages
* 118MB Raspbian Image (Seems better) http://www.linuxsystems.it/raspbian-wheezy-armhf-raspberry-pi-minimal-image/
* Ask Ubuntu Question about GParted (should be usable for partition expansion / growing) http://askubuntu.com/questions/51272/how-do-i-repartition-with-gparted
* Simple python webserver with ajax support on SO http://stackoverflow.com/questions/336866/how-to-implement-a-minimal-server-for-ajax-in-python
* WebGL STL Rendering http://tonylukasavage.com/blog/2013/04/10/web-based-stl-viewing-three-dot-js/
* Knockout JS, library used for data binding in narthex http://knockoutjs.com/

[0]: http://www.robotics.glendale.edu
[1]: http://arduino.cc/en/Main/Software
[2]: https://github.com/gcc-robotics/prometheus/blob/master/sistineChapel/sistineChapel.ino
