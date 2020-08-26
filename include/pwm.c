#include "pwm.h"

//
// InitEPwm1
//
void Init_ePWM1(void)
{
    //
    // Setup TBCLK
    //
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm1Regs.TBPRD = MOTOR_HALF_PERIOD;       // Set timer period
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
    EPwm1Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;     // Not sure, from the example on the manual
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // Same as above
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm1Regs.TBCTL.bit.CLKDIV = TB_DIV1;

    EPwm1Regs.ETSEL.bit.SOCAEN  = 1;            // Enable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;  // 1: Fire SOC event when time-base counter equal to zero. 2: equal to period
    EPwm1Regs.ETPS.bit.SOCAPRD  = 3;            // Generate pulse on 3rd event

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
    EPwm1Regs.CMPA.half.CMPA = MOTOR_HIGH_VALUE; // Set compare A value

    //
    // Set Actions
    //
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.CBD = AQ_SET;
}

//
// InitEPwm2
//
void Init_ePWM2(void)
{
    //
    // Setup TBCLK
    //
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm2Regs.TBPRD = SERVO_HALF_PERIOD;        // Set timer period
    EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;      // Disable phase loading
    EPwm2Regs.TBPHS.half.TBPHS = 0x0000;        // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                   // Clear counter
    EPwm2Regs.TBCTL.bit.PRDLD = TB_SHADOW;      // Not sure, from the example on the manual
    EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Same as above
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;    // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm2Regs.TBCTL.bit.CLKDIV = TB_DIV4;

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
    EPwm2Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.CBD = AQ_SET;
}

//
// InitEPwm3
//
void Init_ePWM3(void)
{
    //
    // Setup TBCLK
    //
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm3Regs.TBPRD = MOTOR_HALF_PERIOD;       // Set timer period
    EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;     // Slave module; Used to be TB_DISABLE
    EPwm3Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;     // Shadow mode
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // Sync flow-through
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm3Regs.DBFED = 20; // FED = 30 TBCLKs Dead-Band Generator Rising Edge Delay Count Register
    EPwm3Regs.DBRED = 20; // RED = 30 TBCLKs Dead-Band Generator Falling Edge Delay Count Register

    //
    // Setup shadow register load on ZERO
    //
    EPwm3Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm3Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm3Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set Compare values
    //
    EPwm3Regs.CMPA.half.CMPA = 1600; // Set compare A value
    EPwm3Regs.CMPB = 0; // Set compare B value

    //
    // Set Actions PWMA in Active high mode, PWMB in Active low mode
    //
    EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.CBD = AQ_SET;
}

//
// InitEPwm4
//
void Init_ePWM4(void)
{
    //
    // Setup TBCLK
    //
    EPwm4Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm4Regs.TBPRD = GIMBAL_HALF_PERIOD;       // Set timer period
    EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;      // Disable phase loading
    EPwm4Regs.TBPHS.half.TBPHS = 0x0000;        // Phase is 0
    EPwm4Regs.TBCTR = 0x0000;                   // Clear counter
    EPwm4Regs.TBCTL.bit.PRDLD = TB_SHADOW;      // Not sure, from the example on the manual
    EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Same as above
    EPwm4Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;    // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm4Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm4Regs.DBFED = 20; // FED = 30 TBCLKs Dead-Band Generator Rising Edge Delay Count Register
    EPwm4Regs.DBRED = 20; // RED = 30 TBCLKs Dead-Band Generator Falling Edge Delay Count Register

    //
    // Setup shadow register load on ZERO
    //
    EPwm4Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm4Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm4Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set Compare values
    //
    EPwm4Regs.CMPA.half.CMPA = 0; // Set compare A value
    EPwm4Regs.CMPB = 0; // Set compare B value

    //
    // Set Actions PWMA in Active high mode, PWMB in Active low mode
    //
    EPwm4Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm4Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CBU = AQ_CLEAR;
    EPwm4Regs.AQCTLB.bit.CBD = AQ_SET;
}


//
// InitEPwm5
//
void Init_ePWM5(void)
{
    //
    // Setup TBCLK
    //
    EPwm5Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm5Regs.TBPRD = GIMBAL_HALF_PERIOD;       // Set timer period
    EPwm5Regs.TBCTL.bit.PHSEN = TB_ENABLE;      // Disable phase loading
    EPwm5Regs.TBPHS.half.TBPHS = 0x0000;        // Phase is 0
    EPwm5Regs.TBCTR = 0x0000;                   // Clear counter
    EPwm5Regs.TBCTL.bit.PRDLD = TB_SHADOW;      // Not sure, from the example on the manual
    EPwm5Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Same as above
    EPwm5Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;    // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm5Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm5Regs.DBFED = 20; // FED = 30 TBCLKs Dead-Band Generator Rising Edge Delay Count Register
    EPwm5Regs.DBRED = 20; // RED = 30 TBCLKs Dead-Band Generator Falling Edge Delay Count Register

    //
    // Setup shadow register load on ZERO
    //
    EPwm5Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm5Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm5Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set Compare values
    //
    EPwm5Regs.CMPA.half.CMPA = 0; // Set compare A value
    EPwm5Regs.CMPB = 0; // Set compare B value

    //
    // Set Actions PWMA in Active high mode, PWMB in Active low mode
    //
    EPwm5Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm5Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm5Regs.AQCTLB.bit.CBU = AQ_CLEAR;
    EPwm5Regs.AQCTLB.bit.CBD = AQ_SET;
}


//
// InitEPwm6
//
void Init_ePWM6(void)
{
    //
    // Setup TBCLK
    //
    EPwm6Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm6Regs.TBPRD = GIMBAL_HALF_PERIOD;      // Set timer period
    EPwm6Regs.TBCTL.bit.PHSEN = TB_ENABLE;     // Disable phase loading
    EPwm6Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
    EPwm6Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm6Regs.TBCTL.bit.PRDLD = TB_SHADOW;     // Not sure, from the example on the manual
    EPwm6Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN; // Same as above
    EPwm6Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm6Regs.TBCTL.bit.CLKDIV = TB_DIV1;
    EPwm6Regs.DBFED = 20; // FED = 30 TBCLKs Dead-Band Generator Rising Edge Delay Count Register
    EPwm6Regs.DBRED = 20; // RED = 30 TBCLKs Dead-Band Generator Falling Edge Delay Count Register

    //
    // Setup shadow register load on ZERO
    //
    EPwm6Regs.CMPCTL.bit.SHDWAMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.SHDWBMODE = CC_SHADOW;
    EPwm6Regs.CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
    EPwm6Regs.CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

    //
    // Set Compare values
    //
    EPwm6Regs.CMPA.half.CMPA = 0; // Set compare A value
    EPwm6Regs.CMPB = 0; // Set compare B value

    //
    // Set Actions PWMA in Active high mode, PWMB in Active low mode
    //
    EPwm6Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm6Regs.AQCTLA.bit.CAD = AQ_CLEAR;
    EPwm6Regs.AQCTLB.bit.CBU = AQ_CLEAR;
    EPwm6Regs.AQCTLB.bit.CBD = AQ_SET;
}

void Init_ePWMs(void)
{
    Init_ePWM1();
    Init_ePWM2();
    Init_ePWM3();
    Init_ePWM4();
    Init_ePWM5();
    Init_ePWM6();

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Start all the timers synced
    EDIS;
}
