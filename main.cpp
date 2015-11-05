#include "mbed.h"
#include "Declarations.h"
#include "power.h"
#include "Motor.h"
#include "Current_Sense.h"
#include "MMA8652.h"

Serial pc(USBTX, USBRX);
const double PI= 3.14159265;
MMA8652 acc( PB_9, PB_8);
float degrees;
float acc_data[3];
Timer t;

Motor_Ctrl Motor;
Current_Sense CSENSE;
power_mgmt power(Motor, CSENSE);
sense_gain gain;
int go_to_sleep = 0;
Timeout go_sleep;
bool F = 0;
bool R = 0;

int get_int(float input)
{
	return static_cast<int>(input);
}
int get_dec(float input)//gets 4 decimal places
{
	input = input *10000.0;
	input = static_cast<int>(input) %10000;
	return static_cast<int>(input);
}


void pressed_F()
{
    printf("Button pressed\n");
    go_to_sleep = !go_to_sleep;
    if(go_to_sleep == 1)
    {
    	t.stop();
    	go_sleep.detach();

    }
    if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		go_sleep.attach(&pressed_F, 5.0);
    		t.start();
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
    	t.stop();
    	go_sleep.detach();
    }
    if(go_to_sleep == 0)
    {
    	if(!(power.battery_status()))
    	{
    		go_sleep.attach(&pressed_R, 5.0);
    		t.start();
    		R = 1;
    		
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
    pc.baud(921600);
    //Orange.period(0.005);
    go_to_sleep = 1;
    while (1)
    {
        if (go_to_sleep)
        {
            
            F = 0;
            R = 0;
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
            if(R && !F)
            {
            	Motor.run_R(0.5);
            	acc.ReadXYZ(acc_data);
                degrees = atan2 (-1.0f*(acc_data[0]),-1.0f*(acc_data[1])) * 180 / PI;
                pc.printf ("%d.%d, %d.%d, %d.%d, %d.%d, %d\n ",
                		get_int(degrees),get_dec(degrees),
                		get_int(acc_data[0]),get_dec(acc_data[0]),
                		get_int(acc_data[1]),get_dec(acc_data[1]),
                		get_int(acc_data[2]),get_dec(acc_data[2]),
                		t.read_us());
            }
            if(F && !R)
            {
            	Motor.run_F(0.5);
            	acc.ReadXYZ(acc_data);
            	acc.ReadXYZ(acc_data);
            	degrees = atan2 (-1.0f*(acc_data[0]),-1.0f*(acc_data[1])) * 180 / PI;
            	pc.printf ("%d.%d, %d.%d, %d.%d, %d.%d, %d\n ",
						get_int(degrees),get_dec(degrees),
						get_int(acc_data[0]),get_dec(acc_data[0]),
						get_int(acc_data[1]),get_dec(acc_data[1]),
						get_int(acc_data[2]),get_dec(acc_data[2]),
						t.read_us());
            }
        }
    }
}
