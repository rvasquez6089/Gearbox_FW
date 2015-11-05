#include "Current_Sense.h"

Current_Sense::Current_Sense() : CSENSE(PC_0), SEL1(PA_11), SEL2(PA_12),
		CSENSE_EN(PC_9), gain(G20)
{

}

void Current_Sense::init()
{
	CSENSE_EN = 1;
	set_gain(gain);
}

int Current_Sense::get_current()
{
	int current;
	if(SEL1.read())
	{
		if(SEL2.read()) //Equation for SEL1=1 and SEL2=1 100V/V
		{
			current = static_cast<int>(1000*(((CSENSE.read()*3.3f)/100.0f)/(0.015f)));
		}
		else//Equation for SEL1=1 and SEL2=0 50V/V
		{
			current = static_cast<int>(1000*(((CSENSE.read()*3.3f)/50.0f)/(0.015f)));
		}
	}
	else
	{
		if(SEL2.read())//Equation for SEL1=0 and SEL2=1 25V/V
		{
			current = static_cast<int>(1000*(((CSENSE.read()*3.3f)/25.0f)/(0.015f)));
		}
		else//Equation for SEL1=1 and SEL2=0 20V/V
		{
			current = static_cast<int>(1000*(((CSENSE.read()*3.3f)/20.0f)/(0.015f)));
		}
	}
	return current;
}

void Current_Sense::set_gain(sense_gain c_gain)
{
	gain = c_gain;
	switch(c_gain)
	{
	case G20:
		SEL1 = 0;
		SEL2 = 0;
		break;
	case G25:
		SEL1 = 0;
		SEL2 = 1;
		break;
	case G50:
		SEL1 = 1;
		SEL2 = 0;
		break;
	case G100:
		SEL1 = 1;
		SEL2 = 1;
		break;
	}
}

void Current_Sense::off()
{
	SEL1 = 0;
	SEL2 = 0;
	CSENSE_EN = 0;
}
