#include "eqep.h"

void Init_EQEP(void){

    EQep1Regs.QUPRD=450000;                 // Unit Timer for 200Hz at 90 MHz SYSCLKOUT

    EQep1Regs.QDECCTL.bit.QSRC = 0;         // QEP quadrature count mode

    EQep1Regs.QEPCTL.bit.FREE_SOFT=2;       // Position counter is unaffected by emulation suspend
    EQep1Regs.QEPCTL.bit.PCRM=1;            // QPOSCNT reset on maximum counts
    EQep1Regs.QEPCTL.bit.UTE=0;             // Unit Timeout Enable
    EQep1Regs.QEPCTL.bit.QCLM=0;            // Latch on unit time out
    EQep1Regs.QEPCTL.bit.IEL=1;             // Index event latch of position counter on rising edge of the index signal
    
    EQep1Regs.QPOSMAX=MOTOR_ENC_LINES-1;
    EQep1Regs.QEPCTL.bit.QPEN=1;            // QEP enable

   // EQep1Regs.QCAPCTL.bit.UPPS=5;           // 1/32 for unit position
   // EQep1Regs.QCAPCTL.bit.CCPS=6;           // 1/64 for CAP clock
   // EQep1Regs.QCAPCTL.bit.CEN=1;            // QEP Capture Enable
}
