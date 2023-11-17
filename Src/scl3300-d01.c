/*
 * scl3300-d01.c
 *
 *  Created on: Jul 27, 2023
 *      Author: Anna Twarowska
 */
// check crc?? Delay of 10 mikrosec between
// module answering for the previous message
//motorola or ti check

#include "scl3300-d01.h"
#include "spi.h"
#include "usart.h"

void scl300SWriteCmd(uint32_t value)
{
  uint8_t temp[4];
  temp[0] = (uint8_t) (value>>24);
  temp[1] = (uint8_t) (value>>16);
  temp[2] = (uint8_t) (value>>8);
  temp[3] = (uint8_t) value;

  HAL_SPI_Transmit(&hspi2, temp, 4, 100);
}

uint32_t scl300STransmitReceive(uint32_t value)
{
  uint8_t temp[4];
  uint8_t temp2[4];
  uint32_t data;
  temp[0] = (uint8_t) (value>>24);
  temp[1] = (uint8_t) (value>>16);
  temp[2] = (uint8_t) (value>>8);
  temp[3] = (uint8_t) value;

  HAL_SPI_TransmitReceive(&hspi2, temp, temp2, 4, 100);
  data = ((uint32_t) temp2[3]) | ((uint32_t) temp2[2] << 8) | ((uint32_t) temp2[1] << 16) | ((uint32_t) temp2[0] << 24);
  return data;
}

uint8_t checkScl3300()
{
  uint8_t value[2];
  scl300SWriteCmd(WHO_AM_I_SCL3300);
  HAL_SPI_Receive(&hspi2, (uint8_t *)&value[0], 1, 100);
  HAL_SPI_Receive(&hspi2, (uint8_t *)&value[1], 1, 100);
  return value[1];
}

void scl3300Init(modeSensitivity mode)
{
  uint32_t buffer;
  int threshold = 0;
  scl300SWriteCmd(SW_RESET);
  HAL_Delay(1);
  scl300SWriteCmd(mode.MODE);
  scl300SWriteCmd(ENABLE_ANGLE);
  HAL_Delay(100);
  do
  {
    if (threshold == 20)
    {
	  HAL_UART_Transmit(&huart1, (uint8_t *)"Device might be broken.\r\n", 27, 100);
	  break;
    }
    buffer = scl300STransmitReceive(STATUS);
    threshold += 1;
  } while (buffer != 0);
}

scl3300Data readData()
{
  uint32_t buffer;
  scl3300Data data;

  buffer = scl300STransmitReceive(READ_ACC_X);
  data.ACC_X = (uint16_t)(buffer<<8)>>16;

  buffer = scl300STransmitReceive(READ_ACC_Y);
  data.ACC_Y = (uint16_t)(buffer<<8)>>16;

  buffer = scl300STransmitReceive(READ_ACC_Z);
  data.ACC_Z = (uint16_t)(buffer<<8)>>16;

  buffer = scl300STransmitReceive(READ_Temperature);
  data.Temperature = (uint16_t)(buffer<<8)>>16;

  buffer = scl300STransmitReceive(READ_ANG_X);
  data.ANG_X = (uint16_t)(buffer<<8)>>16;

  buffer = scl300STransmitReceive(READ_ANG_Y);
  data.ANG_Y = (uint16_t)(buffer<<8)>>16;

  buffer = scl300STransmitReceive(READ_ANG_Z);
  data.ANG_Z = (uint16_t)(buffer<<8)>>16;

  return data;
}

void displayData(scl3300Data data, modeSensitivity mode)
{
  char str1[10] = "";
  float realValue;

  realValue = calcSens(data.ACC_X, mode.ACC_SENS);
  sprintf(str1, "%.2f ", realValue);
  HAL_UART_Transmit(&huart1, (uint8_t*)&str1, 5, 100);

  realValue = calcSens(data.ACC_Y, mode.ACC_SENS);
  sprintf(str1, "%.2f ", realValue);
  HAL_UART_Transmit(&huart1, (uint8_t*)&str1, 5, 100);

  realValue = calcSens(data.ACC_Z, mode.ACC_SENS);
  sprintf(str1, "%.2f ", realValue);
  HAL_UART_Transmit(&huart1, (uint8_t*)&str1, 5, 100);

  realValue = calcSens(data.Temperature, mode.TEMP_SENS);
  sprintf(str1, "%.2f ", realValue);
  HAL_UART_Transmit(&huart1, (uint8_t*)&str1, 5, 100);

  realValue = calcSens(data.ANG_X, mode.ANG_SENS);
  sprintf(str1, "%.2f ", realValue);
  HAL_UART_Transmit(&huart1, (uint8_t*)&str1, 5, 100);

  realValue = calcSens(data.ANG_Y, mode.ANG_SENS);
  sprintf(str1, "%.2f ", realValue);
  HAL_UART_Transmit(&huart1, (uint8_t*)&str1, 5, 100);

  realValue = calcSens(data.ANG_Z, mode.ANG_SENS);
  sprintf(str1, "%.2f ", realValue);
  HAL_UART_Transmit(&huart1, (uint8_t*)&str1, 5, 100);
}

float calcSens(uint16_t data, int mode)
{
  float result;
  result = data / mode;
  return result;
}
