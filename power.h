#ifndef POWER_H
#define POWER_H
#include "mbed.h"

class power_mgmt
{

private:
	AnalogIn VBAT;
public:
	power_mgmt();
	bool battery_status();
	void sleep();
};
//extern AnalogIn VBAT;

//bool battery_status();
#endif //POWER_H
