#include <Arduino.h>
#include "debugger.h"

// Debugger constructor, call in the arduino setup() function
// and pass in a pointer to a Multiplexer object
Debugger::Debugger(Multiplexer* multiplexer, MotorController* motorController)
{
	// Tracking the state of the encoder
	this->debuggerState = 0;

	// Motor number for setMotorSpeed()
	this->debugMotorNumber = 0;

	// Set the mux reference
	this->mux = multiplexer;

	// Set the motorController reference
	this->motor = motorController;
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

void Debugger::getMotorNumber(int userInput)
{
	Serial.println("Input Motor Number between 1 and 5");

	if(userInput != 0)
	{
		// Store the motor number
		this->debugMotorNumber = userInput;

		// Move to the next debugger state
		this->debuggerState = 2;
	}
}

// Set the duty cycle for a motor
void Debugger::setMotorSpeed(int userInput)
{
	Serial.println("Input motor speed between -100 and 100 excluding 0, -999 for stop");

	if(userInput != 0)
	{
		// Clamp the duty cycle between -100 and 100
		if(userInput < -100)
			userInput = -100;

		if(userInput > 100)
			userInput = 100;

		if(userInput == -999)
		{
			userInput = 0;
		}

		// Set the motor to go forward at the provided duty cycle
		this->motor->speed(this->debugMotorNumber, userInput);

		// Print a message
		Serial.print("Setting motor ");
		Serial.print(this->debugMotorNumber);
		Serial.print(" to Duty Cycle ");
		Serial.print(userInput);
		Serial.println("%");

		// Return back to the default state
		this->debuggerState = 0;
	}
}

// Read a mux input
void Debugger::readMuxInput(int userInput)
{
	Serial.println("Input mux input number between 1 and 16, 17 to monitor the encoder angle on mux input C0:");

	if(userInput == 17)
	{
		// Move to the encoder monitoring state
		this->debuggerState = 4;
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
	if(userInput == 1)
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
		// The default debugger state
		case 0:
			// Check if the user wants to print out the digital inputs
			if(userInput == 1)
			{
				this->printDigitalInputs(userInput);
			}
			// Check if the user wants to set a motor speed
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
				Serial.println("Input 1 to show the status of the digital input, 2 to set Motor PWM, 3 to select mux input.");
			}
			break;

		// State waiting for the user to input a motor number
		case 1:
			this->getMotorNumber(userInput);
			break;

		case 2:
			this->setMotorSpeed(userInput);
			break;

		case 3:
			this->readMuxInput(userInput);
			break;

		case 4:
			this->monitorEncoderAngle(userInput);
			break;
	}
}
