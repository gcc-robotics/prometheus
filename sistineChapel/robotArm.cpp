#import <Arduino.h>
#include <math.h>
#include "robotArm.h"

RobotArm::RobotArm()
{
}

void RobotArm::setup()
{
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
	if(jointNumber < 0 || jointNumber > 5)
	{
		return 0.0;
	}

	return this->previousError[jointNumber];
}

float RobotArm::clampGain(float gain, float minGain, float maxGain)
{
	if(gain < minGain)
	{
		gain = minGain;
	}

	if(gain > maxGain)
	{
		gain = maxGain;
	}

	return gain;
}

// Set the PID Gains for the joint
void RobotArm::setPidGains(int jointNumber, float PGain, float IGain, float DGain)
{
	// Clamp the gains
	PGain = this->clampGain(PGain);
	IGain = this->clampGain(IGain);
	DGain = this->clampGain(DGain);

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

	if(angleError < 0.5)
	{
		this->integralTerm[jointNumber] = 0;
	}

	//Serial.print(" integralTerm: ");
	//Serial.print(this->integralTerm[jointNumber]);

	// Calculate the Derivate Term
	float derivativeTerm = (abs(this->previousError[jointNumber]) - abs(angleError)) * (this->derivativeGain[jointNumber]) / timeDifference;

	//Serial.print(" derivativeTerm: ");
	//Serial.print(derivativeTerm);

	// Store current angle error as previous angle error
	this->previousError[jointNumber] = angleError;

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
	//Serial.println(motorSpeed);

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

	angle = fmod(angle, 360);

	this->setPoint[jointNumber] = angle;
}

void RobotArm::elbow(float targetAngle)
{
	this->setJointAngle(2, targetAngle);
}

void RobotArm::loop()
{
	this->moveJointToSetPoint(2);
}
