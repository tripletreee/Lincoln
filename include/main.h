#ifndef F28069_MAIN_H
#define F28069_MAIN_H

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#include "gpio.h"
#include "pwm.h"
#include "pid.h"
#include "ecap.h"


__interrupt void cpu_timer0_isr(void);

inline void Init_PIE_Vector_Table(void){
    
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
}

inline void Enable_interrupts(void){
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
}

inline void Init_system(void){
    // Step 1. Initialize System Control:
    InitSysCtrl();

    // Step 2. Initalize GPIO:
    Init_GPIO();  // Initialize the GPIO

    // Step 3. Clear all interrupts and initialize PIE vector table:
    Init_PIE_Vector_Table();

    // Step 4. Initialize the Device Peripheral. 
    InitCpuTimers();   // Initialize the Cpu Timers
    Init_ePWMs();    // Initialize the ePWM modules
    Init_eCAPs();   // Initialize the eCAP modules

    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 90MHz CPU Freq, 1 millisecond Period (in uSeconds)
    ConfigCpuTimer(&CpuTimer0, 90, 1000); // 1ms for control period

    // Use write-only instruction to set TSS bit = 0
    CpuTimer0Regs.TCR.all = 0x4001;

    // Step 5. User specific code, enable interrupts:
    Enable_interrupts();
}


#endif // end of F28069_MAIN_H definition


