#include "robotArm.h"

#ifndef COMMAND_PROCESSOR
#define COMMAND_PROCESSOR

class CommandProcessor
{
	private:
		// Internal reference to the RobotArm
		RobotArm* arm;

	public:
		// Constructor
		CommandProcessor();

		void setup(RobotArm* robotArm);

		// -----------------------------------------------
		// Commands --------------------------------------
		// -----------------------------------------------

		// Command to set a joint to a specific angle
		void setJointAngle(String command);

		// Loop function for the CommandProcessor
		// Call in the Arduino loop function
		void loop();
};

#endif