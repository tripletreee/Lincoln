#ifndef F28069_BLDC_H
#define F28069_BLDC_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "pwm.h"
#include "util.h"

#define BLDC_AB_POS 2646.0
#define BLDC_POLES 14.0
#define BLDC_TICKS 4096.0
#define TICKS_PER_PHASE (BLDC_TICKS / (BLDC_POLES/2) / 6)

int bldc_calculate_phase(int16 position);
void bldc_commute(int phase_order, int direction, int PWM);
void Init_Motor_Drvs(void);

#endif // end of F28069_BLDC_H definition


