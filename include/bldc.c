#include "bldc.h"
#include "spi.h"

void Init_Motor_Drvs(void){

    // Delay 20ms for sensor update
    DELAY_US(20000);

    // Enable the DRV8305 for gimbal
    GpioDataRegs.GPBSET.bit.GPIO50 = 1;

    // Release the brake pin for motor
    GpioDataRegs.GPASET.bit.GPIO20 = 1;

    // Set the DRV8305 current amplifier gain
    // 0x00 is x10; 0x2A is x40; 0x3F is x80
    SpiaRegs.SPITXBUF = 0<<15 | (0xa<<11) | 0x3F;
}

