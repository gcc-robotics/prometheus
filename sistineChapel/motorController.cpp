#import <Arduino.h>
#include "motorController.h"

MotorController :: motorController()
{
	myMotorNum = 0;
	myDutyCycle = 0.0;
}

MotorController :: motorController(int motorNum)
{
	myMotorNum = motorNum;
	myDutyCycle = 0.0;
}

void MotorController :: setMotor(int motorNum)
{
	myMotorNum = motorNum;
}

int MotorController :: getMotor()
{
	return myMotorNum;
}

void MotorController :: setDutyCycle(float dutyCycle)
{
	myDutyCycle = dutyCycle;
}

float MotorController :: getDutyCycle()
{
	return myDutyCycle;
}

void setPwm(int motorNum, float dutyCycle)
{
	if(dutyCycle > 100)
	{
		dutyCycle = 100;
	}
	if(dutyCycle < 0)
	{
		dutyCycle = 0;
	}
	int pin = 3 * (motorNum-1); 		//selects the first pin on the pwm driver
	float rate = dutyCycle * 40.95; 	//calculates the integer value for the desired duty cycle
	pwm.setPWM(pin + 2, 0, int(rate));
}

void forward(int motorNum)
{
	int pin = 3 * (motorNum - 1);	//selects the first pin on the pwm driver
    
	pwm.setPWM(pin, 4095, 0);	//Sets the INA pin of the motor to LOW
	pwm.setPWM(pin + 1, 0, 4095);	//Sets the INB pin of the motor to High
}

void reverse(int motorNum)
{
	int pin = 3 * (motorNum - 1);	//selects the first pin on the pwm driver
  
	pwm.setPWM(pin, 0, 4095);	//Sets the INA pin of the motor to HIGH
	pwm.setPWM(pin + 1, 4095, 0);	//Sets the INB pin of the motor to LOW
}

void brake(int motorNum)
{
	int pin = 3 * (motorNum - 1);	//selects the first pin on the pwm driver
  
	pwm.setPWM(pin, 0, 4095);	//Sets the INA pin of the motor to High
	pwm.setPWM(pin + 1, 0, 4095);	//Sets the INB pin of the motor to High
}