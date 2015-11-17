#ifndef PID_CONTROL_H
#define PID_CONTROL_H
#include "mbed.h"
#include "MMA8652.h"
#include "Current_Sense.h"
#include "Motor.h"
#include "Declarations.h"
#include <math.h>

const double PI= 3.14159265;
const float R = 4.87;
const float C = 0.1;
const float dt = PID_update_period; ////This is the discrete sample interval
const double RC = R*C;
const double alpha = dt / (RC + dt);
const int XY_BUFFER_SZ = 5;
const int ANGLE_BUFFER_SZ = 5;
const int ANGULAR_SPEED_SZ = 5;
const int ERROR_BUFFER_SZ = 100;


class PID
{

private:
	Motor_Ctrl *Motor;
	Current_Sense *CSENSE;
	MMA8652 *Accel;
	float Kp; //Proportional Tuning Constant
	float Ki; //Integral Tuning Constant
	float Kd; //Derivative Tuning Constant
	float Bias; //Option Bias
	float X_data[XY_BUFFER_SZ];
	float Y_data[XY_BUFFER_SZ];
	float Angle[ANGLE_BUFFER_SZ];
	float Angular_Spd[ANGULAR_SPEED_SZ];
	float Error[ERROR_BUFFER_SZ];
	float Trgt_Ang_Spd;
	float Current_PWM;
public:


	PID(Motor_Ctrl &mtr, Current_Sense &csen, MMA8652 &acc,
			float kp, //Proportional Tuning Constant
			float ki, //Integral Tuning Constant
			float kd, //Derivative Tuning Constant
			float bias); //optional Bias
	/** Fill_XY_Buffer fills the X_data and Y_data arrays, this helps start 
	 * the low pass filter and also fills the buffer when the constructor is
	 * called. */
	void Fill_XY_Buffer();
	/** Reed_Acc reads data from the accelerometer and pushes the data into a
	 * array buffer. It also filters the data with a first order Filter 
	*/
	void Read_Acc(); 
	/** Fill_Angle_Buffer fills the Angle Buffer when the gear starts,
	 * This helps to establish a good stable data base for the PID
	 * control to base is starting calculations from
	 */
	void Fill_Angle_Buffer();
	/** Read_Angle gets the angle and pushes into the Angle buffer
	 */
	void Read_Angle();
	
	void Fill_Angular_Spd_Buffer();

	void Calc_Angular_Spd();
	
	void Clear_Error_Buffer();

	void PID_Init();

	void Calc_Error();

	float Integrate_Error();

	float Derivate_Error();

	void PID_Control();

	void set_speed(float spd);

	float get_speed();
};

#endif
