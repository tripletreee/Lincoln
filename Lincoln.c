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

int16  command_motor_speed = 0;             // motor speed command: [0,1000]
Uint16 command_servo_position = 17250;      // servo angle command: [12000,22500]
Uint16 command_gimbal_position = 2645;      // gimbal angle command: [3480,1820]

int16  shadow_motor_speed = 0;              // motor speed command: [0,1000]
Uint16 shadow_servo_position = 17250;       // servo angle command: [12000,22500]
Uint16 shadow_gimbal_position = 2680;       // gimbal angle command: [3480,1820]
//int  PWM_CNT = 0;
//int  SERVO_CNT = 17250;  //when pulse width = 1.5ms, servo at neutral(16875 + 375offset)  left 11250 right 22500

Uint16 battery_voltage_uint16 = 0;                 // battery voltage ADC result
float battery_voltage_f = 0;

int16 encoder_motor_position = 0;
int16 encoder_motor_position_pre = 0;
int16 encoder_gimbal_position = 0;
int16 encoder_gimbal_position_pre = 0;

int16 measured_motor_speed = 0;         // measured motor speed
int16 measured_motor_speed_pre = 0;     // measured motor speed previous
Uint16 measured_gimbal_angle = 0;       // measured gimbal angle

PID_Obj PID_Motor = {3, 0.25, 0, 750, -100, 0, 0, 750, 0, 0, 0, 0};
PID_Handle PID_Motor_Handle = &PID_Motor;

PID_Obj PID_Gimbal = {10, 1, 120, 400, -400, 0, 0, 1500, -1500, 0, 0, 0};
PID_Handle PID_Gimbal_Handle = &PID_Gimbal;

Uint16 ADC_Results[16];

int count_init = 0;
int count_10khz = 0;
int count_1khz = 0;
Uint32 _tmp = 0;

// system state machine
// 0: Initial
// 1: online
// 2: fault
int state_machine = 0; 


void main(void)
{
    // Initialize the system
    Init_System();

    Init_Motor_Drvs();

    // Forever loop
    for(;;){
    }
}


// ADC ISR 10 kHz
__interrupt void adc_isr(void)
{
    int gimbal_phase_order;
    float gimbal_position_difference;
    int gimbal_direction;

    // 1 kHz control loop
    if(count_10khz == 10)
    {
        count_10khz = 0;

        ADC_Get_Results(ADC_Results);
        battery_voltage_uint16 = ADC_Results[4];
        battery_voltage_f = battery_voltage_uint16*4.834e-3;

        count_1khz++;

        // If counts to 10 (100 Hz), update the command
        if(count_1khz >= 10){
            command_motor_speed = shadow_motor_speed;
            command_servo_position = shadow_servo_position;
            command_gimbal_position = shadow_gimbal_position;
            count_1khz = 0;
        }

        EPwm2Regs.CMPA.half.CMPA = command_servo_position;

        // GPIO 12 is test point
        GpioDataRegs.GPATOGGLE.bit.GPIO12 = 1;

        PID_Control(PID_Motor_Handle, command_motor_speed, measured_motor_speed);

        EPwm1Regs.CMPA.half.CMPA = PID_Motor.outputInt;

        PID_Control(PID_Gimbal_Handle, command_gimbal_position, encoder_gimbal_position);

        gimbal_direction = PID_Gimbal.output > 0 ? 1:0; // 0: right turn; 1: left turn;

        gimbal_position_difference = abs(encoder_gimbal_position - BLDC_AB_POS) / TICKS_PER_PHASE;

        gimbal_phase_order = (int)gimbal_position_difference % 6;

        if (encoder_gimbal_position < BLDC_AB_POS)
        {
            gimbal_phase_order = 5 - gimbal_phase_order;
        }
        bldc_commute(gimbal_phase_order, gimbal_direction, PID_Gimbal.outputInt); //PID_Gimbal.output
    }
    count_10khz++;

    //
    // Clear ADCINT1 flag reinitialize for next SOC
    //
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
}

// cpu_timer0_isr. The main ISR for control loop
// Main ISR frequency is 1 kHz
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}


// Motor encoder ISR, update the motor speed
__interrupt void ecap1_isr(void){

    Uint32 ecap1_t1;
    Uint32 ecap1_t2;
    float duty_count;

    ecap1_t1 = ECap1Regs.CAP1;
    ecap1_t2 = ECap1Regs.CAP2;

    duty_count = ((float)ecap1_t1 / (float)ecap1_t2)*4119;

    if(duty_count > 15 && duty_count < 4112){ // if this is a valid frame

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

        measured_motor_speed = measured_motor_speed *0.7 + measured_motor_speed_pre*0.3;
    }
    else{
        encoder_motor_position_pre = encoder_motor_position;
        encoder_motor_position = encoder_motor_position + measured_motor_speed;
    }

    if(count_init<4){
        count_init++;
        measured_motor_speed = 0;
    }

    ECap1Regs.ECCLR.bit.CEVT2 = 1;
    ECap1Regs.ECCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 4
    //
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP4;
}


// Gimbal encoder ISR, update the gimbal position
__interrupt void ecap3_isr(void){

    Uint32 ecap3_t1;
    Uint32 ecap3_t2;
    float duty_count;

    ecap3_t1 = ECap3Regs.CAP1;
    ecap3_t2 = ECap3Regs.CAP2;

    ECap3Regs.ECCLR.bit.CEVT2 = 1;
    ECap3Regs.ECCLR.bit.INT = 1;

    duty_count = ((float)ecap3_t1 / (float)ecap3_t2)*4119;

    if(duty_count > 15 && duty_count < 4112){

        encoder_gimbal_position_pre = encoder_gimbal_position;

        encoder_gimbal_position = duty_count - 16;
    }

    //
    // Acknowledge this interrupt to receive more interrupts from group 4
    //
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP4;

}

Uint32 MessageReceivedCount = 0;
Uint32 MDL;
Uint32 MDH;


// CAN Bus interrupt
__interrupt void ecan0_isr(void)
{
    can_ReadMailBox(16, &MDL, &MDH);
    MessageReceivedCount++;
    _tmp++;
    can_SendMailBox(0, _tmp, 0x12345678); // send mailbox0
    ECanaRegs.CANRMP.bit.RMP16 = 1; ////PieCtrlRegs.PIEACK.bit.ACK9 = 1;    // Enables PIE to drive a pulse into the CPU
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    return;
}

//
// End of File
//

