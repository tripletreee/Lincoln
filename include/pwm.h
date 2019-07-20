#ifndef F28069_PWM_H
#define F28069_PWM_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "F2806x_Examples.h"

#define EPWM1_TIMER_TBPRD   2999      // Configure the period for the timer 30KHz
#define EPWM2_TIMER_TBPRD   37499     // Configure the PWM period as 300Hz

#define EPWM4_TIMER_TBPRD   2999      // Configure the period for the timer 30KHz
#define EPWM5_TIMER_TBPRD   2999      // Configure the period for the timer 30KHz
#define EPWM6_TIMER_TBPRD   2999      // Configure the period for the timer 30KHz

#define PWM_HIGH_VALUE 3000

void Init_ePWMs(void);

void Init_ePWM1(void);

void Init_ePWM2(void);

void Init_ePWM4(void);

void Init_ePWM5(void);

void Init_ePWM6(void);



#endif // end of F28069_PWM_H definition


