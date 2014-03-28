#ifndef PIDMOTORSPEED_H
#define PIDMOTORSPEED_H

#include "PIMotorSpeed.h"

class PIDMotorSpeed : public PIMotorSpeed
{
	private:
		unsigned long previousTime[5];
		double DTerm[5];
		double derivativeGain[5];
		double previousSetPoint[5];
		double motorSpeed[5];

	protected:
		unsigned long sampleTime;

	public:
		PIDMotorSpeed();
		int calculate(int jointNumber, double currentAngle);
		void setDerivativeGain(int jointNumber, double newintegralgain);
		double getDerivativeGain(int jointNumber);
};

#endif