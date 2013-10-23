#import <Arduino.h>
#include <math.h>
#include "robotArm.h"

RobotArm::RobotArm()
{
	// Set the deadZoneAngle
	this->deadZoneAngle = 5.0;

	// Init elbow parameters
	this->elbowMotorNumber = 0;
	this->elbowEncoderNumber = 0;
	this->elbowTargetAngle = 0.0;
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

bool RobotArm::moveMotorToEncoderAngle(int motorNumber, int encoderNumber, double targetAngle)
{
	// Limit target angle between 0 and 360
	if(targetAngle < 0)
	{
		targetAngle = 0;
	}

	targetAngle = fmod(targetAngle, 360);

	// Shift target angle to -180 to 180
	targetAngle -= 180.0;
	
	// Get current angle and shift it to between -180 and 180
	double currentAngle = this->mux.readEncoder(encoderNumber) - 180;

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

	angleError = fmod(angleError, 360);

	// Debug
	Serial.print("Current angle error: ");
	Serial.print(angleError);

	// Only move the joint if the angleError is greater than the deadZoneAngle
	if(abs(angleError) > this->deadZoneAngle)
	{
		// There is angleError, we need to move the joint

		if(angleError > 0)
		{
			this->motor.speed(motorNumber, 10);
		}
		else
		{
			this->motor.speed(motorNumber, -10);
		}
	}
	else
	{
		// The angleError is small, stop the joint
		this->motor.speed(motorNumber, 0);
		this->motor.brake(motorNumber);

		return true;
	}
}

void RobotArm::elbow(double targetAngle)
{
	if(targetAngle < 0)
	{
		targetAngle = 0;
	}

	targetAngle = fmod(targetAngle, 360);

	this->elbowTargetAngle = targetAngle;
}

void RobotArm::loop()
{
	this->moveMotorToEncoderAngle(this->elbowMotorNumber, this->elbowEncoderNumber, this->elbowTargetAngle);

	Serial.print(" Current elbow angle: ");
	Serial.print(this->mux.readEncoder(this->elbowEncoderNumber));
	Serial.print(" target angle: ");
	Serial.println(this->elbowTargetAngle);
}
