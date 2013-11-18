Prometheus
==========

The Prometheus is a robotic arm project created by the [Glendale Community College Robotics Academy][0].

This repository currently contains the software for the Arduino and the the software and the setup guide for the Raspberry Pi.

Sistine Chapel - Arduino Software
---------------------------------

The sistineChapel subdirectory contains the Arduino software for controlling the Prometheus.

### Setup

To compile and upload to the Arduino use the free [Arduino IDE][1]. Once you have installed the IDE the following steps will compile and upload the code to your Arduino.

* Start Arduino IDE
* Select Board -> Arduino Pro Mini 5V
* Select Processor
* Open [sistineChapel/sistineChapel.ino][2] 
* File -> Upload
* Open Serial Monitor if you have the debugger enabled

Raspberry Pi Software & Setup Guide
-----------------------------------

The Raspberry Pi is used to provide remote control of the Prometheus.



[0]: http://www.robotics.glendale.edu
[1]: http://arduino.cc/en/Main/Software
[2]: https://github.com/gcc-robotics/prometheus/blob/master/sistineChapel/sistineChapel.ino