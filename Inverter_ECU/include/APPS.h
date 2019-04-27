#ifndef INCLUDE_APPS_H_
#define INCLUDE_APPS_H_
#include"adc.h"
#include "sys_common.h"

extern uint16 Request1;
extern uint16 Request2;

void GetTorueRequest();
void GetADC();
uint16 Calculate_Torque_request(uint16 ADCvalue);
uint8 Check_Torque_Plausibility(void);
#endif
