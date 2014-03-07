#ifndef PMOTORSPEED_H
#define PMOTORSPEED_H

class PMotorSpeed
{
	private:
		double proportionalGain[5];
		double min[5];
		double max[5];
       
    protected:
    	int numMotors;
    	double setPoint[5];

	public:
		PMotorSpeed();
		int calculate(int jointNumber, double currentAngle);
		void setSetPoint(int jointNumber, double setPoint);
		void setProportionalGain(int jointNumber, double proportionalgain);
		double getProportionalGain(int jointNumber);
		double getJointMin(int jointNumber);
		double getJointMax(int jointNumber);
		double getAngleError(int jointNumber, double currentAngle);
};

#endif
