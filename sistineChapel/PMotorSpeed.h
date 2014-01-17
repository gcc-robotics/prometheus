#ifndef PMOTORSPEED_H
#define PMOTORSPEED_H

class PMotorSpeed
{
	private:
		float proportionalgain[5];
		float setPoint[5];
		float min[5];
		float max[5];
        int numMotors;

	public:
		PMotorSpeed();
		int calculate(int motorNumber, float currentAngleError);
		void setSetPoint(int motorNumber, float setPoint);
		void setProportionalGain(int motorNumber, float proportionalgain);
		float getJointMin(int motorNumber);
		float getJointMax(int motorNumber);
};

#endif
