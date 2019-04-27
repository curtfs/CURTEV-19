/*
 * BBPS.h
 *
 *  Created on: Apr 25, 2019
 *      Author: Xyntechs
 */

#ifndef INCLUDE_BPPS_H_
#define INCLUDE_BPPS_H_

#include"adc.h"
#include "sys_common.h"

extern uint16 Request3;
extern uint16 Request4;

void GetBrakeRequest();
void GetADC1();
uint16 Calculate_Brake_request(uint16 ADCvalue);


#endif /* INCLUDE_BPPS_H_ */
