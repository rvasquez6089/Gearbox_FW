#include "Motor.h"
#include "Declarations.h"

Motor_Ctrl::Motor_Ctrl() : MOT_A(PA_7), MOT_B(PA_8), MOT_C(PA_9), MOT_D(PA_10)
{
	MOT_C.period(pwm_period);
	MOT_D.period(pwm_period);
}

void Motor_Ctrl::run_F(float pulse_width)
{
	MOT_A = 0;
	MOT_D = 0.0;
	MOT_B = 1;
	MOT_C = pulse_width;
}

void Motor_Ctrl::run_R(float pulse_width)
{
	MOT_B = 0;
	MOT_C = 0.0;
	MOT_A = 1;
	MOT_D = pulse_width;
}

void Motor_Ctrl::off()
{
	MOT_D = 0.0f;
	MOT_A = 0;
	MOT_C = 0.0f;
	MOT_B = 0;
}
