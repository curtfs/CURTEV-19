/** @file sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com 
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "sys_common.h"
#include "can.h"
#include "sci.h"
#include "stdlib.h"

/* USER CODE BEGIN (1) */
unsigned char command[9];
#define  D_SIZE 8
uint8  tx_data1[D_SIZE]  = {0};
uint8 tx_data2[D_SIZE]  = {0};
uint8  tx_data3[D_SIZE]  = {0};
uint8  tx_data4[D_SIZE]  = {0};
uint16 PackCurrent=1200; // 0.1A
uint16 PackVoltage=4100; // 0.1V
uint8 SOC=200;
uint8 packhealth=95;
int highestTemp= 30;
int lowestTemp=-5;
int avgTemp= 25;
uint8 highestThermID=3;
uint8 lowestThermID=50;
int internalTemp=33;
uint16 MinCellVoltage=10000; // 0.0001V
uint16 MaxCellVoltage=50000; //0.0001V
uint8_t highestVoltID=2;
uint8_t lowestVoltID=49;
uint8_t FanSpeed=6;



/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */



/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    // Message 1 Virtual Data
    tx_data1[0]=0b00000000; // Relay State
    tx_data1[1]=0b11111111; // Relay State
    tx_data1[2]=PackCurrent & 0xff; //Masking PackCurrent Data
    tx_data1[3]=(PackCurrent >> 8);
    tx_data1[4]=PackVoltage & 0xff; //Masking PackVoltage Data
    tx_data1[5]=(PackVoltage >> 8);
    tx_data1[6]=SOC;
    tx_data1[7]=20; //CheckSUM

    // Message 2 Virtual Data
    tx_data2[0]=packhealth;
    tx_data2[1]=(uint8)highestTemp;
    tx_data2[2]=(uint8)lowestTemp;
    tx_data2[3]=(uint8)avgTemp;
    tx_data2[4]=highestThermID;
    tx_data2[5]=lowestThermID;
    tx_data2[6]=(uint8)internalTemp;
    tx_data2[7]=20; //CheckSUM

    // Message 3 Virtual Data
    tx_data3[0]=MinCellVoltage & 0xff;
    tx_data3[1]=(MinCellVoltage >> 8);
    tx_data3[2]=highestVoltID;
    tx_data3[3]=lowestVoltID;
    tx_data3[4]=MaxCellVoltage & 0xff;
    tx_data3[5]=(MaxCellVoltage >> 8);
    tx_data3[6]=FanSpeed;
    tx_data3[7]=20; //CheckSUM

    // Message 4 Virtual Data
    tx_data3[0]=0x00; //DTC Flag
    tx_data3[1]=0x01; //DTC Flag
    tx_data3[2]=0x02; //DTC Flag
    tx_data3[3]=0x03; //DTC Flag
    tx_data3[4]=20; //CheckSUM


  while(1)
  {
    canInit();
    canTransmit(canREG1, canMESSAGE_BOX1, tx_data1);
    canTransmit(canREG1, canMESSAGE_BOX2, tx_data2);
    canTransmit(canREG1, canMESSAGE_BOX3, tx_data3);
    canTransmit(canREG1, canMESSAGE_BOX4, tx_data4);
  }1
/* USER CODE END */


}


/* USER CODE BEGIN (4) */
void canMessageNotification( canBASE_t *node,uint32_t messageBox){return;}
void canErroeNotification( canBASE_t *node,uint32_t notification){return;}
void esmGroup1Notification(int bit){return;}
void esmGroup2Notification(int bit){return;}
/* USER CODE END */
