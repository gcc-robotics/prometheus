#import <Arduino.h>

#include "PIMotorSpeed.h"

PIMotorSpeed::PIMotorSpeed() : PMotorSpeed()
{

	for(int i = 0; i < this->numMotors; i++)
	{
		this->ITerm[i] = 0.0;
	}

	this->integralGain[0] = 0.0;
	this->integralGain[1] = 0.0;
	this->integralGain[2] = 0.0;
	this->integralGain[3] = 0.05;
	this->integralGain[4] = 0.0;

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

	// Serial.print("Moving Joint: ");
	// Serial.print(jointNumber);

	if(now - this->previousTime[jointNumber] > this->sampleTime)
	{
		double angleError = this->getAngleError(jointNumber, currentAngle);

		this->ITerm[jointNumber] += this->integralGain[jointNumber] * angleError;

		this->ITerm[jointNumber] = (this->ITerm[jointNumber] > 100) ? 100 : this->ITerm[jointNumber];
		this->ITerm[jointNumber] = (this->ITerm[jointNumber] < -100) ? -100 : this->ITerm[jointNumber];

		double inputDifference = this->setPoint[jointNumber] - this->previousSetPoint[jointNumber];

		DTerm[jointNumber] = inputDifference;

		this->motorSpeed[jointNumber] = PMotorSpeed::calculate(jointNumber, currentAngle) + this->ITerm[jointNumber] - this->derivativeGain[jointNumber] * this->DTerm[jointNumber];

		this->motorSpeed[jointNumber] = (this->motorSpeed[jointNumber] > 100) ? 100 : this->motorSpeed[jointNumber];
		this->motorSpeed[jointNumber] = (this->motorSpeed[jointNumber] < -100) ? -100 : this->motorSpeed[jointNumber];

		this->previousSetPoint[jointNumber] = this->setPoint[jointNumber];
		this->previousTime[jointNumber] = now;
	}

	// Serial.print(", previousTime: ");
	// Serial.print(this->previousTime[jointNumber]);

	// Serial.print(", motorSpeed: ");
	// Serial.print(this->motorSpeed[jointNumber]);

	// Serial.print(", error: ");
	// Serial.println(this->getAngleError(jointNumber, currentAngle));

	return this->motorSpeed[jointNumber];
}

void PIMotorSpeed::setIntegralGain(int jointNumber, double newintegralgain)
{
	jointNumber = constrain(jointNumber, 0, 5);
	newintegralgain = constrain(newintegralgain, 0.0, 5.0);

	double sampleTimeSeconds = ((double)this->sampleTime) / 1000.0;

	this->integralGain[jointNumber] = newintegralgain * sampleTimeSeconds;
}

double PIMotorSpeed::getIntegralGain(int jointNumber)
{
	double sampleTimeSeconds = ((double)this->sampleTime) / 1000.0;

	return integralGain[jointNumber] / sampleTimeSeconds;
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