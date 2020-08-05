#ifndef F28069_SCI_H
#define F28069_SCI_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "F2806x_Examples.h"
#include "clock.h"


#define SCI_FREQ    115200
#define SCI_PRD     ((LSPCLK_FREQ/(SCI_FREQ*8))-1)

void Init_SCI_A(void);

void Init_SCI(void);


#endif // end of F28069_SCI_H definition
