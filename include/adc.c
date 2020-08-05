#include "adc.h"

void Init_ADC(void){

    InitAdc();          // Initialize the ADC for F2806x

    AdcOffsetSelfCal(); // ADC offset self calibrate for F2806x

    //
    // Configure ADC
    //
    EALLOW;

    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;    // ADCINT1 trips after AdcResults latch
    AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = 1;  // Enable non-overlap mode

    AdcRegs.INTSEL1N2.bit.INT1E     = 1;    // Enabled ADCINT1
    AdcRegs.INTSEL1N2.bit.INT1CONT  = 0;    // Disable ADCINT1 Continuous mode
    AdcRegs.INTSEL1N2.bit.INT1SEL   = 1;    // setup EOC1 to trigger ADCINT1 to fire

    //
    // setup SOCs and their configurations
    //
    
    AdcRegs.ADCSOC0CTL.bit.CHSEL 	= 9;    // SOC0, channel ADCINB1
    AdcRegs.ADCSOC0CTL.bit.TRIGSEL 	= 5;    // SOC0, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC0CTL.bit.ACQPS 	= 6;    // SOC0, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC1CTL.bit.CHSEL 	= 9;    // SOC1, channel ADCINB1
    AdcRegs.ADCSOC1CTL.bit.TRIGSEL 	= 5;    // SOC1, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC1CTL.bit.ACQPS 	= 6;    // SOC1, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
    
    AdcRegs.ADCSOC2CTL.bit.CHSEL 	= 10;   // SOC2, channel ADCINB2
    AdcRegs.ADCSOC2CTL.bit.TRIGSEL 	= 5;    // SOC2, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC2CTL.bit.ACQPS 	= 6;    // SOC2, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC3CTL.bit.CHSEL 	= 11;   // SOC3, channel ADCINB3
    AdcRegs.ADCSOC3CTL.bit.TRIGSEL 	= 5;    // SOC3, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC3CTL.bit.ACQPS 	= 6;    // SOC3, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC4CTL.bit.CHSEL 	= 12;   // SOC4, channel ADCINB4
    AdcRegs.ADCSOC4CTL.bit.TRIGSEL 	= 5;    // SOC4, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC4CTL.bit.ACQPS 	= 6;    // SOC4, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC5CTL.bit.CHSEL 	= 13;   // SOC5, channel ADCINB5
    AdcRegs.ADCSOC5CTL.bit.TRIGSEL 	= 5;    // SOC5, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC5CTL.bit.ACQPS 	= 6;    // SOC5, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC6CTL.bit.CHSEL 	= 6;    // SOC6, channel ADCINA6
    AdcRegs.ADCSOC6CTL.bit.TRIGSEL 	= 5;    // SOC6, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC6CTL.bit.ACQPS 	= 6;    // SOC6, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC7CTL.bit.CHSEL 	= 7;    // SOC7, channel ADCINA7
    AdcRegs.ADCSOC7CTL.bit.TRIGSEL 	= 5;    // SOC7, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC7CTL.bit.ACQPS 	= 6;    // SOC7, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC8CTL.bit.CHSEL 	= 8;    // SOC8, channel ADCINB0
    AdcRegs.ADCSOC8CTL.bit.TRIGSEL 	= 5;    // SOC8, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC8CTL.bit.ACQPS 	= 6;    // SOC8, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC9CTL.bit.CHSEL 	= 9;    // SOC9, channel ADCINB1
    AdcRegs.ADCSOC9CTL.bit.TRIGSEL 	= 5;    // SOC9, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC9CTL.bit.ACQPS 	= 6;    // SOC9, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
    
    AdcRegs.ADCSOC10CTL.bit.CHSEL 	= 10;   // SOC10, channel ADCINB2
    AdcRegs.ADCSOC10CTL.bit.TRIGSEL = 5;    // SOC10, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC10CTL.bit.ACQPS 	= 6;    // SOC10, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC11CTL.bit.CHSEL 	= 11;   // SOC11, channel ADCINB3
    AdcRegs.ADCSOC11CTL.bit.TRIGSEL = 5;    // SOC11, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC11CTL.bit.ACQPS 	= 6;    // SOC11, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC12CTL.bit.CHSEL 	= 12;   // SOC12, channel ADCINB4
    AdcRegs.ADCSOC12CTL.bit.TRIGSEL = 5;    // SOC12, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC12CTL.bit.ACQPS 	= 6;    // SOC12, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC13CTL.bit.CHSEL 	= 13;   // SOC13, channel ADCINB5
    AdcRegs.ADCSOC13CTL.bit.TRIGSEL = 5;    // SOC13, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC13CTL.bit.ACQPS 	= 6;    // SOC13, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC14CTL.bit.CHSEL 	= 13;   // SOC14, channel ADCINB5
    AdcRegs.ADCSOC14CTL.bit.TRIGSEL = 5;    // SOC14, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC14CTL.bit.ACQPS 	= 6;    // SOC14, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)

    AdcRegs.ADCSOC15CTL.bit.CHSEL 	= 13;   // SOC15, channel ADCINB5
    AdcRegs.ADCSOC15CTL.bit.TRIGSEL = 5;    // SOC15, trigger source 5 (ePWM1)
    AdcRegs.ADCSOC15CTL.bit.ACQPS 	= 6;    // SOC15, S/H Window to 7 ADC Clock Cycles, (6 ACQPS plus 1)
    
    EDIS;
}

