#include "sci.h"

//
// Init_SCI A
//
void Init_SCI_A(void){

    SciaRegs.SCIFFTX.all = 0xE040; // Reset SCI, Enable FIFO, reset FIFOTX, disable interrupt, 0 FIFO level
    SciaRegs.SCIFFRX.all = 0x6061; // Reset FIFORX, enable FIFORX interrrupt, 1 word FIFO level
    SciaRegs.SCIFFCT.all = 0x00;

    SciaRegs.SCICCR.all = 0x0007; // 1 stop bit,  No loopback, No parity,8 char bits, async mode,
    
    SciaRegs.SCICTL1.all = 0x0043; // Enable TX and RX, enable error frame interrupt

    SciaRegs.SCICTL2.all = 0x0000; // Disable TX and RX interrupt

    SciaRegs.SCIHBAUD = ((Uint16)SCI_PRD) >> 8; // Set Baud rate 115200
    SciaRegs.SCILBAUD = SCI_PRD;

    SciaRegs.SCIPRI.bit.FREE = 1; // Denotes Priority as free running

    SciaRegs.SCICTL1.bit.SWRESET = 1;       // Relinquish SCI from Reset
    SciaRegs.SCIFFTX.bit.TXFIFOXRESET = 1;
    SciaRegs.SCIFFRX.bit.RXFIFORESET = 1;
}


void Init_SCI(void)
{
    Init_SCI_A();
    return;
}
