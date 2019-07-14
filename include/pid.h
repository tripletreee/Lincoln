#ifndef F28069_PID_H
#define F28069_PID_H

#include "F2806x_Device.h"     // F2806x Headerfile Include File


typedef struct _PID_Obj_
{
  float Kp;                //!< the proportional gain for the PID controller
  float Ki;                //!< the integral gain for the PID controller
  float Kd;                //!< the derivative gain for the PID controller

  float integralMax;
  float integralMin;

  float errorPrev;
  float errorIntegral;

  float outMax;            //!< the maximum output value allowed for the PID controller
  float outMin;            //!< the minimum output value allowed for the PID controller

  float output;
  float outputPre;
  Uint16 outputInt;

} PID_Obj;

//! \brief Defines the PID handle
//!
typedef struct _PID_Obj_ *PID_Handle;



void PID_Control(PID_Handle handle , float target, float feedback);


#endif

