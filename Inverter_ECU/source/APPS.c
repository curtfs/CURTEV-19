#include"APPS.h"
extern adcData_t adc_data[4];
void GetADC()
{
    adcStartConversion(adcREG1,adcGROUP1);      //Recieving from ADC group1

    // Infinite Loop for contineous operation
    while((adcIsConversionComplete(adcREG1,adcGROUP1))==0);
    adcGetData(adcREG1, adcGROUP1,&adc_data[0]);
}

void GetTorueRequest()
{
    GetADC();
    /* conversion results :                                       */
    Request1= Calculate_Torque_request(adc_data[0].value); //Convert ADC value to torque
    Request2= Calculate_Torque_request(adc_data[1].value); //Convert ADC value to torque
}

uint16 Calculate_Torque_request(uint16 ADCvalue)
{
    float Torque_Command;
    Torque_Command=ADCvalue;
    Torque_Command/=4095.0;
    Torque_Command*=212;
    Torque_Command=Torque_Command*32767/(0.75*200*1.5);
    ADCvalue=Torque_Command;

    return ADCvalue;
}

uint8 Check_Torque_Plausibility(void)
{
    uint16 Deviation = abs(Request1 - Request2);
    uint16 Max_Deviation = 3088; // 10% * 30877 maximum torque request.
    if(Deviation < Max_Deviation)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}




