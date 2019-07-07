#include "pid.h"



// Gimbal PID controller
float control_PID(float target, float current){
    const double Kp = 1.0;
    const double Ki = 0.8;
    const double Kd = 0.0;
    const double limit_p = 0.5; // Integral anti windup (upper bound)
    const double limit_n = -0.5; // Integral anti windup (lower bound)
    double error = 0.0; // velocity error
    double p_value = 0.0; // Proportional term output
    double i_value = 0.0; // Integral term output
    double d_value = 0.0; // Derivative term output
    double PID_out; // Output value of the controller
    double gimbal_error_prev = 0;     // record error from the previous control cycle for the Kd term
    double gimbal_error_integral = 0; // Accumulate the error for the Ki term
    int CONTROL_Ts=0;

    error = target - current;
    p_value = Kp * error; // Proportional term
    gimbal_error_integral = gimbal_error_integral + Ki * error * CONTROL_Ts; // Integration in discrete-time
    i_value = gimbal_error_integral; // Integral term
    if (i_value >= limit_p) { i_value = limit_p;} // Integral anti windup
    else if (i_value <= limit_n) { i_value = limit_n;}
    gimbal_error_integral = i_value;
    d_value = Kd * (error - gimbal_error_prev)/CONTROL_Ts; // Derivative term
    gimbal_error_prev = error; // record the error value for next cycle
    PID_out = p_value + i_value + d_value;

    return PID_out;
}
