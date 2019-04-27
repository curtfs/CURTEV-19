/*
 * BPPS.c
 *
 *  Created on: Apr 25, 2019
 *      Author: Xyntechs
 */
#include"BPPS.h"
adcData_t adc_data[4];
void GetADC1()
{
    adcStartConversion(adcREG1,adcGROUP1);      //Recieving from ADC group1
    // Infinite Loop for contineous operation
    while((adcIsConversionComplete(adcREG1,adcGROUP1))==0);
    adcGetData(adcREG1, adcGROUP1,&adc_data[0]);
}

void GetBrakeRequest()
{
    GetADC1();
    /* conversion results :                                       */
    Request3= Calculate_Brake_request(adc_data[2].value); //Convert ADC value to torque
    Request4= Calculate_Brake_request(adc_data[3].value); //Convert ADC value to torque
}

uint16 Calculate_Brake_request(uint16 ADCvalue)
{
    float Brake_Command;
    Brake_Command=ADCvalue;
    Brake_Command/=4095.0;
    Brake_Command*=212;
    Brake_Command=Brake_Command*32767/(0.75*200*1.5);
    ADCvalue=Brake_Command;

    return ADCvalue;
}






