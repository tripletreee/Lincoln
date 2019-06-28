//###########################################################################
//
// FILE:   Example_2806xLEDBlink.c
//
// TITLE:  Timer based blinking LED Example
//
//!  \addtogroup f2806x_example_list
//!  <h1>Timer based blinking LED(timed_led_blink)</h1>
//!
//!  This example configures CPU Timer0 for a 500 msec period, and toggles the 
//!  GPIO34 LED once per interrupt. For testing purposes, this example
//!  also increments a counter each time the timer asserts an interrupt.
//!
//!  \b Watch \b Variables \n
//!  - CpuTimer0.InterruptCount
//!
//! \b External \b Connections \n
//!  Monitor the GPIO34 LED blink on (for 500 msec) and off (for 500 msec) on 
//!  the 2806x control card.
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

//
// Defines
//

#define EPWM3_TIMER_TBPRD   8999       // Configure the period for the timer 10KHz
#define EPWM3_MAX_CMPA     4500        // Just for test

//
// Function Prototypes
//
//__interrupt void epwm3_timer_isr(void);
//void InitEPwmTimer(void);
__interrupt void cpu_timer0_isr(void);
void InitEPwm3Example(void);

//
// Globals
//
Uint32  EPwm3TimerIntCount;     //counts entries into PWM3 Interrupt
int  PWM_CNT = 1000;
int  test_param = 4500;

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
    //InitEPwm1Gpio();
    //InitEPwm2Gpio();
    InitEPwm3Gpio();

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
    PieVectTable.TINT0 = &cpu_timer0_isr;
    //PieVectTable.EPWM1_INT = &epwm1_timer_isr;
    EDIS;      // This is needed to disable write to EALLOW protected registers

    //
    // Step 4. Initialize the Device Peripheral. This function can be
    //         found in F2806x_CpuTimers.c
    //
    InitCpuTimers();   // For this example, only initialize the Cpu Timers
    
    //InitEPwmTimer();    // For this example, only initialize the ePWM Timers
    InitEPwm3Example();

    //
    // Initalize counters
    //
    EPwm3TimerIntCount = 0;

    //
    // Enable CPU INT3 which is connected to EPWM1-6 INT
    //
    // IER |= M_INT3;

    //
    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-3
    //
    PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER3.bit.INTx3 = 1;

    //
    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 80MHz CPU Freq, 50 millisecond Period (in uSeconds)
    //
    ConfigCpuTimer(&CpuTimer0, 90, 500000); //0.5s

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
/*
//
// InitEPwmTimer -
//
void
InitEPwmTimer()
{
    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 0;      // Stop all the TB clocks
    EDIS;

    //
    // Setup Sync
    //
    EPwm1Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    //EPwm2Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    //EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through
    //EPwm4Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_IN;  // Pass through

    //
    // Allow each timer to be sync'ed
    //
    EPwm1Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm2Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm3Regs.TBCTL.bit.PHSEN = TB_ENABLE;
    //EPwm4Regs.TBCTL.bit.PHSEN = TB_ENABLE;

    EPwm1Regs.TBPHS.half.TBPHS = PWM_CNT;
    //EPwm2Regs.TBPHS.half.TBPHS = 200;
    //EPwm3Regs.TBPHS.half.TBPHS = 300;
    //EPwm4Regs.TBPHS.half.TBPHS = 400;

    EPwm1Regs.TBPRD = PWM1_TIMER_TBPRD;
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP;    // Count up
    EPwm1Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm1Regs.ETSEL.bit.INTEN = PWM1_INT_ENABLE;  // Enable INT
    EPwm1Regs.ETPS.bit.INTPRD = ET_1ST;           // Generate INT on 1st event
    EPwm1Regs.TBCTL.bit.PRDLD = TB_SHADOW;

    EALLOW;
    SysCtrlRegs.PCLKCR0.bit.TBCLKSYNC = 1;       // Start all the timers synced
    EDIS;
}*/

//
// InitEPwm3Example -
//
void
InitEPwm3Example(void)
{
    //
    // Setup TBCLK
    //
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UP; // Count up
    EPwm3Regs.TBPRD = EPWM3_TIMER_TBPRD;       // Set timer period
    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
    EPwm3Regs.TBPHS.half.TBPHS = 0x0000;       // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm3Regs.TBCTL.bit.PRDLD = TB_SHADOW;     // Not sure, from the example on the manual
    EPwm3Regs.TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE; // Same as above
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = TB_DIV1;   // Clock ratio to SYSCLKOUT Watch out here, not sure if it should be TB_DIV2
    EPwm3Regs.TBCTL.bit.CLKDIV = TB_DIV1;

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
    EPwm3Regs.CMPA.half.CMPA = PWM_CNT; // Set compare A value
    // EPwm3Regs.CMPB = EPWM3_MAX_CMPB;           // Set Compare B value

    //
    // Set Actions
    //
    /*EPwm3Regs.AQCTLA.bit.CAU = AQ_SET;    // Set PWM3A on event B, up count
    EPwm3Regs.AQCTLA.bit.CBU = AQ_CLEAR;  // Clear PWM3A on event B, up count

    EPwm3Regs.AQCTLB.bit.ZRO = AQ_TOGGLE; // Toggle EPWM3B on Zero*/
    EPwm3Regs.AQCTLA.bit.ZRO = AQ_SET;
    EPwm3Regs.AQCTLA.bit.CAU = AQ_CLEAR;
    EPwm3Regs.AQCTLB.bit.ZRO = AQ_SET;
    EPwm3Regs.AQCTLB.bit.CBU = AQ_CLEAR;

    //
    // Interrupt where we will change the Compare Values
    //
    /*EPwm3Regs.ETSEL.bit.INTSEL = ET_CTR_ZERO;     // Select INT on Zero event
    EPwm3Regs.ETSEL.bit.INTEN = 1;                // Enable INT
    EPwm3Regs.ETPS.bit.INTPRD = ET_3RD;           // Generate INT on 3rd event*/

    //
    // Start by increasing the compare A and decreasing compare B
    //
    //EPwm3Regs.CMPA.Direction = 1;
}


//
// epwm1_timer_isr - Interrupt routines uses in this example
//
/*
__interrupt void
epwm1_timer_isr(void)
{
    EPwm1TimerIntCount++;

    //
    // Clear INT flag for this timer
    //
    EPwm1Regs.ETCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 3
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;
}*/

//
// cpu_timer0_isr - 
//
__interrupt void
cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    
    //
    // Toggle GPIO34 once per 500 milliseconds
    //
    //GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
    PWM_CNT = PWM_CNT + test_param;
    test_param = -test_param;
    EPwm3Regs.CMPA.half.CMPA = PWM_CNT;

    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//
// End of File
//

