#ifndef MULTIPLEXER_H
#define MULTIPLEXER_H

class Multiplexer 
{
	private:
		// Digital pins on the arduino that control the analog mux
		int muxControlPin0;
		int muxControlPin1;
		int muxControlPin2;
		int muxControlPin3;

		// Analog pin on the arduino that reads the signal from the mux
		int muxSignalPin;

		// Currently selected input
		int selectedInput;

		// Limits the input to between 0 and 15 inclusive
		int clampInputNumber(int inputNumber);

		// Sets the mux to the provided input number
		void setInput(int inputNumber);

	public:
		// Constructor that prepares the mux for use
		Multiplexer();

		// Reads a specific input from the mux
		int readInput(int inputNumber);

		// Read a specific input from the mux and coverts it to an encoder angle
		float readEncoder(int inputNumber);

		// Read the selected mux input as a pot
		// Return float between 0 and 1
		float readPotentiometer(int inputNumber);
};

#endif