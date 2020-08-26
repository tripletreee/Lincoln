#include "auto.h"

void Init_Auto(AUTO_Handle auto_handle){

    AUTO_Obj *auto_obj = (AUTO_Obj *) auto_handle;

    auto_obj->command_motor_speed = 0;          // motor speed command: [0,1000]
    auto_obj->command_servo_position = 17250;   // servo angle command: [12000,22500]
    auto_obj->command_gimbal_position = 2680;   // gimbal angle command: [3480,1820]

    auto_obj->shadow_motor_speed = 0;           // motor speed command: [0,1000]
    auto_obj->shadow_servo_position = 17250;    // servo angle command: [12000,22500]
    auto_obj->shadow_gimbal_position = 2680;    // gimbal angle command: [3480,1820]

    auto_obj->motor_position = 2680;
    auto_obj->motor_position_pre = 2680;
    auto_obj->motor_speed = 0;
    auto_obj->motor_speed_pre = 0;
    auto_obj->motor_pwm = 0;
    auto_obj->motor_pwm_pre = 0;

    auto_obj->gimbal_position = 0;
    auto_obj->gimbal_position_pre = 0;
    auto_obj->gimbal_speed = 0;
    auto_obj->gimbal_speed_pre = 0;
    auto_obj->gimbal_current = 0;
    auto_obj->gimbal_current_pre = 0;
    auto_obj->gimbal_current_phase = 0;

    auto_obj->gimbal_phase_order = 0;
    auto_obj->gimbal_position_difference = 0;
    auto_obj->gimbal_direction = 0;
    auto_obj->gimbal_direction_pre = 0;
    auto_obj->gimbal_current_sample_num = 0;

    auto_obj->count_10khz = 0;

    PID_Obj PID_Motor = {3, 0.25, 0, 750, -100, 0, 0, 750, 0, 0, 0, 0};
    PID_Obj PID_Gimbal_Position = {8, 0.0001, 80, 1000, -1000, 0, 0, 1500, -1500, 0, 0, 0};
    PID_Obj PID_Gimbal_Current  = {1.3, 0.005, 0, 1300, -1300, 0, 0, 1500-GIMBAL_DEAD_XING, -1500+GIMBAL_DEAD_XING, 0, 0, 0};

    auto_obj->PID_Motor = PID_Motor;
    auto_obj->PID_Gimbal_Position = PID_Gimbal_Position;
    auto_obj->PID_Gimbal_Current  = PID_Gimbal_Current;
}
