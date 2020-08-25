#ifndef F28069_AUTO_H
#define F28069_AUTO_H

#include "pid.h"

typedef struct _AUTO_Obj_
{
    float command_motor_speed;              // motor speed command: [0,1000]
    float command_servo_position;           // servo angle command: [12000,22500]
    float command_gimbal_position;          // gimbal angle command: [3480,1820]

    float shadow_motor_speed;               // motor speed command: [0,1000]
    float shadow_servo_position;            // servo angle command: [12000,22500]
    float shadow_gimbal_position;           // gimbal angle command: [3480,1820]

    float battery_voltage_uint16;           // battery voltage ADC result
    float battery_voltage_f;

    float motor_position;                   // Motor position
    float motor_position_pre;               // Motor previous position
    float motor_speed;                      // measured motor speed
    float motor_speed_pre;                  // measured motor speed previous
    float motor_pwm;                        // PWM
    float motor_pwm_pre;                    // PWM previous

    float gimbal_position;                  // Gimbal position
    float gimbal_position_pre;              // Gimbal previous position
    float gimbal_speed;                     // Gimbal speed
    float gimbal_speed_pre;                 // Gimbal previous speed
    float gimbal_current;                   // Gimbal current
    float gimbal_current_pre;               // Gimbal previous current
    float gimbal_current_phase;             // Gimbal current phase

    Uint32 gimbal_phase_order;              // Gimbal phase order
    Uint32 gimbal_position_difference;      // Gimbal position difference
    Uint32 gimbal_direction;                // Gimbal direction
    Uint32 gimbal_direction_pre;            // Gimbal previous direction

    Uint32 count_10khz;
    Uint32 current_pointer;

    PID_Obj PID_Motor;
    PID_Obj PID_Gimbal_Position;
    PID_Obj PID_Gimbal_Current;

} AUTO_Obj;

//! \brief Defines the AUTO handle
//!
typedef struct _AUTO_Obj_ *AUTO_Handle;

void Init_Auto(AUTO_Handle auto_handle);


#endif /* F28069_AUTO_H */
