#ifndef PMOTORSPEED_H
#define PMOTORSPEED_H

class PMotorSpeed
{
	private:
		float proportionalgain[5];
		float setPoint[5];

		const int numMotors = 5;

		float min[5];
		float max[5];

	public:
		PMotorSpeed();
		int calculate(float currentAngleError);
		void setPoint(int motorNumber, float setPoint);
		float getJointMin(int motorNumber);
		float getJointMax(int motorNumber);
};

#endif