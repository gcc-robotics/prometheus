Prometheus
==========

The Prometheus is a robotic arm project created by the [Glendale Community College Robotics Academy][0].

This repository contains the software, electronics schematics, and mechanical models for the Prometheus robotic arm.

Prometheus Startup Procedure
----------------------------
1. Make sure the hardware and electronics are connected correctly.
2. Power up the Raspberry Pi, Router, and only the 5V rail for the electronics board.
   __Do not turn on the 12V rail! This will cause the arm to move to it's default position which could cause rapid movement and the arm could impact something.__
3. Connect a computer, tablet, or phone to the router either by ethernet or WiFi. The WiFi info is on the router.
4. Now open a browser and go to:

		10.33.0.2
	
	 This will open up the Prometheus Remote Control Interface.

5. Now click __Settings__ in the top right corner of the screen and then turn on the __Command Console__. Click the __X__ in the top right corner to close the Settings Screen.

6. Now go to the bottom of the page and tun the following commands:

        getJointLimits 0
        getJointLimits 1
        getJointLimits 2
        getJointLimits 3
        getJointLimits 4

7. Now use the setpoint sliders to set all the joint to a position where the arm will not hit anything when it is turned on.

8. Finaly, once you are confident that the setpoints are good turn on the 12V rail. The arm will now move to the setpoints you specified and you will be able to control it using the remote control interface.

Sistine Chapel - Arduino Software
---------------------------------

The __sistineChapel__ subdirectory contains the Arduino software for controlling the Prometheus.

### Setup

To compile and upload to the Arduino use the free [Arduino IDE][1]. Once you have installed the IDE the following steps will let you upload and test the Arduino software.

* Start Arduino IDE
* Select Tools -> Board -> Arduino Pro Mini 5V W/ ATmega328
* Open [sistineChapel/sistineChapel.ino][2] 
* File -> Upload

Narthex - Raspberry Pi Software
-------------------------------

The __narthex__ subdirectory contains the software to be run on the Raspberry Pi. 

The software runs a simple web server to serve a static web page with controls for Prometheus enabling remote control. The python code transfers commands between the Arduino and the Javascript running in the open webpage.

[0]: http://www.robotics.glendale.edu
[1]: http://arduino.cc/en/Main/Software
[2]: https://github.com/gcc-robotics/prometheus/blob/master/sistineChapel/sistineChapel.ino
