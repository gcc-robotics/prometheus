#import <Arduino.h>
#include <math.h>
#include "multiplexer.h"

// Function to call in the arduino setup function
Multiplexer::Multiplexer()
{
	// Digital pins that control the analog mux
	this->muxControlPin0 = 4;
	this->muxControlPin1 = 5;
	this->muxControlPin2 = 6;
	this->muxControlPin3 = 7;

	// Set the mux control pins as outputs
	pinMode(this->muxControlPin0, OUTPUT);
	pinMode(this->muxControlPin1, OUTPUT);
	pinMode(this->muxControlPin2, OUTPUT);
	pinMode(this->muxControlPin3, OUTPUT);

	// Default the control pins to low
	digitalWrite(this->muxControlPin0, LOW);
	digitalWrite(this->muxControlPin1, LOW);
	digitalWrite(this->muxControlPin2, LOW);
	digitalWrite(this->muxControlPin3, LOW);

	// Analog pin that reads the signal from the mux
	this->muxSignalPin = A0;

	// Set the selected input to 0
	this->selectedInput = 0;

	// Set the mux signal pin on the arduino as an INPUT
	pinMode(this->muxSignalPin, INPUT);
}

// Limits the input to between 0 and 15 inclusive
int Multiplexer::clampInputNumber(int inputNumber)
{
	// Clamp the inputNumber
	if(inputNumber < 0)
		inputNumber = 0;

	if(inputNumber > 15)
		inputNumber = 15;

	return inputNumber;
}

// Set the multiplexer to the provided input
// inputNumber = 0-indexed mux input number between 0 and 15
void Multiplexer::setInput(int inputNumber)
{
	// Clamp the inputNumber
	inputNumber = this->clampInputNumber(inputNumber);

	// Only update the control pins if the input number has changed
	if(this->selectedInput != inputNumber)
	{
		// Use bitwise & to set the correct control pins on / off
		digitalWrite(this->muxControlPin0, (inputNumber & 1));
		digitalWrite(this->muxControlPin1, (inputNumber & 2) / 2);
		digitalWrite(this->muxControlPin2, (inputNumber & 4) / 4);
		digitalWrite(this->muxControlPin3, (inputNumber & 8) / 8);

		// Keep track of the selected input
		this->selectedInput = inputNumber;
	}
}

// Read the provided input from the multiplexer
// inputNumber = mux input number between 0 and 15
int Multiplexer::readInput(int inputNumber)
{
	// Clamp the inputNumber
	inputNumber = this->clampInputNumber(inputNumber);

	// Set the mux to the provided input
	this->setInput(inputNumber);

	// Read the mux input and return
	return analogRead(this->muxSignalPin);
}

// Read a specific input from the mux and coverts it to an encoder angle
// inputNumber = mux input number between 0 and 15
float Multiplexer::readEncoder(int  inputNumber)
{
	int sensorValue = this->readInput(inputNumber);

	return fmod((sensorValue / 1023.0 * 360.0), 360.0);
}

// Read the selected mux input as a pot
// Return float between 0 and 1
float Multiplexer::readPotentiometer(int inputNumber)
{
	int sensorValue = this->readInput(inputNumber);
	int deadZone = 95;

	float potValue = (sensorValue - deadZone) / (1023.0 - deadZone);

	if(potValue < 0)
	{
		potValue = 0;
	}

	if(potValue > 1)
	{
		potValue = 1;
	}

	return potValue;
}