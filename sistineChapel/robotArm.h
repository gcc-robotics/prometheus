#include "motorController.h"
#include "multiplexer.h"

#ifndef ROBOTARM_H
#define ROBOTARM_H

class RobotArm 
{
	private:
		float setPoint[5];

		int min[5];
		int max[5];
		
		// Input/Output numbers
		int motorNumber[5];
		int encoderNumber[5];

		// Instances of the MotorController and Multiplexer
		MotorController motor;
		Multiplexer mux;

		/* Calculates the smallest angle difference between the target angle and current angle
		 * input targetAngle = 0 to 359
		 * input currentAngle = 0 to 359
		 * return float between 0 to 180
		 */
		float getAngleError(float targetAngle, float currentAngle);

		/* Contains the logic to actually move a joint to the desired angle
		 * input jointNumber = 0 to 4
		 * return boolean true or false
		 */
		bool moveJointToSetPoint(int jointNumber);

		/* Clamps the input between minimum and maximum
		 * input input any float 
		 * return float between 0.0 and 1.0
		 */
		float clamp(float input, float min = 0.0, float max = 1.0);

		/* Clamps the input between minimum and maximum
		 * input input any integer 
		 * return int between 0 and 4
		 */	
		int clamp(int input, int min = 0, int max = 4);

	public:	
		// Constructor
		RobotArm();

		Multiplexer* getMultiplexer();
		MotorController* getMotorController();

		/* Setup the arm for use
		 * no input
		 * return void
		 */
		void setup();

		/* Set the motor speed for the provided joint and do speed boosts for speeds below the boost threshold
		 * input jointNumber = 0 to 4
		 * input speed = 
		 * return void
		 */
		void setJointMotorSpeed(int jointNumber, int speed);

		/* Set joint setPoint
		 * input jointNumber = 0 to 4
		 * input angle = 0.0 to 359.9
		 * return void
		 */
		void setJointAngle(int jointNumber, float angle);

		/* Gets the current angle value for provided joint
		 * input jointNumber = 0 to 4
		 * return int between 0 to 359
		 */
		int getJointAngle(int jointNumber);

		/* Gets the minimum angle value for provided joint to limit the movements
		 * input jointNumber = 0 to 4
		 * return int between 0 to 359
		 */
		int getJointMinimum(int jointNumber);

		/* Gets the maximum angle value for provided joint to limit the movements
		 * input jointNumber = 0 to 4
		 * return int between 0 to 359
		 */
		int getJointMaximum(int jointNumber);

		/* Move the waist the the provided angle
		 * input degrees = 0.0 to 359.9
		 * return void
		 */
		void waist(float degrees);

		/* Move the shoulder the the provided angle
		 * input degrees = 0.0 to 359.9
		 * return void
		 */
		void shoulder(float degrees);

		/* Move the elbow the the provided angle
		 * input degrees = 0.0 to 359.9
		 * return void
		 */
		void elbow(float degrees);

		/* Move the wrist the the provided angle
		 * input degrees = 0.0 to 359.9
		 * return void
		 */
		void wrist(float degrees);

		/* Move the hand the the provided angle
		 * input degrees = 0.0 to 359.9
		 * return void
		 */
		void hand(float degrees);

		// Run in arduino loop()
		void loop();
};

#endif
