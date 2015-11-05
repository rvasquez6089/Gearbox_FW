#ifndef MOTOR_H
#define MOTOR_H
#include "mbed.h"
#include "Declarations.h"

class Motor_Ctrl
{
private:
	PwmOut MOT_C;
	DigitalOut MOT_B;
	PwmOut MOT_D;
	DigitalOut MOT_A;

public:
	Motor_Ctrl();
	void run_F(float pulse_width);//pulse width should be between 0.0f and 1.0f
	void run_R(float pulse_width);//pulse width should be between 0.0f and 1.0f
	void off();
};
#endif //MOTOR_H
