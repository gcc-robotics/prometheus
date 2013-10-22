#include "multiplexer.h"
#include "motorController.h"

#ifndef DEBUGGER_H
#define DEBUGGER_H

class Debugger 
{
	private:
		// Tracking the state of the encoder
		int debuggerState;

		// Motor number for setMotorSpeed()
		int debugMotorNumber;

		// Internal reference to the mux
		Multiplexer* mux;

		// Internal reference to the motorController
		MotorController* motor;
		
		// -----------------------------------------------
		// Debugger states -------------------------------
		// -----------------------------------------------

		// Print the values of all the arduino inputs
		void printDigitalInputs(int userInput);

		// Get the motor number from the user
		void getMotorNumber(int userInput);

		// Set the duty cycle for a motor
		void setMotorSpeed(int userInput);

		// Read a mux input
		void readMuxInput(int userInput);

		// Monitor the encoder on mux input 0 for 30 seconds
		void monitorEncoderAngle(int userInput);

	public:
		// Constructor
		// multiplexer = a pointer to a Multiplexer object
		Debugger(Multiplexer* multiplexer, MotorController* motorController);

		// Loop function for the debugger
		// Call in the arduino loop function
		void loop();
};

#endif
