#include "gio.h"
#include "sci.h"
#include "can.h"
#include "state.h"
#include <time.h>
uint32 i;
extern uint8 tx_data_start[6];
extern uint8 tx_data_error[6];
extern uint8 tx_data_stop[5];
extern uint8 tx_inverter_start[9];
static State_A currState_A;
static State_E currState_E;
static State_A nextState_A;
static State_E nextState_E;
volatile Buttons_S Buttons;
volatile Data_Inverter Data_EVMS;
volatile uint32 start_btn_pressed = 0;
volatile uint32 RTD_btn_pressed = 0;
volatile uint32 BamocarError = 0;
volatile uint32 Discharge_time_passed = 0;
volatile uint32 Precharge_time_passed = 0;
volatile uint32 timer_inverter_ecu_start = 0;
volatile uint32 CAN_DISCONNECTED;
int Entered;
float Curr;
float Old;
time_t start_t, end_t;
double diff_t;
time_t curr_time;
clock_t start, end;
double diff;
unsigned char buffer[64];
int timer_btn_start = 0;
int timer_btn_RTD = 0;
int timer_btn_RTD_Disable = 0;
int DC_60 = 0;
int DC_90 = 0;

void Initialize_States(void)
{
    currState_A = START;
    currState_E = NO_ERROR;
}

void advanceState()
{
    updateState_E();
    if (currState_E == NO_ERROR)
    {
//        if (Data_EVMS.SDC_Feedback == 0)
//        {
//           currState_A = START;
//
//        }
        updateState_A();

    }
}

void updateState_A()
{
    switch (currState_A)
    {
    case START:
        startAction();
        break;
    case DISCHARGE:
        dischargeAction();
        break;
    case PRECHARGE:
        prechargeAction();
        break;
    case TS_ACTIVATED:
        tsActivateAction();
        break;
    case RTD:
        rtdAction();
        break;
    }
    currState_A = nextState_A;
}

void updateState_E()
{
    switch (currState_E)
    {
    case NO_ERROR:
        noerrorAction();
        break;
    case FATAL_ERROR:
        fatalerrorAction();
        break;
    }
    currState_E = nextState_E;
}

void startAction()
{
    if (Buttons.Start)
    {
        timer_btn_start++;
        if (timer_btn_start > 200000)
        {
            timer_btn_start = 0;
            nextState_A = PRECHARGE;
            Buttons.Start = 0;
            sciSend(scilinREG, 22, (unsigned char *) "Next_State: Pre_Charge");
            sciSend(scilinREG, 2, (unsigned char *) "\r\n");
            for (i = 0; i < 10000000; i++)
                ;
        }
    }
    else
    {
        timer_btn_start = 0;
        nextState_A = START;
    }

}
void dischargeAction()
{
    gioSetBit(gioPORTA, 3, 1); //disable precharge relay
    gioSetBit(gioPORTA, 0, 1); //close SDC
    DC_60++;
    if (DC_60 > 500000) //if(Data_EVMS.DC_Bus_Voltage_60) //if(Data_EVMS.DC_Bus_Voltage < 60)
    {
        nextState_A = START;
        DC_60 = 0;
    }
    else
        nextState_A = DISCHARGE;
}
void prechargeAction()
{
    gioSetBit(gioPORTA, 3, 0); //enable precharge relay
    DC_90++;
    if (Buttons.Start)
    {
        timer_btn_start++;
        DC_90 = 0;
        if (timer_btn_start > 200000)
        {
            timer_btn_start = 0;
            nextState_A = DISCHARGE;
            sciSend(scilinREG, 22, (unsigned char *) "Next_State: Dis_Charge");
            sciSend(scilinREG, 2, (unsigned char *) "\r\n");
            for (i = 0; i < 1000000; i++)
                ;
        }
    }
    else if (DC_90 > 1000000 && Data_EVMS.DC_Bus_Voltage) //else if(Data_EVMS.DC_Bus_Voltage)
    {
        gioSetBit(gioPORTA, 0, 0); //enable SDC
        for (i = 0; i < 25000000; i++);
        gioSetBit(gioPORTA, 3, 1); //disable precharge relay
        timer_btn_start = 0;
        DC_90 = 0;
        nextState_A = TS_ACTIVATED;
        sciSend(scilinREG, 24, (unsigned char *) "Next_State: TS_Activated");
        sciSend(scilinREG, 2, (unsigned char *) "\r\n");
    }
    else
    {
        timer_btn_start = 0;
        nextState_A = PRECHARGE;
    }
}

void tsActivateAction()
{
    if (Buttons.Start)
    {
        if (Buttons.Start)
        {
            timer_btn_start++;
            if (timer_btn_start > 200000)
            {
                timer_btn_start = 0;
                nextState_A = DISCHARGE;
                sciSend(scilinREG, 22,
                        (unsigned char *) "Next_State: Dis_Charge");
                sciSend(scilinREG, 2, (unsigned char *) "\r\n");
                for (i = 0; i < 1000000; i++);
            }
        }
    }
    else if (Buttons.RTD)
    {
        timer_btn_RTD++;
        if (timer_btn_RTD > 200000 && Data_EVMS.BRAKES_PRESSED)
        {

            timer_btn_RTD = 0;
            nextState_A = RTD;
            canTransmit(canREG1, canMESSAGE_BOX3, tx_data_start); //Send torque requests to motor.
            gioSetBit(gioPORTA, 1, 0); //RTD sound
            for (i = 0; i < 15000000; i++);
            gioSetBit(gioPORTA, 1, 1); //RTD sound
            sciSend(scilinREG, 15, (unsigned char *) "Next_State: RTD");
            sciSend(scilinREG, 2, (unsigned char *) "\r\n");
            for (i = 0; i < 1000000; i++)
                ;

        }
    }
    else
    {
        nextState_A = TS_ACTIVATED;
    }
}

void rtdAction()
{

    for (i = 0; i < 1000; i++) ;
    if (Buttons.RTD)
    {
        timer_btn_RTD_Disable++;
        if (timer_btn_RTD_Disable > 20000)
        {
            timer_btn_RTD_Disable = 0;
            nextState_A = TS_ACTIVATED;
            canTransmit(canREG1, canMESSAGE_BOX6, tx_data_stop); //Disable torque commands
            sciSend(scilinREG, 24,
                    (unsigned char *) "Next_State: TS_Activated");
            sciSend(scilinREG, 2, (unsigned char *) "\r\n");
            for (i = 0; i < 1000000; i++)
                ;
        }

    }
    else
    {

        timer_btn_RTD_Disable = 0;
        nextState_A = RTD;
    }
}

void fatalerrorAction()
{
    //TODO check with ground
    gioSetBit(gioPORTA, 0, 1);
    //TODO SPI to display
}

void noerrorAction()
{
    if (CAN_DISCONNECTED == 1)
    {
        nextState_E = FATAL_ERROR;
    }
    else if (timer_inverter_ecu_start > 150)
    {
        nextState_E = FATAL_ERROR;
    }
    else if (!Data_EVMS.INVERTER_ERROR_WARNING)
    {
        nextState_E = FATAL_ERROR;
    }

    else if (!Data_EVMS.IMD_Feedback)
    {
        nextState_E = FATAL_ERROR;
    }
    else if (!Data_EVMS.BSPD_Feedback)
    {
        nextState_E = FATAL_ERROR;
    }


}

