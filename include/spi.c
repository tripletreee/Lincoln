#include "spi.h"

void Init_SPI_A(void){
    SpiaRegs.SPICCR.all =0x000F;        // Reset on, rising edge, 16-bit char bits

    SpiaRegs.SPIFFTX.all=0xE040;        // Enable FIFO's, set TX FIFO level to 4
    SpiaRegs.SPIFFRX.all=0x2044;        // Set RX FIFO level to 4
    SpiaRegs.SPIFFCT.all=0x0;           // FIFO control register

    //
    // Enable master mode, normal phase, enable talk, and SPI interrupt disabled.
    //
    SpiaRegs.SPICTL.all = 0x0006;
    
    SpiaRegs.SPIBRR = 22;        // Baud rate; 0x0002
    SpiaRegs.SPIPRI.bit.FREE = 1;  // Set so breakpoints don't disturb xmission

    SpiaRegs.SPICCR.bit.SPISWRESET=1;  // Enable SPI A
}

void Init_SPI_B(void){
    SpibRegs.SPICCR.all =0x000F;        // Reset on, rising edge, 16-bit char bits

    SpibRegs.SPIFFTX.all=0xE040;        // Enable FIFO's, set TX FIFO level to 4
    SpibRegs.SPIFFRX.all=0x2044;        // Set RX FIFO level to 4
    SpibRegs.SPIFFCT.all=0x0;           // FIFO control register

    //
    // Enable master mode, normal phase, enable talk, and SPI interrupt disabled.
    //
    SpibRegs.SPICTL.all = 0x0006;

    SpibRegs.SPIBRR = 0x0002;        // Baud rate
    SpibRegs.SPIPRI.bit.FREE = 1;  // Set so breakpoints don't disturb xmission

    SpibRegs.SPICCR.bit.SPISWRESET=1;  // Enable SPI B
}

void Init_SPI(void){
    Init_SPI_A();
    Init_SPI_B();
}
