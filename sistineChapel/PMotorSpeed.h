#ifndef PMOTORSPEED_H
#define PMOTORSPEED_H

class PMotorSpeed
{
	private:
		float proportionalGain[5];
		float setPoint[5];
		float min[5];
		float max[5];
       
    protected:
    	int numMotors;

	public:
		PMotorSpeed();
		int calculate(int jointNumber, float currentAngle);
		void setSetPoint(int jointNumber, float setPoint);
		void setProportionalGain(int jointNumber, float proportionalgain);
		float getJointMin(int jointNumber);
		float getJointMax(int jointNumber);
		float getAngleError(int jointNumber, float currentAngle);
};

#endif
