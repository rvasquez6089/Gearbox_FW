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
	sense_gain gain;
public:
	Current_Sense();
	void init();
	int get_current();
	void set_gain(sense_gain c_gain);
	void off();
};

#endif //CURRENT_SENSE_H
