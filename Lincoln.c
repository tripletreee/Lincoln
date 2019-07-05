//###########################################################################
//
// FILE:   Lincoln.c
//
// TITLE:  Lincoln Racing
//
//!  This project configures CPU Timer0 for a 0.001s period, and update
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
#include "include/main.h"


//
// Defines
//


#define EPWM1_MAX_CMPA     1500        // Just for test
#define CONTROL_Ts     0.001       // Control period 0.001s

//
// Function Prototypes
//
__interrupt void cpu_timer0_isr(void);
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
    Init_GPIO();  // Initialize the GPIO

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
    InitCpuTimers();   // Initialize the Cpu Timers
    InitEPwms();    // Initialize the ePWM modules


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
// cpu_timer0_isr - 
//
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    
    //PWM_CNT = PWM_CNT + test_param;
    //test_param = -test_param;
    EPwm1Regs.CMPA.half.CMPA = PWM_CNT;
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

