#ifndef F28069_BLDC_H
#define F28069_BLDC_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "pwm.h"
#include "util.h"

#define BLDC_AB_POS 2646.0
#define BLDC_POLES 14.0
#define BLDC_TICKS 4096.0
#define TICKS_PER_PHASE (BLDC_TICKS / (BLDC_POLES/2) / 6)
#define PHASES_PER_TICK (1/TICKS_PER_PHASE)

void Init_Motor_Drvs(void);

inline void BLDC_Commute(int *current_pointer, int phase_order, int direction, Uint16 PWM){

    if((phase_order == 1) && (direction == 0) || (phase_order == 4) && (direction == 1)){
        // A -> B
        EPwm4Regs.CMPA.half.CMPA = PWM;
        EPwm4Regs.CMPB = PWM;
        EPwm5Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPB = 0;
        *current_pointer = 1;
    }
    else if((phase_order == 2) && (direction == 0) || (phase_order == 5) && (direction == 1)){
        // C -> B
        EPwm4Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm4Regs.CMPB = 0;
        EPwm5Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPA.half.CMPA = PWM;
        EPwm6Regs.CMPB = PWM;
        *current_pointer = 1;
    }
    else if((phase_order == 3) && (direction == 0) || (phase_order == 0) && (direction == 1)){
        // C -> A
        EPwm4Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm4Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPB = 0;
        EPwm6Regs.CMPA.half.CMPA = PWM;
        EPwm6Regs.CMPB = PWM;
        *current_pointer = 0;
    }
    else if((phase_order == 4) && (direction == 0) || (phase_order == 1) && (direction == 1)){
        // B -> A
        EPwm4Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm4Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPA.half.CMPA = PWM;
        EPwm5Regs.CMPB = PWM;
        EPwm6Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPB = 0;
        *current_pointer = 0;
    }
    else if((phase_order == 5) && (direction == 0) || (phase_order == 2) && (direction == 1)){
        // B -> C
        EPwm4Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm4Regs.CMPB = 0;
        EPwm5Regs.CMPA.half.CMPA = PWM;
        EPwm5Regs.CMPB = PWM;
        EPwm6Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPB = GIMBAL_HIGH_VALUE;
        *current_pointer = 2;
    }
    else if((phase_order == 0) && (direction == 0) || (phase_order == 3) && (direction == 1)){
        // A -> C
        EPwm4Regs.CMPA.half.CMPA = PWM;
        EPwm4Regs.CMPB = PWM;
        EPwm5Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPB = 0;
        EPwm6Regs.CMPA.half.CMPA = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPB = GIMBAL_HIGH_VALUE;
        *current_pointer = 2;
    }
}

#endif // end of F28069_BLDC_H definition


