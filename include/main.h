#ifndef F28069_MAIN_H
#define F28069_MAIN_H

// main.h file for including purpose

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#include "string.h"

#include "clock.h"
#include "adc.h"
#include "gpio.h"
#include "pwm.h"
#include "ecap.h"
#include "eqep.h"
#include "can.h"
#include "spi.h"
#include "i2c.h"
#include "sci.h"
#include "cla.h"


#include "pid.h"
#include "bldc.h"


__interrupt void cpu_timer0_isr(void);
__interrupt void ecap1_isr(void);
__interrupt void ecap3_isr(void);
__interrupt void ecan0_isr(void);


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
    PieVectTable.ECAP1_INT = &ecap1_isr;
    PieVectTable.ECAP3_INT = &ecap3_isr;
    PieVectTable.ECAN0INTA = &ecan0_isr;
    EDIS;      // This is needed to disable write to EALLOW protected registers
}

inline void Enable_interrupts(void){
    //
    // Enable CPU INT1 which is connected to CPU-Timer 0
    //
    IER |= M_INT1;
    
    //
    // Enable CPU INT3 which is connected to EPWM1-6 INT
    //
    //IER |= M_INT3;

    //
    // Enable CPU INT4 which is connected to ECAP1-4 INT
    //
    IER |= M_INT4;
    
    //
    // Enable CPU INT6 which is SPI FIFO INT
    //
    //IER |= M_INT6; //0x20; 

    //
    // Enable CPU INT9 which is ECAN, SCI,  interrupt
    //
    IER |= M_INT9;

    //
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    // Enable CPU timer TINT0 in the PIE: Group 1 interrupt 7
    //
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

 
    //
    // Enable EPWM INTn in the PIE: Group 3 interrupt 1-6
    //
    //PieCtrlRegs.PIEIER3.bit.INTx1 = 1;
    //PieCtrlRegs.PIEIER3.bit.INTx4 = 1;

    //
    // Enable eCAP INTn in the PIE: Group 4 interrupt 1-6
    //
    PieCtrlRegs.PIEIER4.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER4.bit.INTx3 = 1;

    //
    // Enable SPI FIFO INTn in the PIE: Group 6 interrupt
    // 
    //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    //PieCtrlRegs.PIEIER6.bit.INTx1=1;     // Enable PIE Group 6, SPIA_RX
    //PieCtrlRegs.PIEIER6.bit.INTx2=1;     // Enable PIE Group 6, SPIA_TX

    //
    // Enable eCAN INTn in the PIE: Group 9 interrupt
    //
    PieCtrlRegs.PIEIER9.bit.INTx5 = 1;  // Enable INTx.5 of INT9 (eCAN0INT)
    //PieCtrlRegs.PIEIER9.bit.INTx6 = 1;  // Enable INTx.6 of INT9 (eCAN1INT)

    //
    // Enable SCI INTn in the PIE: Group 9 interrupt
    //
    //PieCtrlRegs.PIECTRL.bit.ENPIE = 1;    // Enable the PIE block
    //PieCtrlRegs.PIEIER9.bit.INTx1 = 1;    // PIE Group 9, SCIA_RX
    //PieCtrlRegs.PIEIER9.bit.INTx2 = 1;    // PIE Group 9, SCIA_TX

    //
    // Enable CLA TASK1 interrupt: Group 11 interrupt
    //
    //PieCtrlRegs.PIEIER11.bit.INTx1  = 1;



    //
    // Enable global Interrupts and higher priority real-time debug events
    //
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM


}

inline void Init_system(void){

    // Step 1. Initialize System Control:
    InitSysCtrl();

#ifdef FLASH
    memcpy(&RamfuncsRunStart, &RamfuncsLoadStart, (Uint32)&RamfuncsLoadSize);
    memcpy(&Cla1funcsRunStart, &Cla1funcsLoadStart, (Uint32)&Cla1funcsLoadSize);
#endif
    InitFlash();


    // Step 2. Initalize GPIO:
    Init_GPIO();  // Initialize the GPIO



    // Step 3. Clear all interrupts and initialize PIE vector table:
    Init_PIE_Vector_Table();

    // Step 4. Initialize the Device Peripheral. 
    InitCpuTimers();    // Initialize the CPU Timers
    Init_ePWMs();       // Initialize the ePWM modules
    Init_eCAPs();       // Initialize the eCAP modules
    Init_eCANs();       // Initialize the eCAN modules

    // Configure CPU-Timer 0 to interrupt every 500 milliseconds:
    // 90MHz CPU Freq, 1 millisecond Period (in uSeconds)
    ConfigCpuTimer(&CpuTimer0, 90, 1000); // 1ms for control period

    // Use write-only instruction to set TSS bit = 0
    CpuTimer0Regs.TCR.all = 0x4001;

    // Step 5. Enable interrupts:
    Enable_interrupts();
}


#endif // end of F28069_MAIN_H definition


