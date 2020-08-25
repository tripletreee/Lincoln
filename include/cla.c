#include "cla.h"

//
// Task 1 (C) Variables
//


void Init_CLA(void){

    EALLOW;
    Cla1Regs.MVECT1 = (Uint16)((Uint32)&Cla1Task1 -(Uint32)&Cla1Prog_Start);
    Cla1Regs.MPISRCSEL1.bit.PERINT1SEL  = CLA_INT1_NONE;
    Cla1Regs.MMEMCFG.all = CLA_PROG_ENABLE|CLARAM0_ENABLE|CLARAM1_ENABLE|CLARAM2_ENABLE|CLA_RAM1CPUE;
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.bit.INT1 = 1;
    EDIS;
}
