#include "mbed.h"
#include "Declarations.h"
#include "power.h"
power_mgmt power;
PwmOut MOT_C(PA_9);
DigitalOut MOT_B(PA_8);
PwmOut MOT_D(PA_10);
DigitalOut MOT_A(PA_7);
//DigitalOut Orange(PB_0);

Ticker Motor;
int go_to_sleep = 0;
AnalogIn CSENSE(PC_0);
//AnalogIn VBAT(PC_1);
DigitalOut SEL1(PA_11);
DigitalOut SEL2(PA_12);
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
 
void run_300_F()
{

	float current;
	int output;
	MOT_D = 0.0;
	MOT_A = 0;
	MOT_B = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/100.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= 300)
	{
		MOT_C = MOT_C - (0.01f*(((output-300)/1)));
	}
	if(output < 300)
	{
		MOT_C = 0.9f;
	}
}

void run_1200_F()
{

	float current;
	int output;


	MOT_D = 0.0;
	MOT_A = 0;
	MOT_B = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/100.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= 1200)
	{
		MOT_C = MOT_C - (0.01f*(((output-1200)/1)));
	}
	if(output < 1200)
	{
		MOT_C = 1.0f;
	}
}

void run_1500_F()
{

	float current;
	int output;


	MOT_D = 0.0;
	MOT_A = 0;
	MOT_B = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/100.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= 1500)
	{
		MOT_C = MOT_C - (0.01f*(((output-1500)/1)));
	}
	if(output < 1500)
	{
		MOT_C = 1.0f;
	}
}


void run_300_R()
{

	float current;
	int output;
	MOT_B = 0.0;
	MOT_C = 0;
	MOT_A = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/100.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= 300)
	{
		MOT_D = MOT_D - (0.01f*(((output-300)/1)));
	}
	if(output < 300)
	{
		MOT_D = 0.9f;
	}
}

void run_1500_R()
{

	float current;
	int output;
	MOT_B = 0.0;
	MOT_C = 0;
	MOT_A = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/100.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= 1500)
	{
		MOT_D = MOT_D - (0.01f*(((output-1500)/1)));
	}
	if(output < 1500)
	{
		MOT_D = 1.0f;
	}
}

void run_4500_R()
{

	float current;
	int output;
	MOT_B = 0.0;
	MOT_C = 0;
	MOT_A = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/25.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= 4500)
	{
		MOT_D = MOT_D - (0.01f*(((output-4500)/1)));
	}
	if(output < 4500)
	{
		MOT_D = 1.0f;
	}
}

void run_4500_F()
{

	float current;
	int output;


	MOT_D = 0.0;
	MOT_A = 0;
	MOT_B = 1;
	current = 0.0;
	for(int i = 0; i < 10; i++)
	{
		current = current + CSENSE.read();
		//wait_us(10);
	}
	current = current/10;
	current  = ((current*3.3f)/100.0f)/(0.015f);
	current = current*1000.0f;
	output = static_cast<int>(current);
	//pc.printf("Motor Current = %d mA\r\n",output );

	//wait(0.001);
	if(output >= 4500)
	{
		MOT_C = MOT_C - (0.01f*(((output-4500)/1)));
	}
	if(output < 4500)
	{
		MOT_C = 1.0f;
	}
}

int main()
{
	//power_mgmt power;
    int i = 0;
    MOT_A = 0;
    MOT_B = 0;
    MOT_C = 0.0f;
    MOT_D = 0.0f;
    //Orange = 0.0f;
    Button_P.fall(&pressed_F);
    Button_D.fall(&pressed_R);
    power.battery_status();
    MOT_C.period(0.00005);
    MOT_D.period(0.00005);
    //Orange.period(0.005);
    go_to_sleep = 1;
    while (1) {
        if (go_to_sleep) {
            
            F = 0;
            R = 0;
            myled = 0;
            //printf("%d: Entering sleep (press user button to resume)\n", i);
            Motor.detach();
            SEL1 = 0;
            SEL2 = 0;
            CSENSE_EN = 0;
            Orange = 0;
            MOT_D = 0.0f;
			MOT_A = 0;
			MOT_C = 0.0f;
            MOT_B = 0;
            //Orange = 0.0f;
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
            	run_4500_R();
            }
            if(F && !R)
            {
            	run_4500_F();
            }
        }
        i++;
    }
}
