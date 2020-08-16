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
Uint16 command_gimbal_position = 2680;      // gimbal angle command: [3480,1820]

int16  shadow_motor_speed = 0;              // motor speed command: [0,1000]
Uint16 shadow_servo_position = 17250;       // servo angle command: [12000,22500]
Uint16 shadow_gimbal_position = 2680;       // gimbal angle command: [3480,1820]
//int  PWM_CNT = 0;
//int  SERVO_CNT = 17250;  //when pulse width = 1.5ms, servo at neutral(16875 + 375offset)  left 11250 right 22500

Uint16 battery_voltage_uint16 = 0;                 // battery voltage ADC result
float battery_voltage_f = 0;


int16 motor_position = 2680;
int16 motor_position_pre = 2680;
int16 motor_speed = 0;         // measured motor speed
int16 motor_speed_pre = 0;     // measured motor speed previous
Uint16 motor_pwm = 0;     // measured motor speed previous
Uint16 motor_pwm_pre = 0;     // measured motor speed previous
PID_Obj PID_Motor = {3, 0.25, 0, 750, -100, 0, 0, 750, 0, 0, 0, 0};
PID_Handle PID_Motor_Handle = &PID_Motor;

int16 gimbal_position = 0;
int16 gimbal_position_pre = 0;
int16 gimbal_speed = 0;
int16 gimbal_speed_pre = 0;
int16 gimbal_current = 0;
int16 gimbal_current_pre = 0;
int gimbal_current_phase = 0;

PID_Obj PID_Gimbal_Position = {6, 0.1, 60, 1000, -1000, 0, 0, 1500, -1500, 0, 0, 0};
PID_Handle PID_Gimbal_Position_Handle = &PID_Gimbal_Position;

PID_Obj PID_Gimbal_Current = {1, 0.002, 0, 1300, -1300, 0, 0, 1500, -1500, 0, 0, 0};
PID_Handle PID_Gimbal_Current_Handle = &PID_Gimbal_Current;

Uint16 ADC_Results[16];
int *current_pointer;

int count_init = 0;
int count_10khz = 0;
int count_1khz = 0;
Uint32 _tmp = 0;

Uint32 Message_RX_Count = 0;
Uint32 Message_TX_Count = 0;
Uint32 Message_RX_L = 0, Message_RX_H = 0;
Uint32 Message_TX_L = 0, Message_TX_H = 0;
Uint16 Message_RX_Index = 0;

int Message_TX_Flag = 0;

// system state machine
// 0: Initial
// 1: online
// 2: fault
int state_machine = 0; 
struct ECAN_REGS ECanaShadow;
struct ECAN_REGS ECanaShadow_post;

void main(void)
{
    // Initialize the DSP system
    Init_System();

    Init_Motor_Drvs();

    current_pointer = &gimbal_current_phase;

    // Forever loop
    for(;;){
        if(Message_TX_Flag){
            can_SendMailBox(0, 0x01, 0x02);
            Message_TX_Flag = 0;
            Message_TX_Count++;
        }
    }
}
int16 gimbal_phase_order = 0;
float gimbal_position_difference = 0;
int16 gimbal_direction = 0;
int16 gimbal_direction_pre = 0;

// ADC ISR 10 kHz. The main ISR for control loop
__interrupt void adc_isr(void)
{
    // GPIO 12 is set high
    GpioDataRegs.GPASET.bit.GPIO12 = 1;

    command_motor_speed = shadow_motor_speed;
    command_servo_position = shadow_servo_position;
    command_gimbal_position = shadow_gimbal_position;

    // 1 kHz control loop
    if(count_10khz == 10)
    {
        count_10khz = 0;

        // Servo position control
        EPwm2Regs.CMPA.half.CMPA = SERVO_HALF_PERIOD - command_servo_position;

        // Motor velocity control
        PID_Control(PID_Motor_Handle, command_motor_speed, motor_speed);
        motor_pwm_pre = motor_pwm;
        motor_pwm = PID_Motor.outputInt;
        motor_pwm = motor_pwm_pre*0.6 + 0.4*motor_pwm;
        EPwm1Regs.CMPA.half.CMPA = MOTOR_HALF_PERIOD - motor_pwm;

        // Gimbal position control
        PID_Control(PID_Gimbal_Position_Handle, command_gimbal_position, gimbal_position);

    }

    ADC_Get_Results(ADC_Results);
    gimbal_direction_pre = gimbal_direction;
    gimbal_current_pre = gimbal_current;
    gimbal_current = 2048 - ADC_Results[*current_pointer + 1];
    gimbal_current = (gimbal_direction_pre) == 1 ? gimbal_current : -gimbal_current;

    gimbal_current = gimbal_current_pre*0.1 + 0.9*gimbal_current;

    battery_voltage_uint16 = ADC_Results[4];
    battery_voltage_f = battery_voltage_f*0.99 + 0.01*battery_voltage_uint16*BATTERY_FGAIN;

    PID_Control(PID_Gimbal_Current_Handle, PID_Gimbal_Position.output, gimbal_current);

    gimbal_direction = PID_Gimbal_Current.output < 0 ? 0:1; // 0: right turn; 1: left turn;

    gimbal_position_difference = abs(gimbal_position - BLDC_AB_POS) * PHASES_PER_TICK;

    gimbal_phase_order = (int)gimbal_position_difference % 6;

    if (gimbal_position < BLDC_AB_POS)
    {
        gimbal_phase_order = 5 - gimbal_phase_order;
    }

    // BLDC commute
    BLDC_Commute(current_pointer, gimbal_phase_order, gimbal_direction, GIMBAL_HALF_PERIOD - PID_Gimbal_Current.outputInt);

    count_10khz++;

    // GPIO 12 is test point
    GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
    //
    // Clear ADCINT1 flag reinitialize for next SOC
    //
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
}

// cpu_timer0_isr, 1 kHz
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}

// Motor encoder ISR, update the motor position and velocity
__interrupt void ecap1_isr(void){

    Uint32 ecap1_t1;
    Uint32 ecap1_t2;
    float duty_count;

    ecap1_t1 = ECap1Regs.CAP1;
    ecap1_t2 = ECap1Regs.CAP2;

    duty_count = ((float)ecap1_t1 / (float)ecap1_t2)*4119;

    if(duty_count > 15 && duty_count < 4112){ // if this is a valid frame

        motor_position_pre = motor_position;

        motor_position = duty_count - 16;

        motor_speed_pre = motor_speed;

        if( abs(motor_position - motor_position_pre ) < 2000)
        {
            motor_speed = motor_position_pre - motor_position;
        }
        else
        {
            if(motor_position_pre < 2000)
            { // car is moving forward
                motor_speed = motor_position_pre + 4096 - motor_position;
            }
            else{
                motor_speed = motor_position_pre - 4096 - motor_position;
            }
        }
        motor_speed = motor_speed_pre*0.3 + 0.7*motor_speed;
    }
    else{
        motor_position_pre = motor_position;
        motor_position = motor_position + motor_speed;
    }

    if(count_init<4){
        count_init++;
        motor_speed = 0;
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

    duty_count = ((float)ecap3_t1 / (float)ecap3_t2)*4119;

    if(duty_count > 15 && duty_count < 4112){

        gimbal_position_pre = gimbal_position;

        gimbal_position = duty_count - 16;

        gimbal_position = gimbal_position_pre*0.8 + 0.2*gimbal_position;
    }

    ECap3Regs.ECCLR.bit.CEVT2 = 1;
    ECap3Regs.ECCLR.bit.INT = 1;

    //
    // Acknowledge this interrupt to receive more interrupts from group 4
    //
    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP4;

}

int _f = 0;

// CAN Bus interrupt
__interrupt void ecan0_isr(void)
{
    ECanaShadow = ECanaRegs;

    if(ECanaShadow.CANGIF0.bit.GMIF0 == 1){
        // mailbox interrupt
        if(ECanaShadow.CANGIF0.bit.MIV0 == 0){
            // if it is TX (mailbox 0) interrupt
            ECanaShadow.CANTA.all = 0;
            ECanaShadow.CANTA.bit.TA0 = 1;       // Clear TA0
            ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;

//            ECanaShadow.CANTRR.all = 0;
//            ECanaShadow.CANTRR.bit.TRR0 = 1;
//            ECanaRegs.CANTRR.all = ECanaShadow.CANTRR.all;

        }
        else if(ECanaShadow.CANGIF0.bit.MIV0 == 16){

            // if it is RX (mailbox 16) interrupt
            can_ReadMailBox(16, &Message_RX_L, &Message_RX_H);

            Message_RX_Index = Message_RX_L >> 16;
            shadow_motor_speed = Message_RX_L & 0x0000ffff;
            shadow_gimbal_position = Message_RX_H >> 16;
            shadow_servo_position = Message_RX_H & 0x0000ffff;

            Message_TX_L  = (Message_RX_L & 0xffff0000) | motor_speed;
            Message_TX_H  = ((Uint32) gimbal_position) << 16 | battery_voltage_uint16;

            Message_RX_Count++;

            // clear (RMP) Received-Message-Pending Register
            ECanaShadow.CANRMP.all = 0;
            ECanaShadow.CANRMP.bit.RMP16 = 1;
            ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
            Message_TX_Flag = 1;

            //can_SendMailBox(0, Message_TX_L, Message_TX_H); // send mailbox0
        }
    }
    else{
        // other interrupt
//        if(ECanaShadow.CANGIF0.bit.WLIF0 ){
//            ECanaShadow.CANGIF0.all = 0;
//            ECanaShadow.CANGIF0.bit.WLIF0 = 1;
//        }
//        else if(ECanaShadow.CANGIF0.bit.EPIF0){
//            ECanaShadow.CANGIF0.all = 0;
//            ECanaShadow.CANGIF0.bit.EPIF0 = 1;
//        }
//        else if(ECanaShadow.CANGIF0.bit.BOIF0){
//            ECanaShadow.CANGIF0.all = 0;
//            ECanaShadow.CANGIF0.bit.BOIF0 = 1;
//        }
//        else if(ECanaShadow.CANGIF0.bit.AAIF0){
//            ECanaShadow.CANGIF0.all = 0;
//            ECanaShadow.CANGIF0.bit.AAIF0 = 1;
//        }
//        else if(ECanaShadow.CANGIF0.bit.WDIF0){
//            ECanaShadow.CANGIF0.all = 0;
//            ECanaShadow.CANGIF0.bit.WDIF0 = 1;
//        }


    }
    ECanaRegs.CANGIF0.all = ECanaShadow.CANGIF0.all;
    _f = 1-_f;
    ECanaShadow_post = ECanaRegs;

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    return;
}

//
// End of File
//

