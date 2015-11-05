#ifndef POWER_H
#define POWER_H
#include "mbed.h"
#include "Motor.h"
#include "Current_Sense.h"

class power_mgmt
{

private:
	AnalogIn VBAT;
	Motor_Ctrl *Motor;
	Current_Sense *CSENSE;

public:
	power_mgmt();
	power_mgmt(Motor_Ctrl &mtr, Current_Sense &csen);
	bool battery_status();
	void sleep();
	void wake();
};

#endif //POWER_H
