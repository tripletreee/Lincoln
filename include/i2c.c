#include "i2c.h"

//
// Init_I2C 
//
void Init_I2C(void)
{
    //
    // Initialize I2C
    //
    I2caRegs.I2CSAR = 0x0060;		// Slave address (MCP4725 with pin A0 to GND)

    I2caRegs.I2CPSC.all = 8;		// Prescaler (need 7-12 Mhz on module clk); module clk frequency = sysclk/(I2CPSC+1)
    I2caRegs.I2CCLKL = 5;			// NOTE: must be non zero
    I2caRegs.I2CCLKH = 5;			// NOTE: must be non zero
    // i2c clock frequency is sysclk/(PSC+1)/(CLKL+d)/(CLKH+d)
    // where d is 5 if PSC>5; d is 7 if PSC is 0; d is 6 if PSC is 1;
    // if sys clock is 90 MHz, i2c clock frequnecy is 90/(8+1)/(10+5)/(5+5) = 66.67 kHz

    // I2C Interrupt Enable register
    I2caRegs.I2CIER.all = 0x00;		// Enable SCD & ARDY interrupts; used to be 0x24
    // SCD: Stop condition detected interrupt enable bit
    // ARDY: Register-access-ready interrupt enable bit

    //
    // Take I2C out of reset. Stop I2C when suspended
    // I2C mode register
    I2caRegs.I2CMDR.all = 0x0020;	// I2C Mode Register; 0x20 means I2C module reset

    I2caRegs.I2CFFTX.all = 0x6000;	// Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2040;	// Enable RXFIFO, clear RXFFINT,

    return;
}

//
// I2C_WriteTwoBytes
//
void I2CA_WriteWord(Uint16 SlaveAddress, Uint16 Word)
{

    //
    // Wait until the STP bit is cleared from any previous master communication.
    // Clearing of this bit by the module is delayed until after the SCD bit is
    // set. If this bit is not checked prior to initiating a new message, the
    // I2C could get confused.
    //
    /*
    if (I2caRegs.I2CMDR.bit.STP == 1)
    {
        return I2C_STP_NOT_READY_ERROR;
    }
    */

    //
    // Setup slave address
    //
    //I2caRegs.I2CMDR.all = 0x0020;
    I2caRegs.I2CSAR = SlaveAddress;

    //
    // Check if bus busy
    //
    /*
    if (I2caRegs.I2CSTR.bit.BB == 1)
    {
        return I2C_BUS_BUSY_ERROR;
    }*/

    //
    // Setup number of bytes
    //
    I2caRegs.I2CCNT = 2;

    //
    // Setup data to send
    //
    Uint16 _word_high = Word & 0x0f00;
    Uint16 _word_low  = Word & 0x00ff;

    I2caRegs.I2CDXR = _word_high >> 8;
    I2caRegs.I2CDXR = _word_low;

    //
    // Send start as master transmitter
    //
    I2caRegs.I2CMDR.all = 0x6E20;
}


