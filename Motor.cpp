#include "Motor.h"
#include "Declarations.h"

Motor_Ctrl::Motor_Ctrl() : MOT_A(PA_7), MOT_B(PA_8), MOT_C(PA_9), MOT_D(PA_10)
{

}

void Motor_Ctrl::run_F(int current_lim, int correction_factor)
{

	float current;
	int output;
	MOT_D = 0.0;
	MOT_A = 0;
	MOT_B = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/100.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= current_lim)
	{
		MOT_C = MOT_C - (0.01f*(((output-current_lim)/correction_factor)));
	}
	if(output < current_lim)
	{
		MOT_C = 0.9f;
	}
}
