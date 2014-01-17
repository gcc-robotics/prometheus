#import <Arduino.h>
#include <math.h>
#include "PMotorSpeed.h"
#include "helperFunction.cpp"

PMotorSpeed::PMotorSpeed()
{
	for(int i = 0; i < this->numMotors; i++)
	{
		this->setPoint[i] = 0.0;
		this->setPoint(i, 0);
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

void PMotorSpeed::setProportionalGain(int motorNumber, float proportionalgain)
{
	motorNumber = clamp(motorNumber, 0, 5);
	proportionalgain = clamp(proportionalgain, 0.0, 5.0);

	proportionalgain[motorNumber] = proportionalgain;
}

int PMotorSpeed::calculate(int motorNumber, float currentAngleError)
{
	motorNumber = clamp(motorNumber, 0, 5);
	currentAngleError = clamp(currentAngleError, -360.0, 360.0);

	return currentAngleError * proportionalgain[motorNumber];
}

void PMotorSpeed::setPoint(int motorNumber, float setPoint)
{
	motorNumber = clamp(motorNumber, 0, 5);
	
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
	motorNumber = clamp(motorNumber, 0, 5);
	return min[motorNumber];
}

float PMotorSpeed::getJointMax(int motorNumber)
{
	motorNumber = clamp(motorNumber, 0, 5);
	return max[motorNumber]
}