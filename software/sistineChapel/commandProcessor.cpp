#include "commandProcessor.h"

String floatToString(float input)
{
	int first = int(input);
	String result = String(first);

	input -= first;
	input *= 1000;

	result += "." + String(int(input));

	return result;
}

long int lastTime = 0;

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
		else if(command.indexOf("getJointGains") >= 0)
		{
			this->getJointGains(command);
		}
		else if(command.indexOf("setJointGains") >= 0)
		{
			this->setJointGains(command);
		}
		else if(command.indexOf("getJointError") >= 0)
		{
			this->getJointError(command);
		}
		else if(command.indexOf("setClawState") >= 0)
		{
			this->setClawState(command);
		}
	}

	if(millis() - lastTime >= 100)
	{
		this->armStatus();
		lastTime = millis();
	}
}

void CommandProcessor::armStatus()
{
	float error0 = this->arm->getPIMotorSpeed()->getAngleError(0, this->arm->getJointAngle(0));

	float error1 = this->arm->getPIMotorSpeed()->getAngleError(1, this->arm->getJointAngle(1));

	float error2 = this->arm->getPIMotorSpeed()->getAngleError(2, this->arm->getJointAngle(2));

	float error3 = this->arm->getPIMotorSpeed()->getAngleError(3, this->arm->getJointAngle(3));

	float error4 = this->arm->getPIMotorSpeed()->getAngleError(4, this->arm->getJointAngle(4));


	Serial.println("armStatus " + floatToString(this->arm->getJointAngle(0)) 
	               + " " + floatToString(this->arm->getJointAngle(1)) + " " 
	               + floatToString(this->arm->getJointAngle(2)) + " " 
	               + floatToString(this->arm->getJointAngle(3)) + " " 
	               + floatToString(this->arm->getJointAngle(4)) + " " 
	               + floatToString(error0) 
	               + " " + floatToString(error1)
	               + " " + floatToString(error2)
	               + " " + floatToString(error3)
	               + " " + floatToString(error4));
	
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

	Serial.println("jointSetPoint " + String(jointNumber) + " " + String(angle));
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

void CommandProcessor::getJointGains(String command)
{
	int jointNumber = command.substring(14, 15).toInt();

	float PGain = this->arm->getPIMotorSpeed()->getProportionalGain(jointNumber);
	int newPGain = int(PGain * 100.0);

	float IGain = this->arm->getPIMotorSpeed()->getIntegralGain(jointNumber);
	int newIGain = int(IGain * 100.0);

	float DGain = 0.0; //this->arm->getPIMotorSpeed()->getDerivativeGain(jointNumber);
	int newDGain = DGain * 100;

	Serial.println("jointGains " + String(jointNumber) + " " + String(newPGain / 100) + "." + String(newPGain % 100) + " " + String(newIGain / 100) + "." 
		+ String(newIGain % 100) + " " + String(newDGain / 100) + "." + String(newDGain % 100));
}

void CommandProcessor::setJointGains(String command)
{
	int jointNumber = command.substring(14, 15).toInt();
	float proportionalgain = command.substring(16, 17).toInt() + float(command.substring(18, 20).toInt()) / 100;
	float integralgain = command.substring(21, 22).toInt() + float(command.substring(23, 25).toInt()) / 100;
	float derivativegain = command.substring(26, 27).toInt() + float(command.substring(28, 30).toInt()) / 100;

	this->arm->getPIMotorSpeed()->setProportionalGain(jointNumber, proportionalgain);
	this->arm->getPIMotorSpeed()->setIntegralGain(jointNumber, integralgain);
	//this->arm->getPIMotorSpeed()->setDerivativeGain(jointNumber, derivativegain);
}

void CommandProcessor::getJointError(String command)
{
	int jointNumber = command.substring(14, 15).toInt();
	float error = this->arm->getPIMotorSpeed()->getAngleError(jointNumber, this->arm->getJointAngle(jointNumber));
	int newError = error * 100;

	Serial.println("jointError " + String(jointNumber) + " " + String(newError / 100) + "." + String(abs(newError % 100)));
}

void CommandProcessor::setClawState(String command)
{
	int commandState = command.substring(13, 14).toInt();

	if(commandState == 0)
	{
		this->arm->openClaw();
	}
	else if(commandState == 2)
	{
		this->arm->closeClaw();
	}
	else
	{
		this->arm->brakeClaw();
	}
}