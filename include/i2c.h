#ifndef F28069_I2C_H
#define F28069_I2C_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "F2806x_Examples.h"

void Init_I2C(void);

void I2CA_WriteWord(Uint16 SlaveAddress, Uint16 Word);

#endif // end of F28069_I2C_H definition
