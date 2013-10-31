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
	// PID Parameters
	const int numMotors = 5;

	// Previous error
	for(int i = 0; i < numMotors; i++)
	{
		this->previousError[i] = 0.0;
	}

	// lastPidTime
	for(int i = 0; i < numMotors; i++)
	{
		this->lastPidTime[i] = 0.0;
	}
	
	// setPoint
	for(int i = 0; i < numMotors; i++)
	{
		this->setPoint[i] = 0.0;
		this->setJointAngle(i, 0);
	}

	// integralTerm
	for(int i = 0; i < numMotors; i++)
	{
		this->integralTerm[i] = 0.0;
	}

	// proprotionalGain
	for(int i = 0; i < numMotors; i++)
	{
		this->proprotionalGain[i] = 0.75;
	}

	// integralGain
	for(int i = 0; i < numMotors; i++)
	{
		this->integralGain[i] = 0.0;
	}

	// derivativeGain
	for(int i = 0; i < numMotors; i++)
	{
		this->derivativeGain[i] = 0.0;
	}
	
	// Motor numbers
	this->motorNumber[0] = 0;
	this->motorNumber[1] = 0;
	this->motorNumber[2] = 0;
	this->motorNumber[3] = 4;
	this->motorNumber[4] = 0;

	// Encoder numbers
	this->encoderNumber[0] = 0;
	this->encoderNumber[1] = 0;
	this->encoderNumber[2] = 1;
	this->encoderNumber[3] = 0;
	this->encoderNumber[4] = 0;

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

// Get the last angle error for the joint
float RobotArm::getLastError(int jointNumber)
{
	jointNumber = this->clamp(jointNumber, 0, 4);

	return this->previousError[jointNumber];
}

// Get the last pid time for the joint
int RobotArm::getLastPidTime(int jointNumber)
{
	jointNumber = this->clamp(jointNumber, 0, 4);

	return this->lastPidTime[jointNumber];
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

// Set the PID Gains for the joint
void RobotArm::setPidGains(int jointNumber, float PGain, float IGain, float DGain)
{
	// Clamp the gains
	PGain = this->clamp(PGain);
	IGain = this->clamp(IGain);
	DGain = this->clamp(DGain);

	// Set the gains
	this->proprotionalGain[jointNumber] = PGain;
	this->integralGain[jointNumber] = IGain;
	this->derivativeGain[jointNumber] = DGain;
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

float RobotArm::calculateMotorSpeed(int jointNumber, float angleError)
{
	//Serial.print("Joint number: ");
	//Serial.print(jointNumber);

	//Serial.print(" setPoint: ");
	//Serial.print(this->setPoint[jointNumber]);

	// Calculate the Proportional Term
	float proportionalTerm = abs(angleError) * this->proprotionalGain[jointNumber];

	//Serial.print(" PTerm: ");
	//Serial.print(proportionalTerm);

	// Get the time interval
	int timeNow = millis();
	float timeDifference = (timeNow - this->lastPidTime[jointNumber]) / 1000.0;

	//Serial.print(" timeDifference: ");
	//Serial.print(timeDifference);

	// Store the current time as previous
	this->lastPidTime[jointNumber] = timeNow;

	// Calculate the Integral Term
	this->integralTerm[jointNumber] += timeDifference * abs(angleError) * this->integralGain[jointNumber];

	// Zero out the integralTerm when the angle error is small
	if(angleError < 0.5)
	{
		this->integralTerm[jointNumber] = 0;
	}

	//Serial.print(" integralTerm: ");
	//Serial.print(this->integralTerm[jointNumber]);

	// Calculate the Derivate Term
	float derivativeTerm = (abs(this->previousError[jointNumber]) - abs(angleError)) * this->derivativeGain[jointNumber] / timeDifference;

	//Serial.print(" derivativeTerm: ");
	//Serial.print(derivativeTerm);

	// Store current angle error as previous angle error
	this->previousError[jointNumber] = angleError;

	//Serial.print(" previousError: ");
	//Serial.print(this->getLastError(jointNumber));

	float motorSpeed = proportionalTerm + this->integralTerm[jointNumber] + derivativeTerm;

	if(motorSpeed > 100)
	{
		motorSpeed = 100;
	}
	else if(motorSpeed < 0)
	{
		motorSpeed = 0;
	}

	//Serial.print(" motorSpeed: ");
	//Serial.print(motorSpeed);

	return motorSpeed;
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
	int motorSpeed = this->calculateMotorSpeed(jointNumber, angleError);

	//Serial.print(" currentAngle: ");
	//Serial.println(currentAngle);

	// Move in the right direction
	if(angleError > 0)
	{
		this->motor.speed(this->motorNumber[jointNumber], motorSpeed);
	}
	else
	{
		this->motor.speed(this->motorNumber[jointNumber], -motorSpeed);
	}
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

	// Per joint angle limits
	switch(jointNumber)
	{
		// Elbow angle limits
		case 2:
			if(angle < 121)
			{
				angle = 121;
			}
			else if(angle > 221)
			{
				angle = 221;
			}
			break;

		default:
			// Do nothing
			break;
	}

	angle = fmod(angle, 360);

	this->setPoint[jointNumber] = angle;
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
	// Elbow
	this->moveJointToSetPoint(2);

	// Wrist
	this->moveJointToSetPoint(3);
}
