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
	this->motorSpeed = PIMotorSpeed();

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

bool RobotArm::moveJointToSetPoint(int jointNumber)
{
	// Get the current angle
	float currentAngle = this->mux.readEncoder(this->encoderNumber[jointNumber]);

	//Serial.print("Angle Error: ");
	//Serial.print(angleError);

	// Get the motor speed
	int motorSpeed = this->motorSpeed.calculate(jointNumber, currentAngle);

	//Serial.print(" currentAngle: ");
	//Serial.println(currentAngle);
	this->motor.speed(jointNumber, motorSpeed);
}

void RobotArm::setJointAngle(int jointNumber, float angle)
{
	this->motorSpeed.setSetPoint(jointNumber, angle);
}

int RobotArm::getJointAngle(int jointNumber)
{
	return this->mux.readEncoder(this->encoderNumber[jointNumber]);
}

float RobotArm::getJointMinimum(int jointNumber)
{
	return this->motorSpeed.getJointMin(jointNumber);
}

float RobotArm::getJointMaximum(int jointNumber)
{
	return this->motorSpeed.getJointMax(jointNumber);
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
