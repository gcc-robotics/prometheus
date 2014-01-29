#import <Arduino.h>

#include "PIMotorSpeed.h"

PIMotorSpeed::PIMotorSpeed() : PMotorSpeed()
{

	for(int i = 0; i < this->numMotors; i++)
	{
		this->previousMotorSpeed[i] = 0;
	}

	this->integralGain[0] = 1.0;
	this->integralGain[1] = 1.0;
	this->integralGain[2] = 1.0;
	this->integralGain[3] = 1.0;
	this->integralGain[4] = 1.0;

	this->previousTime = 0;
}

int PIMotorSpeed::calculate(int jointNumber, float currentAngle)
{
	int motorSpeed = 0;
	float sum = 0.0;

	if(millis() - this->previousTime > 50)
	{
		float angleError = this->getAngleError(jointNumber, currentAngle);

		sum = PMotorSpeed::calculate(jointNumber, currentAngle) - this->integralGain[jointNumber] * angleError + this->previousMotorSpeed[jointNumber];

		motorSpeed = this->previousMotorSpeed[jointNumber];

		previousMotorSpeed[jointNumber] = sum;
		this->previousTime = millis();
	}
	else
	{
		motorSpeed = previousMotorSpeed[jointNumber];
	}

	return motorSpeed;
}

void PIMotorSpeed::setIntegralGain(int jointNumber, float newintegralgain)
{
	jointNumber = constrain(jointNumber, 0, 5);
	newintegralgain = constrain(newintegralgain, 0.0, 5.0);

	this->integralGain[jointNumber] = newintegralgain;
}

float PIMotorSpeed::getIntegralGain(int jointNumber)
{
	return integralGain[jointNumber];
}