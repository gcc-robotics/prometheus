#import <Arduino.h>

#include "PIDMotorSpeed.h"

PIDMotorSpeed::PIDMotorSpeed() : PIMotorSpeed()
{

	for(int i = 0; i < this->numMotors; i++)
	{
		this->DTerm[i] = 0.0;
	}

	this->derivativeGain[0] = 0.0;
	this->derivativeGain[1] = 0.0;
	this->derivativeGain[2] = 0.0;
	this->derivativeGain[3] = 0.05;
	this->derivativeGain[4] = 0.0;

	for(int i = 0; i < this->numMotors; i++)
	{
		this->previousTime[i] = 0;
	}

	for(int i = 0; i < this->numMotors; i++)
	{
		this->previousSetPoint[i] = 0.0;
	}

	for(int i = 0; i < this->numMotors; i++)
	{
		this->motorSpeed[i] = 0.0;
	}

	this->sampleTime = 100;
}

int PIMotorSpeed::calculate(int jointNumber, double currentAngle)
{
	unsigned long now = millis();

	

	return this->motorSpeed[jointNumber];
}

void PIMotorSpeed::setDerivativeGain(int jointNumber, double newderivativegain)
{
	jointNumber = constrain(jointNumber, 0, 5);
	newderivativegain = constrain(newderivativegain, 0.0, 5.0);

	double sampleTimeSeconds = ((double)this->sampleTime) / 1000.0;

	this->derivativeGain[jointNumber] = newderivativegain / sampleTimeSeconds;
}

double PIMotorSpeed::getDerivativeGain(int jointNumber)
{
	double sampleTimeSeconds = ((double)this->sampleTime) / 1000.0;

	return derivativeGain[jointNumber] / sampleTimeSeconds;
}