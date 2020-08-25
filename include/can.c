#include "can.h"

//
// Initialize eCAN module
//
void Init_eCANs(void){
    
    struct ECAN_REGS ECanaShadow;
    
    InitECana();            // Initialize eCAN-A module

    MBXwrA();

    //
    // Set mailbox0's ID=0x01, TX
    //
    ECanaMboxes.MBOX0.MSGID.bit.IDE = 0;   // Identifier extension bit.
    ECanaMboxes.MBOX0.MSGID.bit.AME = 0;   // Acceptance mask enable bit.
    ECanaMboxes.MBOX0.MSGID.bit.AAM = 0;   // Auto answer mode bit.
    ECanaMboxes.MBOX0.MSGID.bit.STDMSGID = 0x01;
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_H = 0;
    ECanaMboxes.MBOX0.MSGID.bit.EXTMSGID_L = 0;
    
    //
    // Set mailbox16's ID=0x02, RX
    // 
    ECanaMboxes.MBOX16.MSGID.bit.IDE = 0;   // Identifier extension bit.
    ECanaMboxes.MBOX16.MSGID.bit.AME = 0;   // Acceptance mask enable bit.
    ECanaMboxes.MBOX16.MSGID.bit.AAM = 0;   // Auto answer mode bit.
    ECanaMboxes.MBOX16.MSGID.bit.STDMSGID = 0x02;
    ECanaMboxes.MBOX16.MSGID.bit.EXTMSGID_H = 0;
    ECanaMboxes.MBOX16.MSGID.bit.EXTMSGID_L = 0;

    //
    // Configure Mailbox 0 as Tx, 16 as Rx
    //
    ECanaShadow.CANMD.all = ECanaRegs.CANMD.all;
    ECanaShadow.CANMD.bit.MD0 = 0;
    ECanaShadow.CANMD.bit.MD16 = 1;
    ECanaRegs.CANMD.all = ECanaShadow.CANMD.all;

    // Reset all the TRS bits
//    ECanaShadow.CANTRR.all = 0x0000ffff;
//    ECanaRegs.CANTRR.all = ECanaShadow.CANTRR.all;

    //
    // Enable Mailboxes
    //
    ECanaShadow.CANME.all = ECanaRegs.CANME.all;
    ECanaShadow.CANME.bit.ME0 = 1;  // Enable mailbox 0
    ECanaShadow.CANME.bit.ME16 = 1; // Enable mailbox 16
    ECanaRegs.CANME.all = ECanaShadow.CANME.all;

    ECanaShadow.CANMC.all = ECanaRegs.CANMC.all;
    ECanaShadow.CANMC.bit.SUSP = 1;         // 1: FREE; 0: SUSPEND
    ECanaShadow.CANMC.bit.ABO = 1;          // 1£º Auto bus on; Bus off 128 * 11recessive bits have been monitored
    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

    //
    // Set Mailbox interrupt
    //
    EALLOW;
    ECanaShadow.CANMIL.all  = 0x00000000 ; // Mailbox Interrupt Level 0
    ECanaRegs.CANMIL.all = ECanaShadow.CANMIL.all;

    ECanaShadow.CANMIM.all  = 0x00000000;   // Mailbox Interrupt Mask
//    ECanaShadow.CANMIM.bit.MIM0 = 1;    // Unmask mailbox 0 interrupt
    ECanaShadow.CANMIM.bit.MIM16 = 1;   // Unmask mailbox 16 interrupt
    ECanaRegs.CANMIM.all = ECanaShadow.CANMIM.all;

    ECanaShadow.CANGIM.all = 0; // Global interrupt mask
    ECanaShadow.CANGIM.bit.I0EN = 1;   // Enable eCAN1INT or eCAN0INT
    ECanaShadow.CANGIM.bit.I1EN = 0;
    ECanaRegs.CANGIM.all = ECanaShadow.CANGIM.all;
    EDIS;

    //
    // Specify that 8 bytes will be sent/received
    //
    ECanaMboxes.MBOX0.MSGCTRL.bit.DLC = 8;
    ECanaMboxes.MBOX16.MSGCTRL.bit.DLC = 8;

    //
    // Write to the mailbox RAM field of MBOX0 - 15
    //
    ECanaMboxes.MBOX0.MDL.all = 0x12345678;
    ECanaMboxes.MBOX0.MDH.all = 0x12345678;
}

void can_ReadMailBox(int16 MBXnbr, Uint32 *MDL, Uint32 *MDH){
    volatile struct MBOX *Mailbox;
    Mailbox = &ECanaMboxes.MBOX0 + MBXnbr;

    *MDL = Mailbox->MDL.all;
    *MDH = Mailbox->MDH.all;
}

void can_SendMailBox(int16 MBXnbr, Uint32 MDL, Uint32 MDH){

    struct ECAN_REGS ECanaShadow;
    volatile struct MBOX *Mailbox;
    Mailbox = &ECanaMboxes.MBOX0 + MBXnbr;

//    ECanaShadow.CANMC.bit.CDR = 1;
//    ECanaShadow.CANMC.bit.MBNR = 0;
//    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;
//
//    ECanaShadow.CANTRR.all = 0;
//    ECanaShadow.CANTRR.bit.TRR0 = 1;
//    ECanaRegs.CANTRR.all = ECanaShadow.CANTRR.all;

    Mailbox->MDL.all = MDL;
    Mailbox->MDH.all = MDH;

//    ECanaShadow.CANMC.bit.CDR = 0;
//    ECanaRegs.CANMC.all = ECanaShadow.CANMC.all;

//    ECanaShadow.CANTA.all = 0;
//    ECanaShadow.CANTA.bit.TA0 = 1;       // Clear TA0
//    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;


    ECanaShadow.CANTRS.all = 0;
    ECanaShadow.CANTRS.bit.TRS0 = 1;     // Set Transmit Request Set (TRS) register for mailbox 0
    ECanaRegs.CANTRS.all = ECanaShadow.CANTRS.all;


/*
    do
    {
        ECanaShadow.CANTA.all = ECanaRegs.CANTA.all; //Transmit Acknowledge
    } while(ECanaShadow.CANTA.bit.TA0 == 0 );// Wait for TA0 bit to be set

    ECanaShadow.CANTA.all = 0;
    ECanaShadow.CANTA.bit.TA0 = 1;                  // Clear TA0
    ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;
*/
}

/* Zero-out the MBX RAM of CAN-A */

void MBXwrA()
{
    int j;
    volatile struct MBOX *Mailbox  = (void *) 0x6100;

    for(j=0; j<32; j++)
    {
        Mailbox->MSGID.all = 0;
        Mailbox->MSGCTRL.all = 0;
        Mailbox->MDH.all = 0;
        Mailbox->MDL.all = 0;
        Mailbox = Mailbox + 1;
    }
}


