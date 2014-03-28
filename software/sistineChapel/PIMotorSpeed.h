#ifndef PIMOTORSPEED_H
#define PIMOTORSPEED_H

#include "PMotorSpeed.h"

class PIMotorSpeed : public PMotorSpeed
{
	private:
		unsigned long previousTime[5];
		double ITerm[5];
		double integralGain[5];
		double previousSetPoint[5];
		double motorSpeed[5];

		//Derivative
		//double DTerm[5];
		//double derivativeGain[5];

	protected:
		unsigned long sampleTime;

	public:
		PIMotorSpeed();
		int calculate(int jointNumber, double currentAngle);
		void setIntegralGain(int jointNumber, double newintegralgain);
		double getIntegralGain(int jointNumber);

		//Derivative
		//void setDerivativeGain(int jointNumber, double newintegralgain);
		//double getDerivativeGain(int jointNumber);
};

#endif