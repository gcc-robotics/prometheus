#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#include "robotArm.h"
#include "debugger.h"
#include "commandProcessor.h"

// This isn't the greatest placement
RobotArm arm = RobotArm();
Debugger debug = Debugger();
CommandProcessor commandProcessor = CommandProcessor();

// Enable or disable debugger
boolean runDebugger = false;

void setup() 
{ 
	// Start serial communication at the specified baud rate
	Serial.begin(115200); // 115200 is max

	// Set timeout for serial read functions to 10ms
	// Use 10ms instead of the default 1000 so that the
	// arm controls work better while the debugger is active
	Serial.setTimeout(50);

	// Setup the arm
	arm.setup();
	commandProcessor.setup(&arm);
	debug.setup(arm.getMultiplexer(), arm.getMotorController(), &arm);

	// Interrupt
	attachInterrupt(arm.interruptNumber, globalInterruptResponder, CHANGE);

	// Set a random setPoint for the elbow
	//randomSeed(analogRead(A3));
	//arm.waist(random(0, 360));
}

void loop() 
{
	arm.loop();
	commandProcessor.loop();

	if(runDebugger)
	{
		debug.loop();
	}
}

void globalInterruptResponder()
{
	arm.interruptResponder();
}

float stringToFloat(String input)
{
	int dotPosition = input.indexOf(".");
	int decimalPlaces = input.length() - (dotPosition + 1);

	int first = input.substring(0, dotPosition).toInt();
	int second = input.substring(dotPosition + 1).toInt();

	return first + (second / pow(10, decimalPlaces));
}