#include "power.h"
#include "Declarations.h"

//AnalogIn VBAT(PC_1);
power_mgmt::power_mgmt() : VBAT(PC_1)
{

}

power_mgmt::power_mgmt(Motor_Ctrl &mtr, Current_Sense &csen) : VBAT(PC_1)
{
	Motor = &mtr;
	CSENSE = &csen;
}

bool power_mgmt::battery_status()
{
	float Brightness = 0.25;
	double battery_level = 0.0;
	const int on = 50;
	const int off = 100;
	bool low_bat = 0;
	for(int i = 0; i < 1000; i++)
	{
		battery_level = battery_level + VBAT.read();
	}
	battery_level = battery_level/1000.0;
	battery_level = battery_level*3.3*2.0;
	printf("Battery Level mV = %d \n", static_cast<int>(battery_level*1000.0));
	if(battery_level >= 3.825)
	{
		wait_ms(250);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		low_bat = 0;
	}
	else if(battery_level >= 3.45 && battery_level < 3.825)
	{
		wait_ms(250);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		low_bat = 0;
	}
	else if(battery_level >= 3.075 && battery_level < 3.45)
	{
		wait_ms(250);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		low_bat = 0;
	}
	else if(battery_level < 3.075 && battery_level >= 2.9)
	{
		wait_ms(250);
		Orange = Brightness;
		wait_ms(on);
		Orange = 0;
		wait_ms(off);
		low_bat = 0;
	}
	else if(battery_level < 2.9)
	{
		wait_ms(250);
		Orange = Brightness;
		wait_ms(75);
		Orange = 0;
		wait_ms(150);
		low_bat = 1;
	}
	return low_bat;
}

void power_mgmt::sleep()
{
	Orange = 0;
	Motor->off();
	CSENSE->off();
}

void power_mgmt::wake()
{
	CSENSE->init();
}
