#ifndef __STATE_H
#define __STATE_H 

#include "defs.h";

void advanceState();
void updateState_A();
void updateState_E();
void startAction();
void dischargeAction();
void prechargeAction();
void tsActivateAction();
void rtdAction();

#endif