#include "commandProcessor.h"

CommandProcessor::CommandProcessor()
{
	// Twiddle thumbs
}

void CommandProcessor::setup(RobotArm* robotArm)
{
	this->arm = robotArm;
}

// Loop function for the CommandProcessor
// Call in the Arduino loop function
void CommandProcessor::loop()
{
	if(Serial.available() > 0)
	{
		String command = Serial.readString();

		if(command.indexOf("setJointAngle") >= 0)
		{
			int jointNumber = command.substring(14,15).toInt();
			int angle = command.substring(16).toInt();

			this->setJointAngle(jointNumber, angle);

			Serial.println("Setting joint angle");
		}
	}
}

// -----------------------------------------------
// Commands --------------------------------------
// -----------------------------------------------

// Command to set a joint to a specific angle
void CommandProcessor::setJointAngle(int jointNumber, int angle)
{
	this->arm->setJointAngle(jointNumber, angle);
}