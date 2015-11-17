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
	Kp = kp;
	Ki = ki;
	Kd = kd;
	Bias = bias;
	for(int i = 0; i < ANGULAR_SPEED_SZ -1; i++)
	{
		Angular_Spd[i] = 0.0;
	}


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
	for(int i = XY_BUFFER_SZ - 1; i > 1; i--)/** Shifts out the oldest data.
	Stops at 1 so that way the 0 index is empty and ready for new data. */
	{
		X_data[i] = X_data[i-1];
		Y_data[i] = Y_data[i-1];
	}
	Accel.ReadXYZ(acc_data);
	////The Following lines filter the data with a low pass Filter
	////Refer to the constants declared in PID_Control.h
	X_data[0] = (alpha * acc_data[0]) + (1.0 - alpha) * X_data[1];
	Y_data[0] = (alpha * acc_data[1]) + (1.0 - alpha) * Y_data[1];
}

void PID::Fill_Angle_Buffer()
{
	for(int i = ANGLE_BUFFER_SZ - 1; i > 0; i--)
	{
		Angle[i] = atan2 (-1.0f*(X_data[0]),-1.0f*(Y_data[0])) * 180 / PI;
		Read_Acc();
		wait_ms(5);
	}
}

void PID::Read_Angle()
{
	for(int i = ANGLE_BUFFER_SZ - 1; i > 1; i--)/** Shifts out the oldest data.
		Stops at 1 so that way the 0 index is empty and ready for new data. */
	{
		Angle[i] = Angle[i-1];
	}
	Read_Acc();////Refresh X_data and Y_data
	Angle[0] =  atan2 (-1.0f*(X_data[0]),-1.0f*(Y_data[0])) * 180 / PI;
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
	for(int i  = ANGULAR_SPEED_SZ - 1; i > 1; i--)
	{
		Angular_Spd[i] = Angular_Spd[i-1];
	}
	Read_Angle();
	Angular_Spd[i] = (Angle[0]-Angle[1])/PID_update_period;
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
}

void PID::Calc_Error()
{
	for(int i = ERROR_BUFFER_SZ - 1; i > 1; i--)
	{
			Error[i] = Error[i-1];
	}
	Calc_Angular_Spd();
	Error[0] = Trgt_Ang_Spd - Angular_Spd[0];
}

float PID::Integrate_Error()
{
	float intg_error = 0;
	for(int i = 0; i < ERROR_BUFFER_SZ - 1; i++)
	{
		intg_error = intg_error* (Error[i] * PID_update_period);
	}
	return intg_error;
}
