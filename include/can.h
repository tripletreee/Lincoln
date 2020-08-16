#ifndef F28069_CAN_H
#define F28069_CAN_H
#include "DSP28x_Project.h"

void Init_eCANs(void);

void can_ReadMailBox(int16 MBXnbr, Uint32 *MDL, Uint32 *MDH);

void can_SendMailBox(int16 MBXnbr, Uint32 MDL, Uint32 MDH);


void MBXwrA(void);  // This function initializes all 32 MBOXes of CAN-A

#endif // end of F28069_CAN_H definition


