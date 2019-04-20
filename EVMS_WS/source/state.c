#include "state.h"
static State_A currState_A = START;
static State_E currState_E = NO_ERROR;
static State_A nextState_A;
static State_E nextState_E;
//static boolean startBtn_Press;
//static time startBtn_last; 
//static boolean rtdBtn_Press;
//static time rtdBtn_last;

void advanceState()
{
    updateState_E();
    updateState_A();
}

void updateState_A()
{
    switch(currState_A)
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
    switch(currState_E)
    {
        case NO_ERROR:
        break;
        case WARNING:
        break;
        case FATAL_ERROR:
        break;
    }
    currState_E = nextState_E;
}


void startAction()
{
    if(1/*Start BTN pressed*/)
    {
        if(1/*startBtn_Press*/)
        {
        if(1/*currTime - startBtn_last > 3 seconds*/)
        {
            nextState_A = PRECHARGE;
            //startBtn_Press=0;
            //update startBtn_last
        }
        }
        else
        {
            //update startBtn_last
            //startBtn_Press=1;
        }   
    }
    else
    {
        //startBtn_Press=0;
        nextState_A = START;
    }
}
void dischargeAction()
{
    if(1/*DC_BusVoltage < 60*/)
    {
        nextState_A= START;
    }
}
void prechargeAction()
{
    if(0/*Start BTN pressed*/)
    {
        if(1/*startBtn_Press*/)
        {
        if(1/*currTime - startBtn_last > 3 seconds*/)
        {
            nextState_A = DISCHARGE;
            //startBtn_Press=0;
            //update startBtn_last
        }
        }
        else
        {
            //update startBtn_last
            //startBtn_Press=1;
        }   
    }
    else if(1/*currDCBusVoltage > 90%*/)
    {
        nextState_A = TS_ACTIVATED;
    }
    else{
        nextState_A = PRECHARGE;
    }
}
void tsActivateAction()
{
      if(0/*Start BTN pressed*/)
    {
        if(1/*startBtn_Press*/)
        {
        if(1/*currTime - startBtn_last > 3 seconds*/)
        {
            nextState_A = DISCHARGE;
            //startBtn_Press=0;
            //update startBtn_last
        }
        }
        else
        {
            //update startBtn_last
            //startBtn_Press=1;
        }   
    }
    else if(1/*RTD pressed*/)
    {
        if(1/*rtdBtn_Press*/)
        {
            if(1/*currTime - rtdBtn_last > 3 seconds*/)
            {
                if(1/*brkPressed*/)
                {
                    nextState_A=RTD;
                }
            }
        }
        else
        {
            //update rtdBtn_last
            //rtdBtn_Press=1;
        }

    }
    else
    {
        nextState_A = TS_ACTIVATED;
    }
}
void rtdAction()
{
    //Send Torque request from Inverter_ECU
    if(1/*RTD pressed*/)
    {
        if(1/*rtdBtn_Press*/)
        {
            if(1/*currTime - rtdBtn_last > 3 seconds*/)
            {
                    nextState_A=TS_ACTIVATED;
            }
        }
        else
        {
            //update rtdBtn_last
            //rtdBtn_Press=1;
        }

    }
    else
    {
        nextState_A = RTD;
    }
}
