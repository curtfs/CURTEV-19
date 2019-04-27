/* USER CODE BEGIN (0) */

#include "can.h"
#include "sci.h"
#include "APPS.h"
#include "stdlib.h"
#include "BPPS.h"
#include "rti.h"
#include "BUFFERDATA.h"
/* USER CODE END */

/* Include Files */

#include "sys_common.h"

/* USER CODE BEGIN (1) */
uint8 tx_data[2];
unsigned char TorqueShow[8];
uint16 Request1;
uint16 Request2;
uint16 Request3;
uint16 Request4;
uint8 errflag;
uint8 startflag;
uint16 temp;
uint16 currentState;
uint16 nextState;
uint8 implausability;
DATA_BUFFER INVERTER_DATA_BUFFER;
//uint8 INVERTER_DATA[28];
uint8 INVERTER_DATA_1[8];
uint8 INVERTER_DATA_2[8];
uint8 INVERTER_DATA_3[8];
uint8 read1[2];
uint8 read2[2];
uint8 read3[2];
uint8 read4[2];
unsigned char stateshow[2];  // for debug
uint8 i;
uint16 NumberofstateChars; // for debug
uint16 Idle = 0;
uint16 TorqueSR =  1;
uint16 NO_TORQUE  = 2;
uint8 z=0;
//#define SendErr 3
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */
uint32_t checkpackets(uint8_t *src_packet,uint8_t *dst_packet,uint32_t psize);
void Show_torque(void);
void Show_Brake(void);
void sendTorque(void);
void SendInverterData(void);
void SendInverterErr(void);
/* USER CODE END */

void main(void)
{
/* USER CODE BEGIN (3) */
    Request1=0;
    Request2=0;
    Request3=0;
    Request4=0;
    errflag=0;
    startflag=0;
    currentState=Idle;
    nextState=Idle;
    INVERTER_DATA_BUFFER.OUTPUT_VOLTAGE =0;
    INVERTER_DATA_BUFFER.ACTUAL_CURRENT =1;
    INVERTER_DATA_BUFFER.DC_VOLTAGE =400;
    INVERTER_DATA_BUFFER.DC_POWER =3;
    INVERTER_DATA_BUFFER.RESOLVER_SIGNAL =4;
    INVERTER_DATA_BUFFER.MOTOR_TEMPERATURE =5;
    INVERTER_DATA_BUFFER.IGBT_TEMPERATURE = 6;

    /* initialization  */
    sciInit();
    adcInit();
    canInit();
    rtiInit();
    rtiEnableNotification(rtiNOTIFICATION_COMPARE0);
    _enable_IRQ();
    while(1)
    {

        sciSend(scilinREG, 6, (unsigned char *)"State:");
        NumberofstateChars = ltoa((unsigned int)currentState, (char *)stateshow);
        sciSend(scilinREG,NumberofstateChars,stateshow);
        sciSend(scilinREG, 2, (unsigned char *)"\r\n");


        if(currentState==Idle)
        {
             // initialize bamocar
             // torque request = 0
              if( canIsRxMessageArrived(canREG1,canMESSAGE_BOX1))
              {
                 canGetData(canREG1, canMESSAGE_BOX1, read1);
                 nextState = NO_TORQUE;
              }
              else
              {
                nextState=Idle;
              }
              currentState = nextState;
        }

        else if(currentState==TorqueSR)
        {
             sendTorque();
             nextState=TorqueSR;
             if(canIsRxMessageArrived(canREG1,canMESSAGE_BOX2))
             {
                 canGetData(canREG1, canMESSAGE_BOX2, read2);
             SendInverterData();
             }
             if( canIsRxMessageArrived(canREG1,canMESSAGE_BOX6))
             {
                 canGetData(canREG1, canMESSAGE_BOX6, read3);
                  nextState=NO_TORQUE;
             }
             currentState = nextState;
        }

        else if(currentState==NO_TORQUE)
        {
            nextState=NO_TORQUE;
            if(canIsRxMessageArrived(canREG1,canMESSAGE_BOX2))
            {

              canGetData(canREG1, canMESSAGE_BOX2, read2);
              SendInverterData();
            }
            if(canIsRxMessageArrived(canREG1,canMESSAGE_BOX3))
             {
               canGetData(canREG1, canMESSAGE_BOX3, read4);
               errflag=0;
               nextState=TorqueSR;
             }

             currentState = nextState;
        }

    }

/* USER CODE END */

}


/* USER CODE BEGIN (4) */

void Show_torque(void)
{
    uint16 NumberOfCharsOfTorque;
    NumberOfCharsOfTorque = ltoa((unsigned int)Request1, (char *)TorqueShow);
    sciSend(scilinREG, 11, (unsigned char *)"channel 1 :");
    sciSend(scilinREG, NumberOfCharsOfTorque, TorqueShow);
    sciSend(scilinREG, 4, (unsigned char *)"    ");

    NumberOfCharsOfTorque= ltoa((unsigned int)Request2, (char *)TorqueShow);
    sciSend(scilinREG, 11, (unsigned char *)"channel 2 :");
    sciSend(scilinREG, NumberOfCharsOfTorque, TorqueShow);
    sciSend(scilinREG, 2, (unsigned char *)"\r\n");
}
void Show_Brake(void)
{
    uint16 NumberOfCharsOfBrake;
    NumberOfCharsOfBrake = ltoa((unsigned int)Request3, (char *)TorqueShow);
    sciSend(scilinREG, 11, (unsigned char *)"channel 3 :");
    sciSend(scilinREG, NumberOfCharsOfBrake, TorqueShow);
    sciSend(scilinREG, 4, (unsigned char *)"    ");

    NumberOfCharsOfBrake= ltoa((unsigned int)Request4, (char *)TorqueShow);
    sciSend(scilinREG, 11, (unsigned char *)"channel 4 :");
    sciSend(scilinREG, NumberOfCharsOfBrake, TorqueShow);
    sciSend(scilinREG, 2, (unsigned char *)"\r\n");
}

void rtiNotification(uint32 notification)
{
    Request1=0;
    Request2=0;
    errflag=1;

}

void sendTorque(void)
{
    if(errflag==0)
          {
             GetTorueRequest();
          }
          implausability=Check_Torque_Plausibility();
          if(implausability==0)
          {
           rtiStartCounter(rtiCOUNTER_BLOCK0);
          }
          else
          {
           rtiStopCounter(rtiCOUNTER_BLOCK0);
           rtiResetCounter(rtiCOUNTER_BLOCK0);
          }

          Show_torque();
          GetBrakeRequest();
          Show_Brake();
          temp=Request1;
          tx_data[0]=temp & 0xff;
          tx_data[1]=temp>>8;
        // canTransmit(canREG1, canMESSAGE_BOX1, tx_data);
}


uint32_t checkpackets(uint8_t *src_packet,uint8_t *dst_packet,uint32_t psize)
{
    uint32_t err=0;
    uint32_t cnt=psize;

    while(cnt--)
    {
        if((*src_packet++) != (*dst_packet++))
        {
            err++;
        }
    }
    return (err);
}
void SendInverterData(void)
{

     ltoa((unsigned int)INVERTER_DATA_BUFFER.DC_VOLTAGE, (char *)INV_DATA_BUFFER);
       for(i=0;i<4;i++)
        {
           INVERTER_DATA_1[i] = INV_DATA_BUFFER[i];
        }

     ltoa((unsigned int)INVERTER_DATA_BUFFER.DC_POWER, (char *)INV_DATA_BUFFER);
       for(;i<8;i++)
        {
           INVERTER_DATA_1[i] = INV_DATA_BUFFER[i-4];
        }

        canTransmit(canREG1, canMESSAGE_BOX5, INVERTER_DATA_1);
        /*
      ltoa((unsigned int)INVERTER_DATA_BUFFER.RESOLVER_SIGNAL, (char *)INV_DATA_BUFFER);
        for(i=0;i<4;i++)
        {
            INVERTER_DATA_2[i] = INV_DATA_BUFFER[i];
        }
      ltoa((unsigned int)INVERTER_DATA_BUFFER.MOTOR_TEMPERATURE, (char *)INV_DATA_BUFFER);
        for(;i<8;i++)
        {
            INVERTER_DATA_2[i] = INV_DATA_BUFFER[i-4];
        }
        canTransmit(canREG1, canMESSAGE_BOX5, INVERTER_DATA_2);

      ltoa((unsigned int)INVERTER_DATA_BUFFER.IGBT_TEMPERATURE, (char *)INV_DATA_BUFFER);
        for(i=0;i<4;i++)
        {
            INVERTER_DATA_3[i] = INV_DATA_BUFFER[i];
        }
      ltoa((unsigned int)INVERTER_DATA_BUFFER.INVERTER_ERROR_WARNING, (char *)INV_DATA_BUFFER);
        for(;i<8;i++)
        {
             INVERTER_DATA_3[i] = INV_DATA_BUFFER[i-4];
        }
        canTransmit(canREG1, canMESSAGE_BOX5, INVERTER_DATA_3);
*/
    /*
    ltoa((unsigned int)INVERTER_DATA_BUFFER.OUTPUT_VOLTAGE, (char *)INV_DATA_BUFFER);
    for(i=0;i<4;i++)
    {
        INVERTER_DATA[i] = INV_DATA_BUFFER[i];
    }
    ltoa((unsigned int)INVERTER_DATA_BUFFER.ACTUAL_CURRENT, (char *)INV_DATA_BUFFER);
    for(;i<8;i++)
    {
         INVERTER_DATA[i] = INV_DATA_BUFFER[i-4];
    }
    ltoa((unsigned int)INVERTER_DATA_BUFFER.DC_VOLTAGE, (char *)INV_DATA_BUFFER);
    for(;i<12;i++)
    {
         INVERTER_DATA[i] = INV_DATA_BUFFER[i-8];
    }
    ltoa((unsigned int)INVERTER_DATA_BUFFER.DC_POWER, (char *)INV_DATA_BUFFER);
    for(;i<16;i++)
    {
         INVERTER_DATA[i] = INV_DATA_BUFFER[i-12];
    }
    ltoa((unsigned int)INVERTER_DATA_BUFFER.RESOLVER_SIGNAL, (char *)INV_DATA_BUFFER);
    for(;i<20;i++)
    {
         INVERTER_DATA[i] = INV_DATA_BUFFER[i-16];
    }
    ltoa((unsigned int)INVERTER_DATA_BUFFER.MOTOR_TEMPERATURE, (char *)INV_DATA_BUFFER);
    for(;i<24;i++)
    {
          INVERTER_DATA[i] = INV_DATA_BUFFER[i-20];
    }
    ltoa((unsigned int)INVERTER_DATA_BUFFER.IGBT_TEMPERATURE, (char *)INV_DATA_BUFFER);
    for(;i<28;i++)
    {
          INVERTER_DATA[i] = INV_DATA_BUFFER[i-24];
    }
    canTransmit(canREG1, canMESSAGE_BOX5, INVERTER_DATA);
*/
}
void canMessageNotification(canBASE_t *node, uint32 messageBox)
{
    return;
    }
void sciNotification(sciBASE_t *sci, unsigned flags)
{
    return;
}

void canErrorNotification(canBASE_t *node, uint32 notification)
{
    return;
    }

void esmGroup1Notification(unsigned channel)
{
    return;
    }

void esmGroup2Notification(unsigned channel)
{
    return;
    }

/* USER CODE END */
