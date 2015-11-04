#ifndef MOTOR_H
#define MOTOR_H
#include "mbed.h"

class Motor_Ctrl
{
private:
	PwmOut MOT_C;
	DigitalOut MOT_B;
	PwmOut MOT_D;
	DigitalOut MOT_A;

public:
	Motor_Ctrl();
	void run_F(int current_lim, int correction_factor);
};
#endif //MOTOR_H
