#include "gpio.h"

void Init_GPIO(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A

    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A

    GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;  // Configure GPIO12 as GPIO (TP2)
    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;   // GPIO12 = output
    GpioDataRegs.GPASET.bit.GPIO12 = 0;   // output set low
    
    GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;    // Disable pull-up on GPIO6 (EPWM4A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 1;   // Configure GPIO6 as EPWM4A

    GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;    // Disable pull-up on GPIO7 (EPWM4B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 1;   // Configure GPIO7 as EPWM4B

    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;    // Disable pull-up on GPIO8 (EPWM5A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 1;   // Configure GPIO8 as EPWM5A

    GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;    // Disable pull-up on GPIO9 (EPWM5B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 1;   // Configure GPIO0 as EPWM5B

    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;    // Disable pull-up on GPIO10 (EPWM6A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 1;   // Configure GPIO10 as EPWM6A

    GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;     // Disable pull-up on GPIO11 (EPWM6B)
    GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 1;    // Configure GPIO11 as EPWM6B

    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1;    // Configure GPIO16 as SPISIMOA
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1;    // Configure GPIO16 as SPISOMIA
    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1;    // Configure GPIO16 as SPICLKA
    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1;    // Configure GPIO16 as SPISTEA

    GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;    // Configure GPIO20 as GPIO (Motor Brake)
    GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;     // GPIO20 = output
    GpioDataRegs.GPACLEAR.bit.GPIO20 = 1;   // output set low (low: brake; high: release)

    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;    // Configure GPIO21 as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;     // GPIO21 = output
    GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;   // output set low (front: low; back: high)

    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;     // Enable pull-up on GPIO24 (CAP1)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0;   // Synch to SYSCLKOUT GPIO24 (CAP1)
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;    // Configure GPIO24 as CAP1

    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;	    // Enable pull-up on GPIO25 (CAP2)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0;   // Synch to SYSCLKOUT GPIO25 (CAP2)
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 1;	// Configure GPIO25 as CAP2

    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;     // Enable pull-up on GPIO26 (CAP3)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 0;   // Synch to SYSCLKOUT GPIO26 (CAP3)
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 1;    // Configure GPIO26 as CAP3

    GpioCtrlRegs.GPAPUD.bit.GPIO28 = 0;     // Enable pull-up on GPIO28 (Fault DRV8305)
    GpioCtrlRegs.GPAMUX2.bit.GPIO28 = 0;    // Configure GPIO28 as GPIO28
    GpioCtrlRegs.GPADIR.bit.GPIO28 = 0;     // GPIO28 = input

    GpioCtrlRegs.GPBMUX2.bit.GPIO50 = 0;    // Configure GPIO50 as GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO50 = 1;     // GPIO50 = output
    GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;   // output set LOW (disable drv8305)

    GpioCtrlRegs.GPAPUD.bit.GPIO30 = 0;     // Enable pull-up for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO30 = 3;   // Asynch qual for GPIO30 (CANRXA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO30 = 1;    // Configure GPIO30 as CAN RX

    GpioCtrlRegs.GPAPUD.bit.GPIO31 = 0;     // Enable pull-up for GPIO31 (CANTXA)
    GpioCtrlRegs.GPAMUX2.bit.GPIO31 = 1;    // Configure GPIO31 as CAN TX

    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 0;    // Configure GPIO56 as GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO56 = 1;     // GPIO56 = output
    GpioDataRegs.GPBCLEAR.bit.GPIO56 = 1;   // output set LOW (LED1)

    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;    // Configure GPIO57 as GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;     // GPIO57 = output
    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;   // output set LOW (LED2)

    GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;    // Configure GPIO58 as GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;     // GPIO58 = output
    GpioDataRegs.GPBCLEAR.bit.GPIO58 = 1;   // output set LOW (LED3)

    EDIS;
}


