#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "robotArm.h"
#include "debugger.h"

// This isn't the greatest placement
RobotArm arm = RobotArm();
//Debugger debug = Debugger(arm.getMultiplexer(), arm.getMotorController());

// Enable or disable debugger
boolean runDebugger = true;

void setup() 
{ 
	Serial.begin(9600); // 115200 is max

	arm.setup();
}

void loop() 
{
	arm.loop();

	/*if(runDebugger)
	{
		debug.loop();
	}*/
}
