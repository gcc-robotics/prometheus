Prometheus
==========

The Prometheus is a robotic arm project created by the [Glendale Community College Robotics Academy][0].

This repository currently contains the software for the Arduino and the the software and the setup guide for the Raspberry Pi.

Sistine Chapel - Arduino Software
---------------------------------

The _sistineChapel_ subdirectory contains the Arduino software for controlling the Prometheus.

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

The _narthex_ subdirectory contains the software to be run on the Raspberry Pi. 

The software runs a simple web server to serve a static web page with controls for Prometheus enabling remote control.

[0]: http://www.robotics.glendale.edu
[1]: http://arduino.cc/en/Main/Software
[2]: https://github.com/gcc-robotics/prometheus/blob/master/sistineChapel/sistineChapel.ino