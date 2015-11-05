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
const float pwm_period = 0.00005; //F(Hz) = 1/Period    20kHz = 1/0.00005


//----------LEDS-------------//
extern DigitalOut Orange;
extern InterruptIn Button_P;
extern InterruptIn Button_D;
extern DigitalOut myled;

#endif// DECLARATIONS_H
