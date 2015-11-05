#ifndef POWER_H
#define POWER_H
#include "mbed.h"
#include "Motor.h"
#include "Current_Sense.h"

class power_mgmt
{

private:
	AnalogIn VBAT;

public:
	power_mgmt();
	bool battery_status();
	void sleep();
};

#endif //POWER_H
