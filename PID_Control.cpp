#include "PID_Control.h"




PID::PID(Motor_Ctrl &mtr, Current_Sense &csen, MMA8652 &acc,
			float kp, //Proportional Tuning Constant
			float ki, //Integral Tuning Constant
			float kd, //Derivative Tuning Constant
			float bias)
{
	Motor = &mtr;
	CSENSE = &csen;
	Accel = &acc;
	Kp = kp*PID_update_period;
	Ki = (ki*PID_update_period*625)/250;
	Kd = (kd*0.01)/PID_update_period;
	Bias = bias;
	for(int i = 0; i < ANGULAR_SPEED_SZ -1; i++)
	{
		Angular_Spd[i] = 0.0;
	}
	Current_PWM = 0.0;

}

void PID::Fill_XY_Buffer()
{
	float acc_data[3];
	for(int i = XY_BUFFER_SZ - 1; i > 0; i--)
	{
		Accel->ReadXYZ(acc_data);
		X_data[i] = acc_data[0];
		Y_data[i] = acc_data[1];
		wait_ms(5);
	}
}

void PID::Read_Acc()
{
	float acc_data[3];
	for(int i = XY_BUFFER_SZ - 1; i >= 1; i--)/** Shifts out the oldest data.
	Stops at 1 so that way the 0 index is empty and ready for new data. */
	{
		X_data[i] = X_data[i-1];
		Y_data[i] = Y_data[i-1];
	}
	Accel->ReadXYZ(acc_data);
	////The Following lines filter the data with a low pass Filter
	////Refer to the constants declared in PID_Control.h
	X_data[0] = (alpha * acc_data[0]) + (1.0 - alpha) * X_data[1];
	Y_data[0] = (alpha * acc_data[1]) + (1.0 - alpha) * Y_data[1];
}

void PID::Fill_Angle_Buffer()
{
	for(int i = ANGLE_BUFFER_SZ - 1; i > 0; i--)
	{
		Angle[i] = atan2 (-1.0f*(Y_data[0]),-1.0f*(X_data[0])) * 180 / PI;
		Read_Acc();
		wait_ms(5);
	}
}

void PID::Read_Angle()
{
	printf("Angle = %d ", static_cast<int>(Angle[0]));
	for(int i = ANGLE_BUFFER_SZ - 1; i >= 1; i--)/** Shifts out the oldest data.
		Stops at 1 so that way the 0 index is empty and ready for new data. */
	{
		Angle[i] = Angle[i-1];
	}
	Read_Acc();////Refresh X_data and Y_data
	Angle[0] =  atan2 (-1.0f*(Y_data[0]),-1.0f*(X_data[0])) * 180 / PI;
}

void PID::Fill_Angular_Spd_Buffer()
{
	for(int i = ANGULAR_SPEED_SZ - 1; i > 0;i--)
	{
		Angular_Spd[i] = (Angle[0] - Angle[1])/PID_update_period;
		Read_Angle();
		wait_ms(5);
	}
}



void PID::Calc_Angular_Spd()
{
	printf("Angular Speed = %d ", static_cast<int>(Angular_Spd[0]));
	for(int i  = ANGULAR_SPEED_SZ - 1; i >= 1; i--)
	{
		Angular_Spd[i] = Angular_Spd[i-1];
	}
	Read_Angle();
	Angular_Spd[0] = (Angle[0]-Angle[1])/PID_update_period;

}

void PID::Clear_Error_Buffer()
{
	for(int i = 0; i < ERROR_BUFFER_SZ - 1; i++)
	{
		Error[i] = 0.0;
	}
}

void PID::PID_Init()
{
	Fill_XY_Buffer();
	Fill_Angle_Buffer();
	Fill_Angular_Spd_Buffer();
	Clear_Error_Buffer();
	for(int i = 0; i < ANGULAR_SPEED_SZ -1; i++)
	{
		Angular_Spd[i] = 0.0;
	}
	Current_PWM = 0.0;
}

void PID::Calc_Error()
{
	for(int i = ERROR_BUFFER_SZ - 1; i > 1; i--)
	{
			Error[i] = Error[i-1];
	}
	Calc_Angular_Spd();
	Error[0] = Angular_Spd[0] - Trgt_Ang_Spd;
	if(Error[0] > 10000.0)

	{
		Error[0] = 0.0;
	}
	else if(Error[0] < -10000.0)
	{
		Error[0] = 0.0;
	}

}

float PID::Integrate_Error()
{
	float intg_error = 0;
	for(int i = 0; i < ERROR_BUFFER_SZ - 1; i++)
	{
		intg_error = intg_error + (Error[i] * PID_update_period);
	}
	return intg_error;
}

float PID::Derivate_Error()
{
	float der_error = 0;
	const int Averages = 1;
	for(int i = Averages+1;i > 1; i--)
	{
		der_error = der_error +
				(Error[i] - Error[i-1])/PID_update_period;
	}
	der_error = der_error/static_cast<float>(Averages);
	return der_error;
}

void PID::PID_Control()
{

	Calc_Error();
	float temp;
	float New_PWM = 0.0;
	float intg = Integrate_Error();
	float deriv = Derivate_Error();
	New_PWM = Current_PWM - (( Kp * Error[0] +
			Ki * intg + Kd * deriv + Bias)*PID_update_period);
	if(New_PWM > 1.0f)
	{
		New_PWM = 1.0f;
	}
	else if(New_PWM < -1.0f)
	{
		New_PWM = -1.0f;
	}
	printf("Error[0] = %d ",static_cast<int>(Error[0]));
	printf("PWM = %d  ",static_cast<int>(New_PWM*1000.0));
	if(New_PWM > 0.0f)
	{
		printf("Motor R \n");
		Current_PWM = New_PWM;
		Motor->run_R(New_PWM);
	}
	else if(New_PWM < 0.0f)
	{
		printf("Motor F \n");


		Current_PWM = New_PWM;
		temp = fabsf(New_PWM);
		Motor->run_F(temp);
	}


}

void PID::set_speed(float spd)
{
	Trgt_Ang_Spd  = spd;
}

float PID::get_speed()
{
	return Trgt_Ang_Spd;
}


