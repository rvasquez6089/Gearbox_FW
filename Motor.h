#ifndef MOTOR_H
#define MOTOR_H
#include "mbed.h"
#include "Declarations.h"

class Motor_Ctrl
{
private:
	DigitalOut MOT_A;
	DigitalOut MOT_B;
	PwmOut MOT_C;
	PwmOut MOT_D;


public:
	Motor_Ctrl();
	void run_F(float pulse_width);//pulse width should be between 0.0f and 1.0f
	void run_R(float pulse_width);//pulse width should be between 0.0f and 1.0f
	void off();
};
#endif //MOTOR_H
