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

#pragma DATA_SECTION(ADC_Results,"Cla1DataRam1");
#pragma DATA_SECTION(Lincoln_Auto,"Cla1DataRam1");
#pragma DATA_SECTION(Lincoln_Auto_Handle,"Cla1DataRam1");

#pragma CODE_SECTION(adc_isr, "ramfuncs");
#pragma CODE_SECTION(ecan0_isr, "ramfuncs");
#pragma CODE_SECTION(ecap1_isr, "ramfuncs");
#pragma CODE_SECTION(ecap3_isr, "ramfuncs");

AUTO_Obj Lincoln_Auto;
AUTO_Handle Lincoln_Auto_Handle = &Lincoln_Auto;

Uint16 ADC_Results[16];
int count_init = 0;
struct ECAN_REGS ECanaShadow;
struct ECAN_REGS ECanaShadow_post;

Uint32 MessageTX2_RX_Count = 0;
Uint32 MessageTX2_TX_Count = 0;
Uint32 MessageTX2_RX_L = 0, MessageTX2_RX_H = 0;
Uint32 MessageTX2_TX_L = 0, MessageTX2_TX_H = 0;
Uint16 MessageTX2_RX_Index = 0;

Uint32 MessageRC_RX_Count = 0;
Uint32 MessageRC_TX_Count = 0;
Uint32 MessageRC_RX_L = 0, MessageRC_RX_H = 0;
Uint32 MessageRC_TX_L = 0, MessageRC_TX_H = 0;
Uint16 MessageRC_RX_Index = 0;

int LED_Motor_Counter = 0;
int LED_CANBus_TX2_Counter = 0;
int LED_CANBus_RC_Counter = 0;
int LED_Brake_Counter = 0;

float tmp_motor_speed = 0.0f;
float tmp_servo_pos = 0.0f;
float tmp_gimbal_pos = 0.0f;
Uint16 tmp_auto_mode = 0;

int32 command_motor_speed = 0;
int32 command_motor_speed_pre = 0;

// system state machine
// 0: Initial
// 1: online
// 2: fault
int state_machine = 0; 


void main(void)
{
    // Initialize the DSP system
    Init_System();

    Init_Motor_Drvs();

    Init_Auto(Lincoln_Auto_Handle);

    // Forever loop
    for(;;){

        // Read DRV8305 fault pin
        int _fault = GpioDataRegs.GPADAT.bit.GPIO28;

        if(_fault==0){

            // Disable the DRV8305 for gimbal
            GpioDataRegs.GPBCLEAR.bit.GPIO50 = 1;

            // Delay 10ms for sensor update
            DELAY_US(10000);

            // Enable the DRV8305 for gimbal
            GpioDataRegs.GPBSET.bit.GPIO50 = 1;

            // Delay 10ms for sensor update
            DELAY_US(10000);
        }
    }
}


// ADC ISR 10 kHz. The main ISR for control loop
__interrupt void adc_isr(void)
{
    //Cla1ForceTask1(); // Use the CLA to do the control

    //
    // Clear ADCINT1 flag reinitialize for next SOC
    //

    LED_Motor_Counter++;
    LED_Brake_Counter++;

    if(LED_Motor_Counter>2500){
        GpioDataRegs.GPBTOGGLE.bit.GPIO58 = 1;
        LED_Motor_Counter = 0;
    }

    if(LED_Brake_Counter>1000){

        command_motor_speed = Lincoln_Auto.command_motor_speed;

        MessageRC_TX_L = 0x00000000;

        // If the car is braking, stopping, or going backward
        // Turn on the brake light
        if(command_motor_speed < command_motor_speed_pre || command_motor_speed<=0){
            MessageRC_TX_H = 0x00000001;
        }
        else{
            MessageRC_TX_H = 0x00000000;
        }
        can_SendMailBox(1, MessageRC_TX_L, MessageRC_TX_H); // send through mailbox1 to CANID 0x04
        command_motor_speed_pre = command_motor_speed;
        MessageRC_TX_Count++;
        LED_Brake_Counter = 0;
    }

    if(Lincoln_Auto.command_motor_speed > 0){
        GpioDataRegs.GPACLEAR.bit.GPIO21 = 1;
    }
    else{
        GpioDataRegs.GPASET.bit.GPIO21 = 1;
    }

    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
}


// CAN Bus interrupt, suppose to be 100 Hz
__interrupt void ecan0_isr(void)
{
    ECanaShadow = ECanaRegs;

    if(ECanaShadow.CANGIF0.bit.GMIF0 == 1){
//        ECanaShadow.CANTA.all = 0;
//        ECanaShadow.CANTA.bit.TA0 = 1;       // Clear TA0
//        ECanaShadow.CANTA.bit.TA1 = 1;       // Clear TA0
//        ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;


        // mailbox interrupt
        if(ECanaShadow.CANGIF0.bit.MIV0 == 0){
            // if it is TX (mailbox 0) interrupt
            ECanaShadow.CANTA.all = 0;
            ECanaShadow.CANTA.bit.TA0 = 1;       // Clear TA0
            ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;

        }
        else if(ECanaShadow.CANGIF0.bit.MIV0 == 1){
            // if it is TX (mailbox 1) interrupt
            ECanaShadow.CANTA.all = 0;
            ECanaShadow.CANTA.bit.TA1 = 1;       // Clear TA1
            ECanaRegs.CANTA.all = ECanaShadow.CANTA.all;

        }
        else if(ECanaShadow.CANGIF0.bit.MIV0 == 16){

            // if it is RX (mailbox 16) interrupt
            // mailbox 16 is mainly used for receiving TX2 commands
            can_ReadMailBox(16, &MessageTX2_RX_L, &MessageTX2_RX_H);

            MessageTX2_RX_Index = MessageTX2_RX_L >> 16;

            if(Lincoln_Auto.auto_mode == 1){
                // if the vehicle is in autonomous mode

                tmp_motor_speed = MessageTX2_RX_L & 0x0000ffff;

                if(tmp_motor_speed < MOTOR_SPEED_MIN){
                    Lincoln_Auto.shadow_motor_speed = MOTOR_SPEED_MIN;
                }
                else if(tmp_motor_speed > MOTOR_SPEED_MAX){
                    Lincoln_Auto.shadow_motor_speed = MOTOR_SPEED_MAX;
                }
                else{
                    Lincoln_Auto.shadow_motor_speed = tmp_motor_speed;
                }

                tmp_servo_pos = MessageTX2_RX_H & 0x0000ffff;

                if(tmp_servo_pos < SERVO_POS_MIN){
                    Lincoln_Auto.shadow_servo_position = SERVO_POS_MIN;
                }
                else if(tmp_servo_pos > SERVO_POS_MAX){
                    Lincoln_Auto.shadow_servo_position = SERVO_POS_MAX;
                }
                else{
                    Lincoln_Auto.shadow_servo_position = tmp_servo_pos;
                }

                tmp_gimbal_pos = MessageTX2_RX_H >> 16;

                if(tmp_gimbal_pos < GIMBAL_POS_MIN){
                    Lincoln_Auto.shadow_gimbal_position = GIMBAL_POS_MIN;
                }
                else if(tmp_gimbal_pos > GIMBAL_POS_MAX){
                    Lincoln_Auto.shadow_gimbal_position = GIMBAL_POS_MAX;
                }
                else{
                    Lincoln_Auto.shadow_gimbal_position = tmp_gimbal_pos;
                }
            }

            MessageTX2_TX_L  = (MessageTX2_RX_L & 0xffff0000) | (((int16) Lincoln_Auto.motor_speed_for_Jetson) & 0x0000ffff);

            MessageTX2_TX_H  = ((Uint32) Lincoln_Auto.gimbal_position) << 16 | Lincoln_Auto.command_servo_position;

            /*
            if(Lincoln_Auto.auto_mode == 1){    // If it is in autonomous mode, return gimbal position
                MessageTX2_TX_H  = ((Uint32) Lincoln_Auto.gimbal_position) << 16 | Lincoln_Auto.battery_voltage_uint;
            }
            else{   // If it is in manual RC mode, return servo position
                MessageTX2_TX_H  = ((Uint32) Lincoln_Auto.command_servo_position) << 16 | Lincoln_Auto.battery_voltage_uint;
            }
            */

            MessageTX2_RX_Count++;

            // Flash the LED3 for CAN Bus TX2
            if(LED_CANBus_TX2_Counter > 5){
                LED_CANBus_TX2_Counter = 0;
                GpioDataRegs.GPBTOGGLE.bit.GPIO57 = 1;
            }
            else{
                LED_CANBus_TX2_Counter++;
            }

            Lincoln_Auto.motor_speed_for_Jetson = 0;

            // clear (RMP) Received-Message-Pending Register
            ECanaShadow.CANRMP.all = 0;
            ECanaShadow.CANRMP.bit.RMP16 = 1;
            ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;

            can_SendMailBox(0, MessageTX2_TX_L, MessageTX2_TX_H); // send through mailbox0 to CANID 0x01
        }

        else if(ECanaShadow.CANGIF0.bit.MIV0 == 17){

            // if it is RX (mailbox 17) interrupt from RC receiver
            can_ReadMailBox(17, &MessageRC_RX_L, &MessageRC_RX_H);

            tmp_auto_mode = MessageRC_RX_L >> 16;

            if(tmp_auto_mode > 2500){
                Lincoln_Auto.auto_mode = 0;
            }
            else{
                Lincoln_Auto.auto_mode = 1;
            }

            if(Lincoln_Auto.auto_mode == 0){
                tmp_motor_speed = MessageRC_RX_L & 0x0000ffff;
                tmp_motor_speed = (2950 - tmp_motor_speed)*0.5;
                if(abs(tmp_motor_speed)<20){
                    tmp_motor_speed = 0;
                }

                if(tmp_motor_speed > MOTOR_SPEED_MIN && tmp_motor_speed < MOTOR_SPEED_MAX){
                    Lincoln_Auto.shadow_motor_speed = (int32) tmp_motor_speed;
                }

                tmp_servo_pos = MessageRC_RX_H & 0x0000ffff;
                tmp_servo_pos = SERVO_POS_DEF + (2926 - (float)tmp_servo_pos)*11.957; //   *5500/460;

                if(tmp_servo_pos < SERVO_POS_MIN){
                    Lincoln_Auto.shadow_servo_position = SERVO_POS_MIN;
                }
                else if(tmp_servo_pos > SERVO_POS_MAX){
                    Lincoln_Auto.shadow_servo_position = SERVO_POS_MAX;
                }
                else{
                    Lincoln_Auto.shadow_servo_position = (int32) tmp_servo_pos;
                }

                Lincoln_Auto.shadow_gimbal_position = GIMBAL_POS_DEF;
            }

            MessageRC_RX_Count++;

            // Flash the LED2 for CAN Bus RC module
            if(LED_CANBus_RC_Counter > 5){
                LED_CANBus_RC_Counter = 0;
                GpioDataRegs.GPBTOGGLE.bit.GPIO56 = 1;
            }
            else{
                LED_CANBus_RC_Counter++;
            }

            // clear (RMP) Received-Message-Pending Register
            ECanaShadow.CANRMP.all = 0;
            ECanaShadow.CANRMP.bit.RMP17 = 1;
            ECanaRegs.CANRMP.all = ECanaShadow.CANRMP.all;
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
    ECanaShadow_post = ECanaRegs;

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP9;
    return;
}

//
// End of File
//

