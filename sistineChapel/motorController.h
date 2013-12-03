#include <Adafruit_PWMServoDriver.h>

#ifndef MOTORCONTROLLER_H
#define MOTORCONTROLLER_H

class MotorController
{
	private:
		// Pwm object
		Adafruit_PWMServoDriver pwm;

		/* Sets the PWM duty cycle for the desired motor
		 * motorNum = 1 to 5
		 * dutyCycle = -100 to 100
		 * return void
		 */
		void setPwm(int motorNum, float dutyCycle);
		/* Sets specified motor to move forward.
		 * motorNum = 1 to 5
		 * return void
		 */
		void forward(int motorNum);
		/* Sets specified motor to move reverse.
		 * motorNum = 1 to 5
		 * return void
		 */
		void reverse(int motorNum);

	public:
		//empty contructor that creates motorController.
		MotorController();
        void setup();
                
		/* Sets specified motor to specified dutycycle, speed.
		 * motorNumber = 1 to 5
		 * speed = -100 to 100
		 * return void
		 */
		void speed(int motorNumber, int speed);
		/* Sets specified motor to coast.
		 * motorNumber = 1 to 5
		 * return void
		 */
		void coast(int motorNumber);
		/* Sets specified motor to brake.
		 * motorNumber = 1 to 5
		 * return void
		 */
		void brake(int motorNumber);
};

#endif
