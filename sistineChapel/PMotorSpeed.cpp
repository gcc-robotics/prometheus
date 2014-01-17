#import <Arduino.h>
#include <math.h>
#include "PMotorSpeed.h"

PMotorSpeed::PMotorSpeed()
{
	this->numMotors = 5;

	for(int i = 0; i < this->numMotors; i++)
	{
		this->setPoint[i] = 0.0;
		this->setSetPoint(i, 0);
	}

	proportionalgain[0] = 1.0;
	proportionalgain[1] = 1.0;
	proportionalgain[2] = 1.0;
	proportionalgain[3] = 1.0;
	proportionalgain[4] = 1.0;

	min[0] = 0.0;
	min[1] = 0.0;
	min[2] = 0.0;
	min[3] = 0.0;
	min[4] = 0.0;

	max[0] = 359.9;
	max[1] = 359.9;
	max[2] = 359.9;
	max[3] = 359.9;
	max[4] = 359.9;
}

void PMotorSpeed::setProportionalGain(int jointNumber, float newproportionalgain)
{
	jointNumber = constrain(jointNumber, 0, 5);
	newproportionalgain = constrain(newproportionalgain, 0.0, 5.0);

	this->proportionalgain[jointNumber] = newproportionalgain;
}

int PMotorSpeed::calculate(int jointNumber, float currentAngle)
{
	jointNumber = constrain(jointNumber, 0, 5);
	currentAngle = constrain(currentAngle, -360.0, 360.0);

	float currentAngleError = this->getAngleError(jointNumber, currentAngle);

	return currentAngleError * proportionalgain[jointNumber];
}

void PMotorSpeed::setSetPoint(int jointNumber, float setPoint)
{
	jointNumber = constrain(jointNumber, 0, 5);
	
	if(setPoint < 0.0)
	{
		setPoint = setPoint * -1.0;
	}

	setPoint = fmod(setPoint, 360.0);
	
	if(setPoint < min[jointNumber])
	{
		setPoint = min[jointNumber];
	}
	else if(setPoint > max[jointNumber])
	{
		setPoint = max[jointNumber];
	}

	this->setPoint[jointNumber] = setPoint;
}

float PMotorSpeed::getJointMin(int jointNumber)
{
	jointNumber = constrain(jointNumber, 0, 5);
	return min[jointNumber];
}

float PMotorSpeed::getJointMax(int jointNumber)
{
	jointNumber = constrain(jointNumber, 0, 5);
	return max[jointNumber];
}

float PMotorSpeed::getAngleError(int jointNumber, float currentAngle)
{
	float targetAngle = this->setPoint[jointNumber];

	// Shift target angle to -180 to 180
	targetAngle -= 180.0;

	// Limit current angle between 0 and 360
	if(currentAngle < 0)
	{
		currentAngle = 0;
	}

	currentAngle = fmod(currentAngle, 360);

	// Shift current angle to -180 to 180
	currentAngle -= 180.0;

	// Get the smallest angle error
	// Test the logic at: http://jsfiddle.net/frLFG/3/
	float angleError = targetAngle - currentAngle;

	if(angleError > 180)
	{
		angleError -= 360.0;
	}
	else if(angleError < -180)
	{
		angleError += 360.0;
	}

	return fmod(angleError, 360);
}
