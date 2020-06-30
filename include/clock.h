#ifndef F28069_CLOCK_H
#define F28069_CLOCK_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "F2806x_Examples.h"

#define CPU_FREQ        90E6            // CPU frequency 90 MHz
#define LSPCLK_FREQ     (CPU_FREQ/2)    // CSPCLK frequency 45 MHz
#define PWM_FREQ        45E3            // PWM frequency 45 kHz
#define MOTOR_ENC_UPDATE_FREQ 200.0       // Motor encoder update frequency 200 Hz
                                        // Therfore, the highest motor speed is 12000 RPM

#define MOTOR_SPEED_CTRL_PERIOD (PWM_FREQ/MOTOR_ENC_UPDATE_FREQ)

void Init_SCI_A(void);

void Init_SCI(void);

#endif // end of F28069_SCI_H definition
