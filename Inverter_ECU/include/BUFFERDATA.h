/*
 * BUFFERDATA.h
 *
 *  Created on: Apr 26, 2019
 *      Author: Xyntechs
 */

#ifndef INCLUDE_BUFFERDATA_H_
#define INCLUDE_BUFFERDATA_H_

typedef struct
{
  uint32 OUTPUT_VOLTAGE;              // output voltage from register 0x8a
  uint32 ACTUAL_CURRENT;              // output actual current from register 0x5F
  uint32 DC_VOLTAGE;                  // DC voltage from register 0xEB
  uint32 DC_POWER;                    // Output power from register 0xF6
  uint32 RESOLVER_SIGNAL;             // Resolver less of signal from register 0xEC
  uint32 MOTOR_TEMPERATURE;           // motor temperature from register 0x49
  uint32 IGBT_TEMPERATURE;            // IGBT temperature from register 0x4a
  uint32 INVERTER_ERROR_WARNING;        // errors and warnings from register 0x8F
}DATA_BUFFER;

uint8_t INV_DATA_BUFFER[4];           // convert data from 32bit to 4 bytes
#endif /* INCLUDE_BUFFERDATA_H_ */
