#import <Arduino.h>
#include <math.h>
#include "PMotorSpeed.h"

PMotorSpeed::PMotorSpeed()
{
	proportionalGain[0] = 1.0;
	proportionalGain[1] = 1.0;
	proportionalGain[2] = 1.0;
	proportionalGain[3] = 1.0;
	proportionalGain[4] = 1.0;

	min[0] = 20.0;
	min[1] = 3.0;
	min[2] = 7.0;
	min[3] = 20.0;
	min[4] = 0.0;

	max[0] = 340.0;
	max[1] = 167.0;
	max[2] = 175.0;
	max[3] = 300.0;
	max[4] = 359.9;

	this->numMotors = 5;

	for(int i = 0; i < this->numMotors; i++)
	{
		this->setPoint[i] = (min[i] + max[i]) / 2;
		this->setSetPoint(i, this->setPoint[i]);
	}
}

void PMotorSpeed::setProportionalGain(int jointNumber, double newproportionalgain)
{
	jointNumber = constrain(jointNumber, 0, 5);
	newproportionalgain = constrain(newproportionalgain, 0.0, 5.0);

	this->proportionalGain[jointNumber] = newproportionalgain;
}

double PMotorSpeed::getProportionalGain(int jointNumber)
{
	return proportionalGain[jointNumber];
}

int PMotorSpeed::calculate(int jointNumber, double currentAngle)
{
	jointNumber = constrain(jointNumber, 0, 5);
	currentAngle = constrain(currentAngle, -360.0, 360.0);

	double currentAngleError = this->getAngleError(jointNumber, currentAngle);

	return currentAngleError * proportionalGain[jointNumber];
}

void PMotorSpeed::setSetPoint(int jointNumber, double setPoint)
{
	jointNumber = constrain(jointNumber, 0, 5);
	
	while(setPoint < 0.0)
	{
		setPoint += 360;
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

double PMotorSpeed::getJointMin(int jointNumber)
{
	jointNumber = constrain(jointNumber, 0, 5);
	return min[jointNumber];
}

double PMotorSpeed::getJointMax(int jointNumber)
{
	jointNumber = constrain(jointNumber, 0, 5);
	return max[jointNumber];
}

double PMotorSpeed::getAngleError(int jointNumber, double currentAngle)
{

	double targetAngle = this->setPoint[jointNumber];

	double angleError;

	angleError = targetAngle - currentAngle;

	return angleError;
	/*
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
	double angleError = targetAngle - currentAngle;

	if(angleError > 180)
	{
		angleError -= 360.0;
	}
	else if(angleError < -180)
	{
		angleError += 360.0;
	}

	return fmod(angleError, 360);*/
}
