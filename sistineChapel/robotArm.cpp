#import <Arduino.h>
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

bool RobotArm::moveMotorToEncoderAngle(int motorNumber, int encoderNumber, float targetAngle)
{
	// Limit target angle between 0 and 360
	if(targetAngle < 0)
	{
		targetAngle = 0;
	}

	targetAngle %= 360;

	// Shift target angle to -180 to 180
	targetAngle -= 180;
	
	// Get current angle and shift it to between -180 and 180
	float currentAngle = this->mux.readEncoder(encoderNumber) - 180;

	// Get the smallest angle error
	// Test the logic at: http://jsfiddle.net/frLFG/3/
	float angleError = targetAngle - currentAngle;

	if(angleError > 180)
	{
		angleError -= 360;
	}
	else if(angleError < -180)
	{
		angleError += 360;
	}

	angleError %= 360;

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

void RobotArm::elbow(float targetAngle)
{
	if(targetAngle < 0)
	{
		targetAngle = 0;
	}

	targetAngle %= 360;

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
