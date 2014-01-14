#import <Arduino.h>
#include <math.h>
#include "PMotorSpeed.h"

PMotorSpeed::PMotorSpeed()
{
	for(int i = 0; i < this->numMotors; i++)
	{
		this->setPoint[i] = 0.0;
		this->setJointAngle(i, 0);
	}

	min[0] = 0;
	min[1] = 0;
	min[2] = 0;
	min[3] = 0;
	min[4] = 0;

	max[0] = 359;
	max[1] = 359;
	max[2] = 359;
	max[3] = 359;
	max[4] = 359;
}

int PMotorSpeed::calculate(float currentAngleError)
{

}

void PMotorSpeed::setPoint(int motorNumber, float setPoint)
{

}

//need to constraint motorNumber
float PMotorSpeed::getJointMin(int motorNumber)
{
	return min[motorNumber];
}

//need to constraint motorNumber
float PMotorSpeed::getJointMax(int motorNumber)
{
	return max[motorNumber]
}