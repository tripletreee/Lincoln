//###########################################################################
//
// FILE:   Lincoln.c
//
// TITLE:  Lincoln Racing
//
//!  This project is for the control part of an autonomous driving vehicle.
//!
//
//###########################################################################

#include "include/main.h"

int16  command_motor_speed = 0;         // motor speed command: [0,1000]
Uint16 command_servo_position = 17250;     // servo angle command: [12000,22500]
Uint16 command_gimbal_position = 2645;     // gimbal angle command: [3480,1820]

int16  shadow_motor_speed = 0;         // motor speed command: [0,1000]
Uint16 shadow_servo_position = 17250;     // servo angle command: [12000,22500]
Uint16 shadow_gimbal_position = 2680;     // gimbal angle command: [3480,1820]
int  PWM_CNT = 0;
int  SERVO_CNT = 17250;  //when pulse width = 1.5ms, servo at neutral(16875 + 375offset)  left 11250 right 22500

int16 encoder_motor_position = 0;
int16 encoder_motor_position_pre = 0;
int16 encoder_gimbal_position = 0;
int16 encoder_gimbal_position_pre = 0;

int16 measured_motor_speed = 0;       // measured motor speed
int16 measured_motor_speed_pre = 0;    // measured motor speed previous
Uint16 measured_gimbal_angle = 0;     // measured gimbal angle

PID_Obj PID_Motor = {6, 0.5, 0, 1500, -100, 0, 0, 1500, 0, 0, 0, 0};
PID_Handle PID_Motor_Handle = &PID_Motor;

//PID_Obj PID_Gimbal = {25, 3, 300, 800, -800, 0, 0, 3000, -3000, 0, 0, 0};
PID_Obj PID_Gimbal = {20, 2, 240, 800, -800, 0, 0, 3000, -3000, 0, 0, 0};
PID_Handle PID_Gimbal_Handle = &PID_Gimbal;


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

int test = 0 ;
int test1 = 0;
int test2 = 0;
int test3 = 0;


// cpu_timer0_isr. The main ISR for control loop
// Main ISR frequency is 1 kHz
__interrupt void cpu_timer0_isr(void)
{

    int32 ecap2_t1;
    int32 ecap2_t2;
    ecap2_t1 = ECap2Regs.CAP1;
    ecap2_t2 = ECap2Regs.CAP2;

    CpuTimer0.InterruptCount++;
    count_1khz++;

    // If counts to 10 (100 Hz), update the command
    if(count_1khz >= 10){
        command_motor_speed = shadow_motor_speed;
        command_servo_position = shadow_servo_position;
        command_gimbal_position = shadow_gimbal_position;
        //CpuTimer0.InterruptCount = 0;
        count_1khz = 0;
    }
    EPwm2Regs.CMPA.half.CMPA = command_servo_position;

    GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}



// Motor ISR
__interrupt void ecap1_isr(void){
    int32 ecap1_t1;
    int32 ecap1_t2;
    float duty_count;

    ecap1_t1 = ECap1Regs.CAP1;
    ecap1_t2 = ECap1Regs.CAP2;

    //EPwm1Regs.CMPA.half.CMPA = PWM_CNT;

    duty_count = ((float)ecap1_t1 / (float)ecap1_t2)*4119;

    if(duty_count > 15 && duty_count < 4112){ // if this is a valid frame

        encoder_motor_position_pre = encoder_motor_position;

        encoder_motor_position = duty_count - 16;

        measured_motor_speed_pre = measured_motor_speed;

        test = duty_count - 13 - encoder_motor_position;
        test1 = encoder_motor_position;
        test2 = encoder_motor_position_pre;

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
        measured_motor_speed = measured_motor_speed *0.7 + measured_motor_speed_pre*0.3;

        PID_Control(PID_Motor_Handle, command_motor_speed, measured_motor_speed);

        EPwm1Regs.CMPA.half.CMPA = PID_Motor.outputInt;
    }
    else{
        encoder_motor_position_pre = encoder_motor_position;
        encoder_motor_position = encoder_motor_position + measured_motor_speed;
    }

    // Re-enable the event 2 interrupt
    ECap1Regs.ECCLR.bit.CEVT2 = 1;
    // Re-enable the interrupt
    ECap1Regs.ECCLR.bit.INT = 1;


    //
    // Acknowledge this interrupt to receive more interrupts from group 4
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
}


// Gimbal ISR
__interrupt void ecap3_isr(void){
    ECap3Regs.ECCLR.bit.CEVT2 = 1;
    ECap3Regs.ECCLR.bit.INT = 1;
    //
    // Acknowledge this interrupt to receive more interrupts from group 4
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP4;
    int32 ecap3_t1;
    int32 ecap3_t2;
    float duty_count;

    ecap3_t1 = ECap3Regs.CAP1;
    ecap3_t2 = ECap3Regs.CAP2;

    duty_count = ((float)ecap3_t1 / (float)ecap3_t2)*4119;

    if(duty_count > 15 && duty_count < 4112){

        encoder_gimbal_position_pre = encoder_gimbal_position;

        encoder_gimbal_position = (duty_count - 16)*1 + encoder_gimbal_position_pre * 0;

        PID_Control(PID_Gimbal_Handle, command_gimbal_position, encoder_gimbal_position);

        int phase_order;
        float position_difference;
        int direction;
        direction = PID_Gimbal.output > 0 ? 1:0;
        // 0: right turn; 1: left turn;

        position_difference = abs(encoder_gimbal_position - BLDC_AB_POS) / TICKS_PER_PHASE;
        phase_order = (int)position_difference % 6;

        if (encoder_gimbal_position < BLDC_AB_POS)
        {
            phase_order = 5 - phase_order;
        }

        bldc_commute(phase_order, direction, PID_Gimbal.outputInt); //PID_Gimbal.output
    }

}

//
// End of File
//

