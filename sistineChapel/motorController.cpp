#import <Arduino.h>
#include "motorController.h"

MotorController::motorController()
{
}

void MotorController::setPwm(int motorNum, float dutyCycle)
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

void MotorController::forward(int motorNum)
{
	int pin = 3 * (motorNum - 1);	//selects the first pin on the pwm driver
    
	pwm.setPWM(pin, 4095, 0);	//Sets the INA pin of the motor to LOW
	pwm.setPWM(pin + 1, 0, 4095);	//Sets the INB pin of the motor to High
}

void MotorController::reverse(int motorNum)
{
	int pin = 3 * (motorNum - 1);	//selects the first pin on the pwm driver
  
	pwm.setPWM(pin, 0, 4095);	//Sets the INA pin of the motor to HIGH
	pwm.setPWM(pin + 1, 4095, 0);	//Sets the INB pin of the motor to LOW
}

void MotorController::speed(int motorNumber, int speed)
{
	if(speed > 0)
	{
		forward(motorNumber);
		setPwm(motorNumber, speed);
	}
	else if(speed < 0)
	{
		reverse(motorNumber);
		speed *= -1;
		setPwm(motorNumber, speed);
	}
	else
	{
		setPwm(motorNumber, 0);
	}
}

void MotorController::coast(int motorNumber)
{
	int pin = 3 * (motorNumber - 1); //selects the first pin on the pwm driver
  
	pwm.setPWM(pin, 4095, 0); //Sets the INA pin of the motor to Low
	pwm.setPWM(pin + 1, 4095, 0); //Sets the INB pin of the motor to Low
}

void MotorController::brake(int motorNum)
{
	int pin = 3 * (motorNum - 1);	//selects the first pin on the pwm driver
  
	pwm.setPWM(pin, 0, 4095);	//Sets the INA pin of the motor to High
	pwm.setPWM(pin + 1, 0, 4095);	//Sets the INB pin of the motor to High
} 