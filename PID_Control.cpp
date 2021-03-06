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
	Ki = ((ki*PID_update_period*625)/ERROR_BUFFER_SZ)*100000.0;
	Kd = (kd*0.000000000001)/PID_update_period;
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
		wait(PID_update_period);
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
	float Temp_Ang;
	for(int i = ANGLE_BUFFER_SZ - 1; i > 0; i--)
	{
		//Angle[i] = atan2 (-1.0f*(Y_data[0]),-1.0f*(X_data[0])) * 180 / PI;
		Temp_Ang = atan2 (X_data[0],Y_data[0]) * 180 / PI;
		Angle[i] = fmod((Temp_Ang + 360.0),360.0);
		Read_Acc();
		wait(PID_update_period);
	}
}

void PID::Read_Angle()
{
	float Temp_Ang;
	//printf("Ang=%d ", static_cast<int>(Angle[0]));
	for(int i = ANGLE_BUFFER_SZ - 1; i >= 1; i--)/** Shifts out the oldest data.
		Stops at 1 so that way the 0 index is empty and ready for new data. */
	{
		Angle[i] = Angle[i-1];
	}
	Read_Acc();////Refresh X_data and Y_data
	//Angle[0] =  atan2 (-1.0f*(Y_data[0]),-1.0f*(X_data[0])) * 180 / PI;
	Temp_Ang = atan2 (X_data[0],Y_data[0]) * 180 / PI;
	Angle[0] = fmod((Temp_Ang + 360.0),360.0);
}

void PID::Fill_Angular_Spd_Buffer()
{
	for(int i = ANGULAR_SPEED_SZ - 1; i > 0;i--)
	{
		Angular_Spd[i] = (Angle[0] - Angle[1])/PID_update_period;
		Read_Angle();
		wait(PID_update_period);
	}
}



void PID::Calc_Angular_Spd()
{
	//printf("AngSpd = %d ", static_cast<int>(Angular_Spd[0]));
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
	Clear_All_Buffers();
	Fill_XY_Buffer();
	Fill_Angle_Buffer();
	Fill_Angular_Spd_Buffer();

	for(int i = 0; i < ANGULAR_SPEED_SZ -1; i++)
	{
		Angular_Spd[i] = 0.0;
	}
	Current_PWM = 0.0;
}

void PID::Calc_Error()
{
	float temp_error;
	for(int i = ERROR_BUFFER_SZ - 1; i > 1; i--)
	{
			Error[i] = Error[i-1];
	}
	Calc_Angular_Spd();
	//Error[0] = Angular_Spd[0] - Trgt_Ang_Spd;
	temp_error = Trgt_Ang_Spd - Angular_Spd[0];
	if(temp_error > 1000.0)
	{
		temp_error = 0.0;
	}
	else if(temp_error < -1000.0)
	{
		temp_error = 0.0;
	}
	Error[0] = (0.95  * temp_error) + (1.0 - 0.95) * Error[1];
	//Error[0] = temp_error;
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
	float der_error = 0.0;
	const int Averages = 20;
	for(int i = 0;i < Averages; i++)
	{
		der_error = der_error +
				(Error[i] - Error[i+1]);
	}
	//der_error = (Error[i] - Error[i-1])/PID_update_period;

	der_error = der_error/static_cast<float>(Averages);
	der_error = der_error/PID_update_period;
	return der_error;
}

void PID::PID_Control()
{

	Calc_Error();
	float temp;
	float New_PWM = 0.0;
	float intg = Integrate_Error();
	float deriv = Derivate_Error();
	New_PWM = Current_PWM + (( Kp * Error[0] +
			Ki * intg + Kd * deriv + Bias)*PID_update_period);
	//printf("intg=%d ", static_cast<int>(intg*1000.0));
	//printf("deriv=%d ", static_cast<int>(deriv*1000.0));
	if(New_PWM > 1.0f)
	{
		New_PWM = 1.0f;
	}
	else if(New_PWM < -1.0f)
	{
		New_PWM = -1.0f;
	}
	//printf("Error[0] = %d ",static_cast<int>(Error[0]));
	//printf("PWM = %d  ",static_cast<int>(New_PWM*1000.0));

	if(!STOP)
	{
		if(New_PWM > 0.0f)
		{
			//printf("Motor F \n");
			Current_PWM = New_PWM;
			Motor->run_F(New_PWM);
		}
		else if(New_PWM < 0.0f)
		{
			//printf("Motor R \n");


			Current_PWM = New_PWM;
			temp = fabsf(New_PWM);
			Motor->run_R(temp);
		}
	}
	else
	{
		Motor->off();
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

void PID::set_Trgt_Angle(float Trgt_Ang)
{
	Trgt_Angle = Trgt_Ang;
}

void PID::go_to_angle()
{
	//static bool brake;
	float angular_distance;
	angular_distance = Angle[0] - Trgt_Angle;
	if(angular_distance >= 180.0)
	{
		Trgt_Ang_Spd = 45.0;
	}
	else if(angular_distance < -20.0 && angular_distance >= -180.0)
	{
		Trgt_Ang_Spd = 45.0;
	}
	else if(angular_distance < -180.0)
	{
		Trgt_Ang_Spd = -45.0;
	}
	else if(angular_distance > 20.0 && angular_distance < 180)
	{
		Trgt_Ang_Spd = -45.0;
	}
	else if(angular_distance <= 20 && angular_distance >= -20)
	{

		Trgt_Ang_Spd = 0.0;
		/*if(brake)
		{
			Trgt_Ang_Spd = 0.0;
			brake = !brake;
		}
		else
		{
			Trgt_Ang_Spd = 0.0;
			brake = !brake;
		}*/

	}
	PID_Control();
}

void PID::Clear_All_Buffers()
{
	for(int i = 0; i < ERROR_BUFFER_SZ - 1; i++)
	{
		Error[i] = 0.0;
	}
	for(int i = 0; i < XY_BUFFER_SZ - 1; i++)
	{
		X_data[i] = 0.0;
		Y_data[i] = 0.0;
	}
	for(int i = 0; i < ANGLE_BUFFER_SZ - 1; i++)
	{
		Angle[i] = 0.0;
	}
	for(int i = 0; i < ANGULAR_SPEED_SZ - 1; i++)
	{
		Angular_Spd[i] = 0.0;
	}


}

void PID::Stop_At_Angle()
{

	if(Angle[0] < (Trgt_Angle + 5.0) && Angle[0] >(Trgt_Angle - 5.0))
	{
		Motor->off();
		STOP = 1;
	}
	else
	{
		PID_Control();
	}

}
