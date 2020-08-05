#ifndef F28069_CLOCK_H
#define F28069_CLOCK_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "F2806x_Examples.h"

#define CPU_FREQ            90E6            // CPU frequency 90 MHz
#define LSPCLK_FREQ         (CPU_FREQ/2)    // CSPCLK frequency 45 MHz
#define MOTOR_PWM_FREQ      30E3            // PWM frequency 30 kHz
#define GIMBAL_PWM_FREQ     30E3            // GIMBAL PWM frequency 30 kHz
#define GIMBAL_CURRENT_FREQ 10E3            // GIMBAL current control frequency 10 kHz
#define GIMBAL_POS_FREQ     1E3             // GIMBAL position control frequency 1 kHz
#define MOTOR_POS_FREQ      1E3             // MOTOR position control frequency 1 kHz

#define SERVO_PWM_FREQ      300             // SERVO PWM frequency 300 Hz


//#define MOTOR_ENC_UPDATE_FREQ 200.0         // Motor encoder update frequency 200 Hz
                                            // Therefore, the highest motor speed is 12000 RPM

//#define MOTOR_SPEED_CTRL_PERIOD (PWM_FREQ/MOTOR_ENC_UPDATE_FREQ)

void Init_SCI_A(void);

void Init_SCI(void);

#endif // end of F28069_SCI_H definition
