#import <Arduino.h>
#include <math.h>
#include "robotArm.h"

RobotArm::RobotArm()
{
	/*	Joint numbers:
		0: waist
		1: shoulder
		2: elbow
		3: wrist
		4: hand
	*/
}

void RobotArm::setup()
{
	const int numMotors = 5;
	
	// setPoint
	for(int i = 0; i < numMotors; i++)
	{
		this->setPoint[i] = 0.0;
		this->setJointAngle(i, 0);
	}

	// Minimum angles for each joints
	min[0] = 0;
	min[1] = 0;
	min[2] = 0;
	min[3] = 0;
	min[4] = 0;

	// Maximum angles for each joints
	max[0] = 359;
	max[1] = 359;
	max[2] = 359;
	max[3] = 359;
	max[4] = 359;
	
	// Motor numbers
	this->motorNumber[0] = 0;
	this->motorNumber[1] = 1;
	this->motorNumber[2] = 2;
	this->motorNumber[3] = 3;
	this->motorNumber[4] = 4;

	// Encoder numbers
	this->encoderNumber[0] = 0;
	this->encoderNumber[1] = 1;
	this->encoderNumber[2] = 2;
	this->encoderNumber[3] = 3;
	this->encoderNumber[4] = 4;

	this->mux = Multiplexer();
	this->motor = MotorController();

	this->motor.setup();
}

Multiplexer* RobotArm::getMultiplexer()
{
	return &(this->mux);
}

MotorController* RobotArm::getMotorController()
{
	return &(this->motor);
}

// Set the motor speed for the provided joint and 
// do speed boosts for speeds below the boost threshold
void RobotArm::setJointMotorSpeed(int jointNumber, int speed)
{
	// Clamp input
	jointNumber = this->clamp(jointNumber, 0, 4);
	speed = this->clamp(speed, -100, 100);

	// Set motor speed
	this->motor.speed(this->motorNumber[jointNumber], speed);
}

// Clamps the input between min and max
float RobotArm::clamp(float input, float min, float max)
{
	if(input < min)
	{
		input = min;
	}

	if(input > max)
	{
		input = max;
	}

	return input;
}

// Clamps the input between min and max
int RobotArm::clamp(int input, int min, int max)
{
	if(input < min)
	{
		input = min;
	}

	if(input > max)
	{
		input = max;
	}

	return input;
}

float RobotArm::getAngleError(float targetAngle, float currentAngle)
{
	// Limit target angle between 0 and 360
	if(targetAngle < 0)
	{
		targetAngle = 0;
	}

	targetAngle = fmod(targetAngle, 360);

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

bool RobotArm::moveJointToSetPoint(int jointNumber)
{
	// Get the current angle
	float currentAngle = this->mux.readEncoder(this->encoderNumber[jointNumber]);

	// Get the angle error
	float angleError = this->getAngleError(this->setPoint[jointNumber], currentAngle);

	//Serial.print("Angle Error: ");
	//Serial.print(angleError);

	// Get the motor speed
	int motorSpeed = angleError;

	//Serial.print(" currentAngle: ");
	//Serial.println(currentAngle);

	// Set motor speed
	this->setJointMotorSpeed(jointNumber, motorSpeed);

}

void RobotArm::setJointAngle(int jointNumber, float angle)
{
	if(jointNumber < 0 || jointNumber > 5)
	{
		return;
	}

	if(angle < 0)
	{
		angle = 0;
	}

	angle = fmod(angle, 360);

	this->setPoint[jointNumber] = angle;
}

int RobotArm::getJointAngle(int jointNumber)
{
	return this->mux.readEncoder(this->encoderNumber[jointNumber]);
}

int RobotArm::getJointMinimum(int jointNumber)
{
	return min[jointNumber];
}

int RobotArm::getJointMaximum(int jointNumber)
{
	return max[jointNumber];
}

void RobotArm::waist(float targetAngle)
{
	this->setJointAngle(0, targetAngle);
}

void RobotArm::shoulder(float targetAngle)
{
	this->setJointAngle(1, targetAngle);
}

void RobotArm::elbow(float targetAngle)
{
	this->setJointAngle(2, targetAngle);
}

void RobotArm::wrist(float targetAngle)
{
	this->setJointAngle(3, targetAngle);
}

void RobotArm::hand(float targetAngle)
{
	this->setJointAngle(4, targetAngle);
}

void RobotArm::loop()
{
	// Waist
	this->moveJointToSetPoint(0);

	// Shoulder
	this->moveJointToSetPoint(1);

	// Elbow
	this->moveJointToSetPoint(2);

	// Wrist
	this->moveJointToSetPoint(3);

	// Hand
	this->moveJointToSetPoint(4);
}
