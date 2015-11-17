#ifndef DECLARATIONS_H
#define DECLARATIONS_H
#include "mbed.h"
/** pwm_period defines the period of the timers that clock pwm.
 *
 * This constant defines the global period and frequency for all the timers
 * used for PWM in the system. It is important to set the period for all PwmOut
 * pins in the system to this time base otherwise the system will hang and
 * undefined behavior will occur
 */
const float pwm_period = 0.00005; ////F(Hz) = 1/Period F = 20kHz = 1/0.00005

/**  PID_update_period defines how often the pid algorithm corrects the system
 * This constant is also used in determining the time base that filters the
 * incoming XY data from the accelerometer */

const float PID_update_period = 0.01; ////F(Hz) = 1/Period  F = 100Hz = 1/0.01

/** @brief Pin location for Orange leds MOSFET*/
//#define pin_Orange PB_0



//----------LEDS-------------//
extern DigitalOut Orange;
extern DigitalOut myled;
//-----------Push Buttons-----------//
extern InterruptIn Button_P;
extern InterruptIn Button_D;

//-----------Pin Names--------------//


#endif// DECLARATIONS_H
