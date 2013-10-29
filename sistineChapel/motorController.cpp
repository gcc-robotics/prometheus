#import <Arduino.h>
#include "motorController.h"

MotorController::MotorController()
{
	// Setup pwm
	this->pwm = Adafruit_PWMServoDriver();
}

void MotorController::setup()
{
	this->pwm.begin();

	// Set the frequency for the pwm driver
	this->pwm.setPWMFreq(60);
}

void MotorController::setPwm(int motorNum, float dutyCycle)
{
	// Clamp duty cycle between 0 to 100
	if(dutyCycle > 100)
	{
		dutyCycle = 100;
	}

	if(dutyCycle < 0)
	{
		dutyCycle = 0;
	}

	// selects the third pin on the pwm driver
	int pin = 3 * motorNum + 2;

	//calculates the integer value for the desired duty cycle
	float rate = dutyCycle * 40.95;

	this->pwm.setPWM(pin, 0, int(rate));
}

void MotorController::forward(int motorNum)
{
	int pin = 3 * motorNum;	//selects the first pin on the pwm driver

	this->pwm.setPWM(pin, 4095, 0);	//Sets the INA pin of the motor to LOW
	this->pwm.setPWM(pin + 1, 0, 4095);	//Sets the INB pin of the motor to High
}

void MotorController::reverse(int motorNum)
{
	int pin = 3 * motorNum;	//selects the first pin on the pwm driver

	this->pwm.setPWM(pin, 0, 4095);	//Sets the INA pin of the motor to HIGH
	this->pwm.setPWM(pin + 1, 4095, 0);	//Sets the INB pin of the motor to LOW
}

void MotorController::speed(int motorNumber, int speed)
{
	if(speed > 0)
	{
		this->forward(motorNumber);
		this->setPwm(motorNumber, speed);
	}
	else if(speed < 0)
	{
		speed *= -1;

		this->reverse(motorNumber);
		this->setPwm(motorNumber, speed);
	}
	else
	{
		this->setPwm(motorNumber, 0);
	}
}

void MotorController::coast(int motorNumber)
{
	int pin = 3 * motorNumber; //selects the first pin on the pwm driver

	this->pwm.setPWM(pin, 4095, 0); //Sets the INA pin of the motor to Low
	this->pwm.setPWM(pin + 1, 4095, 0); //Sets the INB pin of the motor to Low
}

void MotorController::brake(int motorNum)
{
	int pin = 3 * motorNum;	//selects the first pin on the pwm driver
	
	this->pwm.setPWM(pin, 0, 4095);	//Sets the INA pin of the motor to High
	this->pwm.setPWM(pin + 1, 0, 4095);	//Sets the INB pin of the motor to High
}
