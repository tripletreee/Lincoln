#include "ecap.h"

void Init_eCAP1(void){
    ECap1Regs.ECEINT.all = 0x0000;          // Disable all capture interrupts
    ECap1Regs.ECCLR.all = 0xFFFF;           // Clear all CAP interrupt flags
    ECap1Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped

    //
    // Configure peripheral registers
    //
    ECap1Regs.ECCTL1.bit.FREE_SOFT = 2;     // Emulation not affect
    ECap1Regs.ECCTL1.bit.CAP1POL = 1;       // Falling edge
    ECap1Regs.ECCTL1.bit.CAP2POL = 0;       // Rising edge
    ECap1Regs.ECCTL1.bit.CAP3POL = 1;       // Falling edge
    ECap1Regs.ECCTL1.bit.CAP4POL = 0;       // Rising edge
    ECap1Regs.ECCTL1.bit.CTRRST1 = 0;       // Absolute operation         
    ECap1Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation         
    ECap1Regs.ECCTL1.bit.CTRRST3 = 1;       // Difference operation         
    ECap1Regs.ECCTL1.bit.CTRRST4 = 1;       // Difference operation         
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units
    ECap1Regs.ECCTL1.bit.PRESCALE = 0;      // from 0 to 31, stands for divide by 1, 2, 4, 6 to 62

    ECap1Regs.ECCTL2.bit.CONT_ONESHT = 0;   // Continuous
    ECap1Regs.ECCTL2.bit.STOP_WRAP = 1;     // Stop at 2nd event
    ECap1Regs.ECCTL2.bit.SYNCI_EN = 1;      // Enable sync in
    ECap1Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through

    ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
    ECap1Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
    ECap1Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
    //ECap1Regs.ECEINT.bit.CEVT4 = 1;         // 4 events = interrupt
}

void Init_eCAP2(void){
    ECap2Regs.ECEINT.all = 0x0000;          // Disable all capture interrupts
    ECap2Regs.ECCLR.all = 0xFFFF;           // Clear all CAP interrupt flags
    ECap2Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped

    //
    // Configure peripheral registers
    //
    ECap2Regs.ECCTL1.bit.FREE_SOFT = 2;     // Emulation not affect
    ECap2Regs.ECCTL1.bit.CAP1POL = 1;       // Falling edge
    ECap2Regs.ECCTL1.bit.CAP2POL = 0;       // Rising edge
    ECap2Regs.ECCTL1.bit.CAP3POL = 1;       // Falling edge
    ECap2Regs.ECCTL1.bit.CAP4POL = 0;       // Rising edge
    ECap2Regs.ECCTL1.bit.CTRRST1 = 0;       // Absolute operation         
    ECap2Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation         
    ECap2Regs.ECCTL1.bit.CTRRST3 = 1;       // Difference operation         
    ECap2Regs.ECCTL1.bit.CTRRST4 = 1;       // Difference operation         
    ECap2Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units
    ECap2Regs.ECCTL1.bit.PRESCALE = 0;      // from 0 to 31, stands for divide by 1, 2, 4, 6 to 62

    ECap2Regs.ECCTL2.bit.CONT_ONESHT = 0;   // Continuous
    ECap2Regs.ECCTL2.bit.STOP_WRAP = 1;     // Stop at 2nd event
    ECap2Regs.ECCTL2.bit.SYNCI_EN = 1;      // Enable sync in
    ECap2Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through

    ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
    ECap2Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
    ECap2Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
    //ECap2Regs.ECEINT.bit.CEVT4 = 1;         // 4 events = interrupt
}

void Init_eCAP3(void){
    ECap3Regs.ECEINT.all = 0x0000;          // Disable all capture interrupts
    ECap3Regs.ECCLR.all = 0xFFFF;           // Clear all CAP interrupt flags
    ECap3Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
    ECap3Regs.ECCTL2.bit.TSCTRSTOP = 0;     // Make sure the counter is stopped

    //
    // Configure peripheral registers
    //
    ECap3Regs.ECCTL1.bit.FREE_SOFT = 2;     // Emulation not affect
    ECap3Regs.ECCTL1.bit.CAP1POL = 1;       // Falling edge
    ECap3Regs.ECCTL1.bit.CAP2POL = 0;       // Rising edge
    ECap3Regs.ECCTL1.bit.CAP3POL = 1;       // Falling edge
    ECap3Regs.ECCTL1.bit.CAP4POL = 0;       // Rising edge
    ECap3Regs.ECCTL1.bit.CTRRST1 = 0;       // Absolute operation         
    ECap3Regs.ECCTL1.bit.CTRRST2 = 1;       // Difference operation         
    ECap3Regs.ECCTL1.bit.CTRRST3 = 1;       // Difference operation         
    ECap3Regs.ECCTL1.bit.CTRRST4 = 1;       // Difference operation         
    ECap3Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable capture units
    ECap3Regs.ECCTL1.bit.PRESCALE = 0;      // from 0 to 31, stands for divide by 1, 2, 4, 6 to 62

    ECap3Regs.ECCTL2.bit.CONT_ONESHT = 0;   // Continuous
    ECap3Regs.ECCTL2.bit.STOP_WRAP = 1;     // Stop at 2nd event
    ECap3Regs.ECCTL2.bit.SYNCI_EN = 1;      // Enable sync in
    ECap3Regs.ECCTL2.bit.SYNCO_SEL = 0;     // Pass through

    ECap3Regs.ECCTL2.bit.TSCTRSTOP = 1;     // Start Counter
    ECap3Regs.ECCTL2.bit.REARM = 1;         // arm one-shot
    ECap3Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads
    //ECap3Regs.ECEINT.bit.CEVT4 = 1;         // 4 events = interrupt
}

void Init_eCAPs(void){
    Init_eCAP1();
    Init_eCAP2();
    Init_eCAP3();
}

