#ifndef F28069_CAN_H
#define F28069_CAN_H
#include "F2806x_Device.h"     // F2806x Headerfile Include File

void Init_eCANs(void);

void can_ReadMailBox(int16 MBXnbr, Uint32 *MDL, Uint32 *MDH);

void can_SendMailBox0(void);


#endif // end of F28069_CAN_H definition


