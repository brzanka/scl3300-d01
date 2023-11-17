/*
 * scl3300-d01.h
 *
 *  Created on: Jul 27, 2023
 *      Author: Anna Twarowska
 */

#ifndef INC_SCL3300_D01_H_
#define INC_SCL3300_D01_H_

#include <stdint.h>
#include <stdio.h>

/* uncomment mode used only */
#define WHO_AM_I_SCL3300		0x40000091
#define BANK0					0xFC000073	//used always apart from SERIAL reading
#define BANK1					0xFC00016E	//default bank
#define MODE1					0xB400001F  //default mode
//#define MODE2					0xB4000102
//#define MODE3					0xB4000225
//#define MODE4					0xB4000338
#define ENABLE_ANGLE			0xB0001F6F	//default disable
#define READ_ACC_X  			0x040000F7
#define READ_ACC_Y  			0x080000FD
#define READ_ACC_Z  			0x0C0000FB
#define READ_Temperature  		0x140000EF
#define READ_ANG_X  			0x240000C7
#define READ_ANG_Y   			0x280000CD
#define READ_ANG_Z   			0x2C0000CB
#define SERIAL1   				0x640000A7	//before reading switch to BANK1
#define SERIAL2  				0x680000AD
#define STATUS					0x180000E5	//reading resets the register
#define ERR_FLAG1				0x1C0000E3
#define ERR_FLAG2				0x200000C1
#define SW_RESET 				0xB4002098

#define WHO_AM_I_SCL3300_RSP	0xC1
#define ACC_SENSITIVITY_1		6000		//LSB/g
#define ACC_SENSITIVITY_2		3000
#define ACC_SENSITIVITY_34		12000
#define ACC_SENSITIVITY_ANG_1	105			//LSB/°
#define ACC_SENSITIVITY_ANG_2	52
#define ACC_SENSITIVITY_ANG_34	209
#define ANG_SENSITIVITY			182 		//LSB/°
#define TEMP_SENSITIVITY		19			//LSB/°C
#define TEMP_OFFSET				-273		//°C

typedef struct
{
  int16_t ACC_X;
  int16_t ACC_Y;
  int16_t ACC_Z;
  int16_t Temperature;
  int16_t ANG_X;
  int16_t ANG_Y;
  int16_t ANG_Z;
} scl3300Data;

typedef struct
{
  int ACC_SENS;
  int ANG_SENS;
  int TEMP_SENS;
  uint32_t MODE;
} modeSensitivity;

uint8_t checkScl3300();
void scl3300Init(modeSensitivity mode);
scl3300Data readData();
void displayData(scl3300Data data, modeSensitivity mode);
float calcSens(uint16_t data, int mode);
uint32_t scl300STransmitReceive(uint32_t value);
void scl300SWriteCmd(uint32_t value);

#endif /* INC_SCL3300_D01_H_ */
