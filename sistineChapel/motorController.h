#include <Adafruit_PWMServoDriver.h>

#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class MotorController
{
	private:
		// Pwm object
		Adafruit_PWMServoDriver pwm;

		//sets the PWM duty cycle for the desired motor
		void setPwm(int motorNum, float dutyCycle);
		// On Call Motor is set to move Forward.
		void forward(int motorNum);
		// On Call Motor is set to move Reverse
		void reverse(int motorNum);

	public:
		//empty contructor that creates motorController.
		MotorController();
                void setup();
                
		//Sets specified motor to specified dutycycle, speed.
		//motorNumber = 1 to 5
		//speed = -100 to 100
		void speed(int motorNumber, int speed);
		//On call motor is reset, all pins set to Low. Motor is set to coast.
		void coast(int motorNumber);
		// On call motor is reset, all pins set to HIGH. Motor is Stopped.
		void brake(int motorNumber);
};

#endif
