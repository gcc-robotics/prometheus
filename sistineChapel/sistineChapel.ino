#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "robotArm.h"
#include "debugger.h"

// This isn't the greatest placement
RobotArm arm = RobotArm();
Debugger debug;

// Enable or disable debugger
boolean runDebugger = false;

void setup() 
{ 
	// Start serial communication at the specified baud rate
	Serial.begin(9600); // 115200 is max

	// Set timeout for serial read functions to 100ms
	// Use 100ms instead of the default 1000 so that the
	// arm controls work better while the debugger is active
	Serial.setTimeout(100);

	arm.setup();
	debug.setup(arm.getMultiplexer(), arm.getMotorController(), &arm);

	// Set a random setPoint for the elbow
	randomSeed(analogRead(A3));
	arm.elbow(random(0, 361));
}

void loop() 
{
	arm.loop();

	if(runDebugger)
	{
		debug.loop();
	}
}
