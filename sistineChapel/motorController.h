#pragma once

class MotorController
{
	private:
		//current motor number 1 - 5
		int myMotorNum;
		//current dutycycle 0 - 100
		float myDutyCycle;
		
	public:
		//contructor
		//creates motorController object; does not have specified motor number, which is set to 0
		//duty cycle is set to 0
		motorController();
		//contructor
		//creates motorController object with specified motor number
		//duty cycle is set to 0
		motorController(int motorNum);
		//sets motorController specified motor number
		void setMotor(int motorNum);
		//returns motor number
		int getMotor();
		//sets motorControoler specified dutycycle
		void setDutyCycle(float dutyCycle);
		//returns duty cycle
		float getDutyCycle();
		//sets the PWM duty cycle for the desired motor
		void setPwm(int motorNum, float dutyCycle);
		// On Call Motor is set to move Forward.
		void forward(int motorNum);
		// On Call Motor is set to move Reverse
		void reverse(int motorNum);
		// On call motor is reset, all pins set to HIGH. Motor is Stopped.
		void brake(int motorNum);
};