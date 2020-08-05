#include "bldc.h"
#include "spi.h"

void Init_Motor_Drvs(void){
    _delay(400000);

    // Enable the DRV8305 for gimbal
    GpioDataRegs.GPBSET.bit.GPIO50 = 1;

    // Release the brake pin for motor
    GpioDataRegs.GPASET.bit.GPIO20 = 1;

    // Set the DRV8305 current amplifier gain
    // 0x00 is x0; 0x2A is x40; 0x3F is x80
    SpiaRegs.SPITXBUF = 0<<15 | (0xa<<11) | 0x3F;
}

int bldc_calculate_phase(int16 position){
    // A-B: 0
    // C-B: 1
    // C-A: 2
    // B-A: 3
    // B-C: 4
    // A-C: 5

    int phase_order;
    float position_difference;

    position_difference = abs(position - BLDC_AB_POS);
    position_difference = position_difference /  TICKS_PER_PHASE;

    return phase_order;
}

void bldc_commute(int phase_order, int direction, int PWM){

    if((phase_order == 1) && (direction == 0) || (phase_order == 4) && (direction == 1)){
        // A -> B
        EPwm4Regs.CMPA.half.CMPA = PWM;
        EPwm4Regs.CMPB = PWM;
        EPwm5Regs.CMPA.half.CMPA = 0;
        EPwm5Regs.CMPB = 0;
        EPwm6Regs.CMPA.half.CMPA = 0;
        EPwm6Regs.CMPB = GIMBAL_HIGH_VALUE;
    }
    else if((phase_order == 2) && (direction == 0) || (phase_order == 5) && (direction == 1)){
        // C -> B
        EPwm4Regs.CMPA.half.CMPA = 0;
        EPwm4Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPA.half.CMPA = 0;
        EPwm5Regs.CMPB = 0;
        EPwm6Regs.CMPA.half.CMPA = PWM;
        EPwm6Regs.CMPB = PWM;
    }
    else if((phase_order == 3) && (direction == 0) || (phase_order == 0) && (direction == 1)){
        // C -> A
        EPwm4Regs.CMPA.half.CMPA = 0;
        EPwm4Regs.CMPB = 0;
        EPwm5Regs.CMPA.half.CMPA = 0;
        EPwm5Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPA.half.CMPA = PWM;
        EPwm6Regs.CMPB = PWM;
    }
    else if((phase_order == 4) && (direction == 0) || (phase_order == 1) && (direction == 1)){
        // B -> A
        EPwm4Regs.CMPA.half.CMPA = 0;
        EPwm4Regs.CMPB = 0;
        EPwm5Regs.CMPA.half.CMPA = PWM;
        EPwm5Regs.CMPB = PWM;
        EPwm6Regs.CMPA.half.CMPA = 0;
        EPwm6Regs.CMPB = GIMBAL_HIGH_VALUE;
    }
    else if((phase_order == 5) && (direction == 0) || (phase_order == 2) && (direction == 1)){
        // B -> C
        EPwm4Regs.CMPA.half.CMPA = 0;
        EPwm4Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm5Regs.CMPA.half.CMPA = PWM;
        EPwm5Regs.CMPB = PWM;
        EPwm6Regs.CMPA.half.CMPA = 0;
        EPwm6Regs.CMPB = 0;
    }
    else if((phase_order == 0) && (direction == 0) || (phase_order == 3) && (direction == 1)){
        // A -> C
        EPwm4Regs.CMPA.half.CMPA = PWM;
        EPwm4Regs.CMPB = PWM;
        EPwm5Regs.CMPA.half.CMPA = 0;
        EPwm5Regs.CMPB = GIMBAL_HIGH_VALUE;
        EPwm6Regs.CMPA.half.CMPA = 0;
        EPwm6Regs.CMPB = 0;
    }
}
