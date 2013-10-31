#include "multiplexer.h"
#include "motorController.h"
#include "robotArm.h"

#ifndef DEBUGGER_H
#define DEBUGGER_H

class Debugger 
{
	private:
		// Tracking the state of the encoder
		int debuggerState;

		// Joint number
		int debugJointNumber;

		// debugInputNumber
		int debugInputNumber;

		// The joint test angle
		float debugAngle;

		// Internal reference to the mux
		Multiplexer* mux;

		// Internal reference to the motorController
		MotorController* motor;

		// Internal reference to the RobotArm
		RobotArm* arm;
		
		// -----------------------------------------------
		// Debugger states -------------------------------
		// -----------------------------------------------

		// Print the values of all the arduino inputs
		void printDigitalInputs(int userInput);

		// Get the joint number from the user
		void getJointNumber(int userInput);

		// Set the angle for the selected joint
		void setJointAngle(int userInput);

		// Read a mux input
		void readMuxInput(int userInput);

		// Monitor the encoder on mux input 0
		void monitorEncoderAngle(int userInput);

		// Monitor a potentiometer value
		void monitorPotentiometerValue(int userInput);

		// Let the user change the PID gains and monitor the joint
		void tuneJointPid(int userInput);

	public:
		// Constructor
		Debugger();

		void setup(Multiplexer* multiplexer, MotorController* motorController, RobotArm* robotArm);

		// Loop function for the debugger
		// Call in the arduino loop function
		void loop();
};

#endif
