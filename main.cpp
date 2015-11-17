#include "mbed.h"
#include "Declarations.h"
#include "power.h"
#include "Motor.h"
#include "Current_Sense.h"
#include "PID_Control.h"
MMA8652 Accel(PB_9, PB_8);
Motor_Ctrl Motor;
Current_Sense CSENSE;
power_mgmt power(Motor, CSENSE);
sense_gain gain;
int go_to_sleep = 0;
Timeout go_sleep;
bool FF = 0;
bool RR = 0;
PID PID_Control(Motor,CSENSE, Accel, 1.0f,0.0f,0.0f,0.0f);
Ticker PID_Call;



void pressed_F()
{
    printf("Button pressed\n");
    go_to_sleep = !go_to_sleep;
    if(go_to_sleep == 1)
    {
    	go_sleep.detach();
    	PID_Call.detach();
    	PID_Control.set_speed(0.0);

    }
    if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		go_sleep.attach(&pressed_F, 10.0);
    		PID_Control.PID_Init();
    		PID_Control.set_speed(30.0);
    		PID_Call.attach(&PID_Control, &PID::PID_Control, PID_update_period);
    	}
	}
}

void pressed_R()
{
    printf("Button pressed\n");
    go_to_sleep = !go_to_sleep;
    if(go_to_sleep == 1)
    {
    	go_sleep.detach();
    	PID_Call.detach();
    	PID_Control.set_speed(0.0);
    }
    if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		go_sleep.attach(&pressed_R, 10.0);
    		PID_Control.PID_Init();
    		PID_Control.set_speed(-30.0);
    		PID_Call.attach(&PID_Control, &PID::PID_Control, PID_update_period);
    		
    	}
	}
}
 


int main()
{

	CSENSE.set_gain(G50); //Sets the current gain
    Motor.off();
    Button_P.fall(&pressed_F);
    Button_D.fall(&pressed_R);
    power.battery_status();

    //Orange.period(0.005);
    go_to_sleep = 1;
    while (1)
    {
        if (go_to_sleep)
        {
            
            FF = 0;
            RR = 0;
            myled = 0;
            //printf("%d: Entering sleep (press user button to resume)\n", i);
            power.sleep();
            sleep();
            wait(0.1);
            power.wake();

        }
        else
        {
            //printf("%d: Running\n", i);
            myled = 1;
            Orange = 1;  

        }
    }
}
