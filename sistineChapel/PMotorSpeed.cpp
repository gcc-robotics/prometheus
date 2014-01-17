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

void PMotorSpeed::setProportionalGain(int motorNumber, float newproportionalgain)
{
	motorNumber = constrain(motorNumber, 0, 5);
	newproportionalgain = constrain(newproportionalgain, 0.0, 5.0);

	this->proportionalgain[motorNumber] = newproportionalgain;
}

int PMotorSpeed::calculate(int motorNumber, float currentAngleError)
{
	motorNumber = constrain(motorNumber, 0, 5);
	currentAngleError = constrain(currentAngleError, -360.0, 360.0);

	return currentAngleError * proportionalgain[motorNumber];
}

void PMotorSpeed::setSetPoint(int motorNumber, float setPoint)
{
	motorNumber = constrain(motorNumber, 0, 5);
	
	if(setPoint < 0.0)
	{
		setPoint = setPoint * -1.0;
	}

	setPoint = fmod(setPoint, 360.0);
	
	if(setPoint < min[motorNumber])
	{
		setPoint = min[motorNumber];
	}
	else if(setPoint > max[motorNumber])
	{
		setPoint = max[motorNumber];
	}

	this->setPoint[motorNumber] = setPoint;
}

float PMotorSpeed::getJointMin(int motorNumber)
{
	motorNumber = constrain(motorNumber, 0, 5);
	return min[motorNumber];
}

float PMotorSpeed::getJointMax(int motorNumber)
{
	motorNumber = constrain(motorNumber, 0, 5);
	return max[motorNumber];
}
