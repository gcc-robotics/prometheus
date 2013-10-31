#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "robotArm.h"
#include "debugger.h"

// This isn't the greatest placement
RobotArm arm = RobotArm();
Debugger debug = Debugger();

// Enable or disable debugger
boolean runDebugger = true;

void setup() 
{ 
	// Start serial communication at the specified baud rate
	Serial.begin(115200); // 115200 is max

	// Set timeout for serial read functions to 10ms
	// Use 10ms instead of the default 1000 so that the
	// arm controls work better while the debugger is active
	Serial.setTimeout(10);

	arm.setup();
	debug.setup(arm.getMultiplexer(), arm.getMotorController(), &arm);

	// Set a random setPoint for the elbow
	//randomSeed(analogRead(A3));
	//arm.elbow(random(0, 361));
}

void loop() 
{
	arm.loop();

	if(runDebugger)
	{
		debug.loop();
	}
}
