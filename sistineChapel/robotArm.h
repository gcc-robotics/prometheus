#include "motorController.h"
#include "multiplexer.h"

#ifndef ROBOTARM_H
#define ROBOTARM_H

class RobotArm 
{
	private:
		// deadZoneAngle used for deciding how close the joints need to be to 
		// their target angle to be considered in the correct position
		double deadZoneAngle;

		// Elbow parameters
		int elbowMotorNumber;
		int elbowEncoderNumber;
		double elbowTargetAngle;

		// Instances of the MotorController and Multiplexer
		MotorController motor;
		Multiplexer mux;

		// Contains the logic to actually move a joint to the desired angle
		bool moveMotorToEncoderAngle(int motorNumber, int encoderNumber, double angle);
	public:	
		// Constructor
		RobotArm();

		Multiplexer* getMultiplexer();
		MotorController* getMotorController();

		// Setup the arm for use
		void setup();

		// Move the elbow the the provided angle
		void elbow(double degrees);

		// Run in arduino loop()
		void loop();
};

#endif