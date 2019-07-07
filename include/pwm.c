#include "pwm.h"

#define EPWM1_TIMER_TBPRD   2999      // Configure the period for the timer 30KHz
#define EPWM2_TIMER_TBPRD   37499     // Configure the PWM period as 300Hz

//
// InitEPwm1
//
void Init_ePWM1(void)
{
    //
    // Setup TBCLK
    //
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm1Regs.TBPRD = EPWM1_TIMER_TBPRD;       // Set timer period
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;     // Not sure, from the example on the manual
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // Same as above
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    //
    // Setup shadow register load on ZERO
    //
    EPwm1Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm1Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm1Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set Compare values
    //
    EPwm1Regs.CMPA.half.CMPA = 0; // Set compare A value

    //
    // Set Actions
    //
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;
}

//
// InitEPwm2
//
void Init_ePWM2(void)
{
    //
    // Setup TBCLK
    //
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm2Regs.TBPRD = EPWM2_TIMER_TBPRD;       // Set timer period
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;     // Not sure, from the example on the manual
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // Same as above
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV8;

    //
    // Setup shadow register load on ZERO
    //
    EPwm2Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm2Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm2Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set Compare values
    //
    EPwm2Regs.CMPA.half.CMPA = 0; // Set compare A value

    //
    // Set Actions
    //
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;
}

void Init_ePWMs(void)
{
    Init_ePWM1();
    Init_ePWM2();
}
