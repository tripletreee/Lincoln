#include "pid.h"


void PID_Control(PID_Handle handle , float target, float feedback)
{

    PID_Obj *PID_obj = (PID_Obj *)handle;

    float error;
    float p_term;
    float i_term;
    float d_term;
    float output;

    error = target - feedback;

    p_term = PID_obj->Kp * error;                          // Proportional term
    i_term = PID_obj->Ki * error + PID_obj->errorIntegral; // Integral term
    d_term = PID_obj->Kd * (error - PID_obj->errorPrev);   // Derivative term

    if (i_term >= PID_obj->integralMax) { i_term = PID_obj->integralMax;} // Integral Max
    else if (i_term <= PID_obj->integralMin) { i_term = PID_obj->integralMin;} // Integral Min

    PID_obj->errorIntegral = i_term;   // update the integral value
    PID_obj->errorPrev = error;         // update the previous error

    output = p_term + i_term + d_term;

    if(output >= PID_obj->outMax) { output = PID_obj->outMax;}      // Limit out max
    else if(output <= PID_obj->outMin) { output = PID_obj->outMin;} // Limit out min

    PID_obj->outputPre = PID_obj->output;
    PID_obj->output = output*0.5 + PID_obj->outputPre*0.5;
    PID_obj->outputInt = (Uint16) abs(PID_obj->output);
}


