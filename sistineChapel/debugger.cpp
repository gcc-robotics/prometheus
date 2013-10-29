#include <Arduino.h>
#include "debugger.h"
#include <math.h>

// Debugger constructor
Debugger::Debugger()
{
	this->debugPotNumber = 6;
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
				// User wants to set the base angle
				Serial.print("[NOT IMPLEMENTED]");
				break;

			case 2:
				// User wants to set the shoulder angle
				Serial.print("[NOT IMPLEMENTED]");
				break;

			case 3:
				// User wants to set the elbow angle
				this->arm->elbow(userInput);
				Serial.print("elbow");
				break;

			case 4:
				// User wants to set the wrist angle
				Serial.print("[NOT IMPLEMENTED]");
				break;

			case 5:
				// User wants to set the hand angle
				Serial.print("[NOT IMPLEMENTED]");
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
	float angle = this->mux->readEncoder(0);

	// Print the angle
	Serial.print("Current angle: ");
	Serial.println(angle);

	// Check if the user wants to stop the monitoring
	if(userInput == -1)
	{
		// Return back to the default state
		this->debuggerState = 0;
	}
}

// Monitor a potentiometer value
void Debugger::monitorPotentiometerValue(int userInput)
{
	// Get the value of the pot on mux input 6
	float potValue = this->mux->readPotentiometer(this->debugPotNumber);

	// Print the potValue
	Serial.print("-1 to stop, 6, 7, 8 to select pot, Pot ");
	Serial.print(this->debugPotNumber);
	Serial.print(" value: ");
	Serial.println(potValue);

	if(userInput == 6 || userInput == 7 || userInput == 8)
	{
		// User wants to change pot number
		this->debugPotNumber = userInput;
	}
	// The user wants to stop the monitoring
	else if(userInput == -1)
	{
		// Return back to the default state
		this->debuggerState = 0;
	}
}

// Let the user change the PID gains and monitor the joint
void Debugger::tuneJointPid(int userInput)
{
	// Get the values of the pots
	float PGain = this->mux->readPotentiometer(6);
	float IGain = this->mux->readPotentiometer(7);
	float DGain = this->mux->readPotentiometer(8);

	// Print the potValue
	Serial.print("-1 to stop, 1 through 5 to select joint, 6 for random angle, Button on board to set the gains, Joint: ");
	Serial.print(this->debugJointNumber);
	Serial.print(" P: ");
	Serial.print(PGain);
	Serial.print(" I: ");
	Serial.print(IGain);
	Serial.print(" D: ");
	Serial.print(DGain);
	Serial.print(" Error: ");
	Serial.println(this->arm->getLastError(this->debugJointNumber - 1));

	// Check if the button is pressed
	if(digitalRead(10) == HIGH)
	{
		// Set the gains for the current joint
		this->arm->setPidGains(this->debugJointNumber - 1, PGain, IGain, DGain);
		Serial.print("Setting PID gains for joint ");
		Serial.print(this->debugJointNumber);
		Serial.println(".");
	}

	if(userInput >= 1 && userInput <= 5)
	{
		// User wants to change joint number
		this->debugJointNumber = userInput;
	}
	else if(userInput == 6)
	{
		this->debugAngle = fmod(this->debugAngle + 180, 361);

		this->arm->setJointAngle(this->debugJointNumber - 1, this->debugAngle);
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

		case 6:
			this->tuneJointPid(userInput);
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
			// Check if the user wants to tune PIDs
			else if(userInput == 4)
			{
				this->debuggerState = 6;
			}
			// Print the menu message
			else
			{
				Serial.println("Input 1 to show the status of the digital input, 2 to set Joint Angle, 3 to select mux input, 4 for PID tunning.");
			}
			break;
	}
}
