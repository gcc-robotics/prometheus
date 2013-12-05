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
                        this->setJointAngle(command);
                }
				else if(command.indexOf("getJointAngle") >= 0)
				{
						this->getJointAngle(command);
				}
				else if(command.indexOf("getJointLimits") >= 0)
				{
						this->getJointLimits(command);
				}
        }
}

// -----------------------------------------------
// Commands --------------------------------------
// -----------------------------------------------

// Command to set a joint to a specific angle
void CommandProcessor::setJointAngle(String command)
{
        int jointNumber = command.substring(14,15).toInt();
        int angle = command.substring(16).toInt();

        this->arm->setJointAngle(jointNumber, angle);
}

void CommandProcessor::getJointAngle(String command)
{
	int jointNumber = command.substring(14, 15).toInt();
	int angle = this->arm->getJointAngle(jointNumber);
	
	Serial.println("jointAngle " + String(jointNumber) + " " + String(angle));
	
}

void CommandProcessor::getJointLimits(String command)
{
	int jointNumber = command.substring(15, 16).toInt();
	int min = this->arm->getJointMinimum(jointNumber);
	int max = this->arm->getJointMaximum(jointNumber);
	
	Serial.println("jointLimits " + String(jointNumber) + " " + String(min) + " " + String(max));
}