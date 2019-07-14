//###########################################################################
//
// FILE:   Lincoln.c
//
// TITLE:  Lincoln Racing
//
//!  This project is for the control part of an autonomous driving vehicle.
//!  the PID controller output for the gimbal.
//
//###########################################################################

#include "include/main.h"

int16  command_motor_speed = 0;         // motor speed command: [0,1000]
Uint16 command_servo_angle = 17250;     // servo angle command: [12000,22500]
Uint16 command_gimbal_angle = 2645;     // gimbal angle command: [3480,1820]

int16  shadow_motor_speed = 0;         // motor speed command: [0,1000]
Uint16 shadow_servo_angle = 17250;     // servo angle command: [12000,22500]
Uint16 shadow_gimbal_angle = 2645;     // gimbal angle command: [3480,1820]
int  PWM_CNT = 150;
int  GIMBAL_CNT_4A = 300;
int  GIMBAL_CNT_4B = 300;
int  GIMBAL_CNT_5A = 0;
int  GIMBAL_CNT_5B = 0;
int  GIMBAL_CNT_6A = 0;
int  GIMBAL_CNT_6B = 3000;
int  SERVO_CNT = 17250;  //when pulse width = 1.5ms, servo at neutral(16875 + 375offset)  left 11250 right 22500

int16 encoder_motor_position = 0;
int16 encoder_motor_position_pre = 0;
int16 encoder_gimbal_position = 0;

int16 measured_motor_speed = 0;       // measured motor speed
int16 measured_motor_speed_pre = 0;    // measured motor speed previous
Uint16 measured_gimbal_angle = 0;     // measured gimbal angle

int  PWM_CNT = 0;

PID_Obj PID_Motor = {6, 0.5, 0, 1500, -100, 0, 0, 1500, 0, 0, 0, 0};
PID_Handle PID_Motor_Handle = &PID_Motor;

PID_Obj PID_Gimbal;
PID_Handle PID_Gimbal_Handle;

//PID_Motor = {1, 1, 1, 1, 1, 1, 1, 1, 1};

// system state machine
// 0: Initial
// 1: online
// 2: fault
int state_machine = 0; 


void main(void)
{
    // Initialize the system
    Init_system();

    // Forever loop
    for(;;){
    }
}

int count_1khz = 0;
int count_300hz = 0;

// cpu_timer0_isr
// The main ISR for control loop
__interrupt void cpu_timer0_isr(void)
{
    int32 ecap2_t1;
    int32 ecap2_t2;
    int32 ecap3_t1;
    int32 ecap3_t2;

    // Main ISR frequency is 1 kHz

    CpuTimer0.InterruptCount++;
    count_1khz++;

    
    //EPwm1Regs.CMPA.half.CMPA = PWM_CNT;
    //EPwm2Regs.CMPA.half.CMPA = SERVO_CNT;
    EPwm4Regs.CMPA.half.CMPA = GIMBAL_CNT_4A;
    EPwm4Regs.CMPB = GIMBAL_CNT_4B;
    EPwm5Regs.CMPA.half.CMPA = GIMBAL_CNT_5A;
    EPwm5Regs.CMPB = GIMBAL_CNT_5B;
    EPwm6Regs.CMPA.half.CMPA = GIMBAL_CNT_6A;
    EPwm6Regs.CMPB = GIMBAL_CNT_6B;

    //ecap1_t1 = ECap1Regs.CAP1;
    //ecap1_t2 = ECap1Regs.CAP2;
    ecap2_t1 = ECap2Regs.CAP1;
    ecap2_t2 = ECap2Regs.CAP2;
    ecap3_t1 = ECap3Regs.CAP1;
    ecap3_t2 = ECap3Regs.CAP2;

    encoder_gimbal_position = ((float)ecap3_t1 / (float)ecap3_t2)*4119 - 16;

    // If counts to 10 (100 Hz), update the command
    if(count_1khz >= 10){
        command_motor_speed = shadow_motor_speed;
        command_servo_angle = shadow_servo_angle;
        command_gimbal_angle = shadow_gimbal_angle;
        //CpuTimer0.InterruptCount = 0;
        count_1khz = 0;
    }
    EPwm2Regs.CMPA.half.CMPA = command_servo_angle;

    GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


__interrupt void ecap1_isr(void)
{

    ECap1Regs.ECCLR.bit.CEVT2 = 1;
    ECap1Regs.ECCLR.bit.INT = 1;
    ECap1Regs.ECCTL2.bit.REARM = 1;

    int32 ecap1_t1;
    int32 ecap1_t2;
    float duty_count;

    ecap1_t1 = ECap1Regs.CAP1;
    ecap1_t2 = ECap1Regs.CAP2;
    duty_count = ((float)ecap1_t1 / (float)ecap1_t2)*4119;


    //EPwm1Regs.CMPA.half.CMPA = PWM_CNT;

    if(duty_count > 14){ // if this is a valid frame

        encoder_motor_position_pre = encoder_motor_position;

        encoder_motor_position = duty_count - 16;

        measured_motor_speed_pre = measured_motor_speed;

        if( abs(encoder_motor_position - encoder_motor_position_pre ) < 2000)
        {
            measured_motor_speed = encoder_motor_position_pre - encoder_motor_position;
        }
        else
        {
            if(encoder_motor_position_pre < 2000)
            { // car is moving forward
                measured_motor_speed = encoder_motor_position_pre + 4096 - encoder_motor_position;
            }
            else{
                measured_motor_speed = encoder_motor_position_pre - 4096 - encoder_motor_position;
            }
        }
        measured_motor_speed = measured_motor_speed *0.4 + measured_motor_speed_pre*0.6;

        PID_Control(PID_Motor_Handle, command_motor_speed, measured_motor_speed);

        EPwm1Regs.CMPA.half.CMPA = PID_Motor.outputInt;
    }



    //
    // Acknowledge this interrupt to receive more interrupts from group 4
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}

//
// End of File
//

