#ifndef __DEFS_H
#define __DEFS_H

typedef struct Btns
{
    int RTD;
    int Start;
} Buttons_S;

typedef struct Data_EV
{
    uint32 DC_Bus_Voltage; //Current DC voltage, read from Inverter through Inverter ECU
    uint32 DC_Bus_Voltage_60;
    uint32 SDC_Feedback;           // Shutdown circuit feedback
    uint32 IMD_Feedback;            // IMD
    uint32 BSPD_Feedback;            //BSPD
    uint32 INVERTER_ERROR_WARNING;     // errors and warnings from register 0x8F
    uint32 BRAKES_PRESSED;

} Data_Inverter;

typedef enum
{
    START, DISCHARGE, PRECHARGE, TS_ACTIVATED, RTD
} State_A;
typedef enum
{
    NO_ERROR, WARNING, FATAL_ERROR
} State_E;

#endif
