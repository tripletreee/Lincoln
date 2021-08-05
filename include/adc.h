#ifndef F28069_ADC_H
#define F28069_ADC_H

#pragma CODE_SECTION(ADC_Get_Results, "Cla1Prog");

#include "F2806x_Device.h"     // F2806x Headerfile Include File
#include "F2806x_Examples.h"

#define ADC_OFFSET_VA 0
#define ADC_OFFSET_VB 0
#define ADC_OFFSET_VC 0
#define ADC_OFFSET_IA 2012
#define ADC_OFFSET_IB 2012
#define ADC_OFFSET_IC 2012
#define BATTERY_FGAIN 4.834e-3f

void Init_ADC(void);

inline void ADC_Get_Results(Uint16 *ADC_Results){

    ADC_Results[0] = AdcResult.ADCRESULT0;
    ADC_Results[1] = AdcResult.ADCRESULT1;
    ADC_Results[2] = AdcResult.ADCRESULT2;
    ADC_Results[3] = AdcResult.ADCRESULT3;
    ADC_Results[4] = AdcResult.ADCRESULT4;
//    ADC_Results[5] = AdcResult.ADCRESULT5;
//    ADC_Results[6] = AdcResult.ADCRESULT6;
//    ADC_Results[7] = AdcResult.ADCRESULT7;
//    ADC_Results[8] = AdcResult.ADCRESULT8;
//    ADC_Results[9] = AdcResult.ADCRESULT9;
//    ADC_Results[10] = AdcResult.ADCRESULT10;
//    ADC_Results[11] = AdcResult.ADCRESULT11;
//    ADC_Results[12] = AdcResult.ADCRESULT12;
//    ADC_Results[13] = AdcResult.ADCRESULT13;
//    ADC_Results[14] = AdcResult.ADCRESULT14;
//    ADC_Results[15] = AdcResult.ADCRESULT15;

}

#endif // end of F28069_ADC_H definition


