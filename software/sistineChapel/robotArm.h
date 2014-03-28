#include "motorController.h"
#include "multiplexer.h"
#include "PIMotorSpeed.h"

#ifndef ROBOTARM_H
#define ROBOTARM_H

class RobotArm 
{
	private:
		// Interrupt
		volatile bool emergencyState;
		int interruptPinNumber;

		// Input/Output numbers
		int motorNumber[5];
		int encoderNumber[5];
		float encoderOffset[5]; 

		// Instances of the MotorController and Multiplexer
		MotorController motor;
		Multiplexer mux;
		PIMotorSpeed motorSpeed;

		/* Contains the logic to actually move a joint to the desired angle
		 * input jointNumber = 0 to 4
		 * return boolean true or false
		 */
		bool moveJointToSetPoint(int jointNumber);

	public:	
		// Interrupt
		int interruptNumber;

		// Constructor
		RobotArm();

		Multiplexer* getMultiplexer();
		MotorController* getMotorController();
		PIMotorSpeed* getPIMotorSpeed();

		/* Setup the arm for use
		 * no input
		 * return void
		 */
		void setup();

		/* Called whenever the interrupt changes state
		* No Input
		* return void
		*/
		void interruptResponder();

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
		float getJointMinimum(int jointNumber);

		/* Gets the maximum angle value for provided joint to limit the movements
		 * input jointNumber = 0 to 4
		 * return int between 0 to 359
		 */
		float getJointMaximum(int jointNumber);

		void openClaw();

		void closeClaw();

		void brakeClaw();

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
