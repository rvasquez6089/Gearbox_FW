#ifndef PID_CONTROL_H
#define PID_CONTROL_H
#include "mbed.h"
#include "MMA8652.h"
#include "Current_Sense.h"
#include "Motor.h"
#include "Declarations.h"
#include <math.h>

const double PI= 3.14159265;
const float R = 10;
const float C = 0.1;
const float dt = PID_update_period; ////This is the discrete sample interval
const double RC = R*C;
const double alpha = dt / (RC + dt);
const int XY_BUFFER_SZ = 5;
const int ANGLE_BUFFER_SZ = 5;
const int ANGULAR_SPEED_SZ = 5;
const int ERROR_BUFFER_SZ = 250;


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
	 * It calls the Read_Acc() function to bring in fresh accelerometer
	 * data and it calculates the angle pushes it into the Angle data
	 * buffer
	 */
	void Read_Angle();


	/**Fill_Angular_Spd_Buffer() fills the Angular_Spd buffer. This helps
	 * in the PID_Init() function to fill the buffer with real valid data
	 * from which to base its initial calculations and movements from.
	 */
	void Fill_Angular_Spd_Buffer();


	/** Calc_Angular_Spd() calculates the angular speed from the most
	 * recent and second most recent angle values in the Angle buffer
	 * and pushes the angular speed into the Angular_Spd buffer.
	 */
	void Calc_Angular_Spd();


	/** Clear_Error_Buffer() Clears the error buffer from the most
	 * run of the gearbox. This helps the gearbox when starting up
	 * to base its movement calculations from valid data.
	 */
	void Clear_Error_Buffer();


	/**PID_Init() Calls the Fill_XY_Buffer(), Fill_Angle_Buffer(),
	 * Fill_Angular_Spd_Buffer, and Clear_Error_Buffer() functions to
	 * the PID loop all fresh valid data to start its PID calculations.
	 */
	void PID_Init();


	/**Calc_Error() function calculates the error by subtracting Trgt_Ang_Spd
	 * from Angular_Spd[0] to get the error in angular speed. Once the
	 * error is calculated it is then stored in the Error[] buffer.
	 */
	void Calc_Error();


	/**Integrate_Error() integrates the error from the data stored in
	 * Error buffer in respect to the sampling rate of the data and
	 * length of the Error Buffer.
	 */
	float Integrate_Error();


	/**Derivate_Error calculates the derivative of the error over a set
	 * number of error sampels and returns a float of the change
	 * in error over those samples.
	 */
	float Derivate_Error();

	/**PID_Control() calls the Calc_Error() function which calculates
	 * the current error in the angular speed of the gearbox
	 * and calculates a new PWM value for the system and then
	 * turns the motor according to those calculations
	 * This function needs to be called on a constant period
	 * basis (PID_update_period)
	 */
	void PID_Control();

	void set_speed(float spd);

	float get_speed();
};

#endif
