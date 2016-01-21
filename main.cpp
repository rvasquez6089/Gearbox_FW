#include "mbed.h"
#include "Declarations.h"
#include "power.h"
#include "Motor.h"
#include "Current_Sense.h"
#include "PID_Control.h"
Serial pc(USBTX,USBRX);
MMA8652 Accel(PB_9, PB_8);
Motor_Ctrl Motor;
Current_Sense CSENSE;
power_mgmt power(Motor, CSENSE);
sense_gain gain;
int go_to_sleep = 0;
Timeout go_sleep;
Timeout go_home;
Timer Stall;
bool FF = 0;
bool RR = 0;
bool running = 0;

PID PID_Control(Motor,CSENSE, Accel, 0.80f,0.0005f,0.025f,0.0f);
//PID PID_Control(Motor,CSENSE, Accel, 100.0f,0.0f,0.0f,0.0f); //good
Ticker PID_Call;

void pressed_CW();

void pressed_CCW();

void sleep_init();

void set_to_sleep();

void go_CW();

void go_CCW();

void go_home_CW();

void go_home_CCW();
int main()
{
	bool overload = 0;
	bool timer_started = 0;
	float temp;
	pc.baud(921600);
    printf("Priority Designs Robotic Gear \n");
    printf("Ryan Vasquez 2016 \n");
	Orange.period(pwm_period);
	CSENSE.set_gain(G25); //Sets the current gain
    Motor.off();
    Button_P.fall(&pressed_CW);
    Button_D.fall(&pressed_CCW);
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
            //Orange = 1;
            temp = CSENSE.get_current();
            //printf("Current mA = %d \n", static_cast<int>(temp*1.0));
            if(CSENSE.get_current() > 3200)
            {
            	if(!timer_started)
            	{
            		Stall.start();
            		//printf("Timer started \n");
            		timer_started = 1;

            	}
            	else
            	{
                    if(Stall.read() > 2.0)
                    {

                    	//printf("Timer stopped \n");
                    	sleep_init();
                		Stall.stop();
                		Stall.reset();
                    	power.battery_status();

                		timer_started = 0;

                    }
                    else
                    {
                    	//PID_Control.STOP = 0;

                    }
            	}
            }
            else
            {

            		Stall.stop();
            		Stall.reset();
            		timer_started = 0;


            }

        }
    }
}


void pressed_CW()
{
    pc.printf("CW pressed\n");
    go_to_sleep = 0;
    if(running)
    {
		go_sleep.detach();
		PID_Call.detach();
		go_home.detach();
		set_to_sleep();
    }
    else
    {
		//nothing
	}
    if(go_to_sleep == 1)
    {
    	sleep_init();

    }
    else if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		if(Accel.upright())
    		{
    			running = 1;
    			Orange = 0.25;
    			go_sleep.detach();
    			PID_Call.detach();
    			go_home.detach();
    			PID_Control.STOP = 0;
    			go_sleep.attach(&set_to_sleep, 30.0);//makes gearbox go to sleep after a timeout
    			//30 seconds.
    			go_CW();//makes the gearbox go CW for 10 seconds
    			go_home.attach(&go_home_CW,10.0);
    		}
    	}
	}
}

void pressed_CCW()
{
    pc.printf("CCW pressed\n");
    go_to_sleep = 0;
    if(running)
    {
		go_sleep.detach();
		PID_Call.detach();
		go_home.detach();
		set_to_sleep();
    }
    else
    {
		//nothing
	}
    if(go_to_sleep == 1)
    {
    	sleep_init();

    }
    else if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		if(Accel.upright())
    		{
    			running = 1;
    			Orange = 0.25;
    			go_sleep.detach();
    			PID_Call.detach();
    			go_home.detach();
    			PID_Control.STOP = 0;
    			go_sleep.attach(&set_to_sleep, 30.0);//makes gearbox go to sleep after a timeout
    			//30 seconds.
    			go_CCW();//makes the gearbox go CW for 10 seconds
    			go_home.attach(&go_home_CCW,10.0);
    		}
    	}
	}
}

void sleep_init()
{
	go_sleep.detach();
	PID_Call.detach();
	go_home.detach();
	PID_Control.set_speed(0.0);

	set_to_sleep();
}

void set_to_sleep()
{
	go_to_sleep = 1;
	running = 0;
}

void go_CW()
{

	PID_Control.PID_Init();
	PID_Control.set_speed(-90.0);
	PID_Call.attach(&PID_Control, &PID::PID_Control, PID_update_period);

}

void go_CCW()
{

	PID_Control.PID_Init();
	PID_Control.set_speed(90.0);
	PID_Call.attach(&PID_Control, &PID::PID_Control, PID_update_period);

}

void go_home_CW()
{
	PID_Control.set_Trgt_Angle(180.0);
	PID_Control.set_speed(-30.0);
	PID_Call.detach();
	PID_Control.STOP = 0;
	PID_Call.attach(&PID_Control, &PID::Stop_At_Angle, PID_update_period);
}

void go_home_CCW()
{
	PID_Control.set_Trgt_Angle(180.0);
	PID_Control.set_speed(30.0);
	PID_Call.detach();
	PID_Control.STOP = 0;
	PID_Call.attach(&PID_Control, &PID::Stop_At_Angle, PID_update_period);
}
