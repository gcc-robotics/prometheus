#include "motorController.h"
#include "multiplexer.h"

#ifndef ROBOTARM_H
#define ROBOTARM_H

class RobotArm 
{
	private:
		// PID Parameters
		float previousError[5];
		float lastPidTime[5];
		float setPoint[5];
		float integralTerm[5];

		float proprotionalGain[5];
		float integralGain[5];
		float derivativeGain[5];
		
		// Input/Output numbers
		int motorNumber[5];
		int encoderNumber[5];

		// Instances of the MotorController and Multiplexer
		MotorController motor;
		Multiplexer mux;

		float getAngleError(float targetAngle, float currentAngle);
		float calculateMotorSpeed(int jointNumber, float angleError);

		// Contains the logic to actually move a joint to the desired angle
		bool moveJointToSetPoint(int jointNumber);

		// Clamps the input between min and max
		float clamp(float input, float min = 0.0, float max = 1.0);
		int clamp(int input, int min = 0, int max = 4);

	public:	
		// Constructor
		RobotArm();

		Multiplexer* getMultiplexer();
		MotorController* getMotorController();

		// Setup the arm for use
		void setup();

		// Get the last pid time for the joint
		int getLastPidTime(int inputNumber);

		// Get the last angle error for the joint
		float getLastError(int jointNumber);

		// Set the PID Gains for the joint
		void setPidGains(int jointNumber, float PGain, float IGain, float DGain);

		// Set joint setPoint
		void setJointAngle(int jointNumber, float angle);

		// Move the elbow the the provided angle
		void elbow(float degrees);

		// Run in arduino loop()
		void loop();
};

#endif
