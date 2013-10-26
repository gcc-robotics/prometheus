#include "motorController.h"
#include "multiplexer.h"

#ifndef ROBOTARM_H
#define ROBOTARM_H

class RobotArm 
{
	private:
		// PID Parameters
		float previousError[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
		float lastPidTime[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
		float setPoint[5] = {0.0, 0.0, 0.0, 0.0, 0.0};
		float integralTerm[5] = {0.0, 0.0, 0.0, 0.0, 0.0};

		float proprotionalGain[5] = {1.0, 1.0, 0.16, 1.0, 1.0};
		float integralGain[5] = {0.5, 0.5, 0.01, 0.5, 0.5};
		float derivativeGain[5] = {0.5, 0.5, 0.02, 0.5, 0.5};
		
		// Input/Output numbers
		int motorNumber[5] = {0, 0, 0, 0, 0};
		int encoderNumber[5] = {0, 0, 0, 0, 0};

		// Instances of the MotorController and Multiplexer
		MotorController motor;
		Multiplexer mux;

		float getAngleError(float targetAngle, float currentAngle);
		float calculateMotorSpeed(int jointNumber, float angleError);

		// Contains the logic to actually move a joint to the desired angle
		bool moveJointToSetPoint(int jointNumber);

		// Clamps the gain between minGain and maxGain
		float clampGain(float gain, float minGain = 0.0, float maxGain = 1.0);

	public:	
		// Constructor
		RobotArm();

		Multiplexer* getMultiplexer();
		MotorController* getMotorController();

		// Setup the arm for use
		void setup();

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
