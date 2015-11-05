#ifndef CURRENT_SENSE_H
#define CURRENT_SENSE_H
#include "mbed.h"
enum sense_gain { G20, G25, G50, G100};

class Current_Sense
{
private:
	AnalogIn CSENSE;
	DigitalOut SEL1;
	DigitalOut SEL2;
	DigitalOut CSENSE_EN;
public:
	Current_Sense();
	int get_current();
	void set_gain(sense_gain gain);
};

#endif //CURRENT_SENSE_H
