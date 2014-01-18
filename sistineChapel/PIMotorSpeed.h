#ifndef PIMOTORSPEED_H
#define PIMOTORSPEED_H

#include "PMotorSpeed.h"

class PIMotorSpeed : public PMotorSpeed
{
	private:
		long int previousTime;
		int previousMotorSpeed[5];
		float integralGain[5];

	public:
		PIMotorSpeed();
		int calculate(int jointNumber, float currentAngle);
};

#endif