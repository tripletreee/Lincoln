#include "gpio.h"

void Init_GPIO(void)
{
    EALLOW;

    GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;    // Disable pull-up on GPIO0 (EPWM1A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 1;   // Configure GPIO0 as EPWM1A

    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;    // Disable pull-up on GPIO2 (EPWM2A)
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 1;   // Configure GPIO2 as EPWM2A

    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;  // Configure GPIO21 as GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;   // GPIO21 = output
    GpioDataRegs.GPASET.bit.GPIO21 = 0;   // output set low (front: low; back: high)

    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 0;   // Enable pull-up on GPIO24 (CAP1)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0; // Synch to SYSCLKOUT GPIO24 (CAP1)
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;  // Configure GPIO24 as CAP1

    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 0;	// Enable pull-up on GPIO25 (CAP2)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0; // Synch to SYSCLKOUT GPIO25 (CAP2)
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 1;	   // Configure GPIO25 as CAP2

    GpioCtrlRegs.GPAPUD.bit.GPIO26 = 0;    // Enable pull-up on GPIO26 (CAP3)
    GpioCtrlRegs.GPAQSEL2.bit.GPIO26 = 0; // Synch to SYSCLKOUT GPIO26 (CAP3)
    GpioCtrlRegs.GPAMUX2.bit.GPIO26 = 1;    // Configure GPIO26 as CAP3

    EDIS;
}


