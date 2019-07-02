//###########################################################################
//
// FILE:   Lincoln.c
//
// TITLE:  Lincoln Racing
//
//!  This example configures CPU Timer0 for a 0.001s period, and update
//!  the PID controller output for the gimbal.
//!
//!  \b Watch \b Variables \n
//!  - CpuTimer0.InterruptCount
//
//! \b External \b Connections \n
//!  - EPWM1A is on GPIO0
//!  - EPWM1B is on GPIO1
//!  - EPWM2A is on GPIO2
//!  - EPWM2B is on GPIO3
//!  - EPWM3A is on GPIO4
//!  - EPWM3B is on GPIO5
//
//###########################################################################

//
// Included Files
//
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
/*   xinkai*/
//
// Defines
//

#define EPWM1_TIMER_TBPRD   2999       // Configure the period for the timer 30KHz
#define EPWM2_TIMER_TBPRD   37499     // Configure the PWM period as 300Hz
#define EPWM1_MAX_CMPA     1500        // Just for test
#define CONTROL_Ts     0.001       // Control period 0.001s

//
// Function Prototypes
//
__interrupt void cpu_timer0_isr(void);
void InitEPwm1Example(void);
void InitEPwm2Example(void);
double gimbal_PID(double target, double current);

//
// Globals
//
int  PWM_CNT = 0;
int  SERVO_CNT = 16875;  //when pulse width = 1.5ms, servo at neutral  left 11250 right 22500
//int  test_param = 4500;
double gimbal_error_prev = 0;     // record error from the previous control cycle for the Kd term
double gimbal_error_integral = 0; // Accumulate the error for the Ki term

//
// Main
//
void main(void)
{
    //
    // Step 1. Initialize System Control:
    // PLL, WatchDog, enable Peripheral Clocks
    // This example function is found in the F2806x_SysCtrl.c file.
    //
    InitSysCtrl();

    //
    // Step 2. Initalize GPIO:
    // This example function is found in the F2806x_Gpio.c file and
    // illustrates how to set the GPIO to it's default state.
    //
    // InitGpio();  // Skipped for this example
    //
    // For this case just init GPIO pins for ePWM1, ePWM2, ePWM3
    // These functions are in the F2806x_EPwm.c file
    //
    InitEPwm1Gpio();
    InitEPwm2Gpio();
    //InitEPwm3Gpio();

    //
    // Step 3. Clear all interrupts and initialize PIE vector table:
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    // This function is found in the F2806x_PieCtrl.c file.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR).
    // This will populate the entire table, even if the interrupt
    // is not used in this example.  This is useful for debug purposes.
    // The shell ISR routines are found in F2806x_DefaultIsr.c.
    // This function is found in F2806x_PieVect.c.
    //
    InitPieVectTable();

    //
    // Interrupts that are used in this example are re-mapped to
    // ISR functions found within this file.
    //
    EALLOW;    // This is needed to write to EALLOW protected registers
    PieVectTable.TINT0 = &cpu_timer0_isr; // Start the CPU timer
    EDIS;      // This is needed to disable write to EALLOW protected registers

    //
    // Step 4. Initialize the Device Peripheral. This function can be
    //         found in F2806x_CpuTimers.c
    //
    InitCpuTimers();   // For this example, only initialize the Cpu Timers
    InitEPwm1Example();
    InitEPwm2Example();

    //
    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
    //
    //PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    //PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    //PieCtrlRegs.PIEIER3.bit.INTx3 = 1;

    //
    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 80MHz CPU Freq, 50 millisecond Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 90, 1000000); //1s just for testing
    //ConfigCpuTimer(&CpuTimer0, 90, CONTROL_Ts*1000000); //Timer period = control period

    //
    // To ensure precise timing, use write-only instructions to write to the
    // entire register. Therefore, if any of the configuration bits are changed
    // in ConfigCpuTimer and InitCpuTimers (in F2806x_CpuTimers.h), the
    // below settings must also be updated.
    //
    
    //
    // Use write-only instruction to set TSS bit = 0
    //
    CpuTimer0Regs.TCR.all = 0x4001;

    //
    // Step 5. User specific code, enable interrupts:
    //

    //
    // Enable CPU INT1 which is connected to CPU-Timer 0
    //
    IER |= M_INT1;

    //
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

    //
    // Enable global Interrupts and higher priority real-time debug events
    //
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

    //
    // Step 6. IDLE loop. Just sit and loop forever (optional)
    //
    for(;;);
}

//
// InitEPwm1Example -
//
void
InitEPwm1Example(void)
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
    EPwm1Regs.CMPA.half.CMPA = PWM_CNT; // Set compare A value
    // EPwm3Regs.CMPB = EPWM3_MAX_CMPB;           // Set Compare B value

    //
    // Set Actions
    //
    EPwm1Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm1Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm1Regs.AQCTLB.bit.CBU = AQ_CLEAR;
}

//
// InitEPwm1Example -
//
void
InitEPwm2Example(void)
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
    EPwm2Regs.CMPA.half.CMPA = SERVO_CNT; // Set compare A value
    // EPwm3Regs.CMPB = EPWM3_MAX_CMPB;           // Set Compare B value

    //
    // Set Actions
    //
    EPwm2Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm2Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm2Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm2Regs.AQCTLB.bit.CBU = AQ_CLEAR;
}

//
// cpu_timer0_isr - 
//
__interrupt void
cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    
    //PWM_CNT = PWM_CNT + test_param;
    //test_param = -test_param;
    //EPwm1Regs.CMPA.half.CMPA = PWM_CNT;
      EPwm2Regs.CMPA.half.CMPA = SERVO_CNT;

    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


// Gimbal PID controller
double gimbal_PID(double target, double current){
    const double Kp = 1.0;
    const double Ki = 0.8;
    const double Kd = 0.0;
    const double limit_p = 0.5; // Integral anti windup (upper bound)
    const double limit_n = -0.5; // Integral anti windup (lower bound)
    double error = 0.0; // velocity error
    double p_value = 0.0; // Proportional term output
    double i_value = 0.0; // Integral term output
    double d_value = 0.0; // Derivative term output
    double PID_out; // Output value of the controller

    error = target - current;
    p_value = Kp * error; // Proportional term
    gimbal_error_integral = gimbal_error_integral + Ki * error * CONTROL_Ts; // Integration in discrete-time
    i_value = gimbal_error_integral; // Integral term
    if (i_value >= limit_p) { i_value = limit_p;} // Integral anti windup
    else if (i_value <= limit_n) { i_value = limit_n;}
    gimbal_error_integral = i_value;
    d_value = Kd * (error - gimbal_error_prev)/CONTROL_Ts; // Derivative term
    gimbal_error_prev = error; // record the error value for next cycle
    PID_out = p_value + i_value + d_value;

    return PID_out;
}

//
// End of File
//

