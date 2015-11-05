#include "mbed.h"
#include "Declarations.h"
#include "power.h"
#include "Motor.h"
#include "Current_Sense.h"
Motor_Ctrl Motor;
Current_Sense CSENSE;
power_mgmt power(Motor, CSENSE);
sense_gain gain;

int go_to_sleep = 0;

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

	CSENSE.set_gain(G50); //Sets the current gain
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
            power.sleep();
            sleep();
            wait(0.1);


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
            	Motor.run_F(0.5);
            }
        }
        i++;
    }
}
