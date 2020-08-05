# Lincoln Racing
This is a autonomous racing car project.  
The code is running on the TI's DSP TMS320F28069M. This package is developed based on TI's C2000 Ware. The development environment is based on TI's Code Composer Studio V8.2.  
The code is developed by Haosen W. and Xinkai Z. Project created at July 4, 2019.

# System Structure
The racing car has three motors:  
1. Brushless motor for power drive. 
2. Brushless servo motor for steering.
3. Brushless gimbal motor for camera panning.

The DSP controls the movement of these three motors. The power motor and gimbal motor are equiped with a 14 bit magnetic rotary position sensor, respectively. The DSP receives the command from a host computer, which is nVIDIA Jetson TX2 at this moment, through CAN bus.

# Control System Timing 
The DSP system clock frequency is 90 MHz.  
The DSP will recieve the command frame at a rate of 60/120/240 FPS, which depends on the host.  Therefore, the vehicle position control frequency is 60/120/240 Hz.
The inner loop control frequency is 1 kHz.  
The two magnetic encoder will update their postion info at 1.1 kHz.  

ADC sampling frequency 10 kHz. Position control loop is 1 kHz. Motor velocity control loop is 1 kHz, gimbal position control loop is 1 kHz, servo PWM is 300 Hz.
Every ten times of the main ISR, the encoder position will be updated.  
Based on the position control loop, the command from the host will be updated periodically through CAN bus.

# PWM Setting
PWM1 is for motor.
PWM2 is for servo.
PWM4,5,6 are for gimble.
PWMs are synced and PWM1 triggers ADC sampling at 10kHz.

# CAN Bus Protocol
CAN bus will give a 8-byte answer:
index | motor_speed | gimbal_position | battery_voltage

HOST should send a 8-byte command:
index | motor_speed | gimbal_position | servo_position