#include <Arduino.h>
#include "debugger.h"
#include <math.h>

// Debugger constructor
Debugger::Debugger()
{
	this->debugInputNumber = 6;
	this->debugJointNumber = 3;
	this->debugAngle = 0.0;
}

// Call in the arduino setup() function
// and pass in a pointer to a Multiplexer 
// and MotorController objects
void Debugger::setup(Multiplexer* multiplexer, MotorController* motorController, RobotArm* robotArm)
{
	// Tracking the state of the encoder
	this->debuggerState = 0;

	// debugJointNumber
	this->debugJointNumber = 0;

	// Set the mux reference
	this->mux = multiplexer;

	// Set the motorController reference
	this->motor = motorController;

	// Set the robotArm reference
	this->arm = robotArm;
}

// Print the values of all the arduino inputs
void Debugger::printDigitalInputs(int userInput)
{
	// Print the status of the arduino digital inputs
	for(int i = 0; i < 8; i++)
	{
		Serial.print("Pin ");
		Serial.print(i);
		Serial.print(" value: ");
		Serial.println(digitalRead(i));
	}
}

void Debugger::getJointNumber(int userInput)
{
	Serial.println("Input Joint Number between 1 and 5, 3 for elbow, 4 for wrist.");

	if(userInput >= 1 && userInput <= 6)
	{
		// Store the motor number
		this->debugJointNumber = userInput;

		// Move to the next debugger state
		this->debuggerState = 2;
	}
}

// Set the duty cycle for a motor
void Debugger::setJointAngle(int userInput)
{
	Serial.println("Input joint angle in degrees.");

	if(userInput != 0)
	{
		Serial.print("Setting ");

		// Set the selected joint to the provided angle
		switch(this->debugJointNumber)
		{
			case 1:
				// User wants to set the waist angle
				this->arm->waist(userInput);
				Serial.print("waist");
				break;

			case 2:
				// User wants to set the shoulder angle
				this->arm->shoulder(userInput);
				Serial.print("shoulder");
				break;

			case 3:
				// User wants to set the elbow angle
				this->arm->elbow(userInput);
				Serial.print("elbow");
				break;

			case 4:
				// User wants to set the wrist angle
				this->arm->wrist(userInput);
				Serial.print("wrist");
				break;

			case 5:
				// User wants to set the hand angle
				this->arm->hand(userInput);
				Serial.print("hand");
				break;

			default:
				Serial.print("[NOT IMPLEMENTED]");
				break;
		}

		// Print the rest of the message
		Serial.print(" to ");
		Serial.print(userInput);
		Serial.println(" degrees");

		// Return back to the default state
		this->debuggerState = 0;
	}
}

// Read a mux input
void Debugger::readMuxInput(int userInput)
{
	Serial.println("Input mux input number between 1 and 16, 17 to monitor the encoder angle on mux input C0, 18 to monitor a pot.");

	if(userInput == 17)
	{
		// Move to the encoder monitoring state
		this->debuggerState = 4;
	}
	else if(userInput == 18)
	{
		// Move to the encoder monitoring state
		this->debuggerState = 5;
	}
	else if(userInput != 0)
	{
		// Print the value of the selected mux input
		Serial.print("Set mux to input");
		Serial.println(userInput);
		Serial.print("Current mux input value: ");
		Serial.println(this->mux->readInput(userInput - 1));

		// Return back to the default state
		this->debuggerState = 0;
	}
}

// Monitor the encoder on mux input 0
void Debugger::monitorEncoderAngle(int userInput)
{
	// Get the angle from the encoder on mux input 0
	float angle = this->mux->readEncoder(this->debugInputNumber);

	// Print the angle
	Serial.print("-1 to stop, 1 to 16 to select input, Current angle: ");
	Serial.println(angle);

	if(userInput > 0 && userInput <= 16)
	{
		this->debugInputNumber = userInput - 1;
	}
	// Check if the user wants to stop the monitoring
	else if(userInput == -1)
	{
		// Return back to the default state
		this->debuggerState = 0;
	}
}

// Monitor a potentiometer value
void Debugger::monitorPotentiometerValue(int userInput)
{
	// Get the value of the pot on mux input 6
	float potValue = this->mux->readPotentiometer(this->debugInputNumber);

	// Print the potValue
	Serial.print("-1 to stop, 6, 7, 8 to select pot, Pot ");
	Serial.print(this->debugInputNumber);
	Serial.print(" value: ");
	Serial.println(potValue);

	if(userInput == 6 || userInput == 7 || userInput == 8)
	{
		// User wants to change pot number
		this->debugInputNumber = userInput;
	}
	// The user wants to stop the monitoring
	else if(userInput == -1)
	{
		// Return back to the default state
		this->debuggerState = 0;
	}
}


// Loop function for the debugger
// Run in the arduino loop function
void Debugger::loop()
{
	int userInput = Serial.parseInt();

	switch(this->debuggerState)
	{
		// State waiting for the user to input a motor number
		case 1:
			this->getJointNumber(userInput);
			break;

		case 2:
			this->setJointAngle(userInput);
			break;

		case 3:
			this->readMuxInput(userInput);
			break;

		case 4:
			this->monitorEncoderAngle(userInput);
			break;

		case 5:
			this->monitorPotentiometerValue(userInput);
			break;

		// The default debugger state
		default:
		case 0:
			// Check if the user wants to print out the digital inputs
			if(userInput == 1)
			{
				this->printDigitalInputs(userInput);
			}
			// Check if the user wants to set a joint angle
			else if(userInput == 2)
			{
				this->debuggerState = 1;
			}
			// Check if the user wants to output a mux input
			else if(userInput == 3)
			{
				this->debuggerState = 3;
			}
			// Print the menu message
			else
			{
				Serial.println("Input 1 to show the status of the digital input, 2 to set Joint Angle, 3 to read mux inputs.");
			}
			break;
	}
}
