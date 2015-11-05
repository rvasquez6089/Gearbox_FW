#include "mbed.h"
#include "Declarations.h"
#include "power.h"
#include "Motor.h"
#include "Current_Sense.h"
power_mgmt power;
Motor_Ctrl Motor;
Current_Sense CSENSE;
sense_gain gain;
//Ticker Motor;

int go_to_sleep = 0;
//AnalogIn VBAT(PC_1);

void run_300();
Timeout go_sleep;
bool F = 0;
bool R = 0;




void pressed_F()
{
    printf("Button pressed\n");
    go_to_sleep = !go_to_sleep;
    if(go_to_sleep == 1)
    {
    	go_sleep.detach();

    }
    if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		go_sleep.attach(&pressed_F, 10.0);
    		F = 1;
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
    }
    if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		go_sleep.attach(&pressed_R, 10.0);
    		R = 1;
    		
    	}
	}
}
 


int main()
{

	CSENSE.set_gain(G50);
	//power_mgmt power;
    int i = 0;
    //Motor.shut_off();
    Button_P.fall(&pressed_F);
    Button_D.fall(&pressed_R);
    power.battery_status();

    //Orange.period(0.005);
    go_to_sleep = 1;
    while (1) {
        if (go_to_sleep) {
            
            F = 0;
            R = 0;
            myled = 0;
            //printf("%d: Entering sleep (press user button to resume)\n", i);
            //Motor.detach();
            SEL1 = 0;
            SEL2 = 0;
            CSENSE_EN = 0;
            Orange = 0;
            Motor.off();
            sleep();
            //deepsleep();
            wait(0.1);
            SEL1=0;
            SEL2=1;
            CSENSE_EN = 1;
        } else {
            //printf("%d: Running\n", i);
            myled = 1;
            Orange = 1;  
            if(R && !F)
            {
            	Motor.run_R(0.5);
            }
            if(F && !R)
            {
            	Motor.run_F(0.1);
            }
        }
        i++;
    }
}
