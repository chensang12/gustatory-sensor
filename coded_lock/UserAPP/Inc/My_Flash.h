#ifndef __MY_FLASH_H
#define __MY_FLASH_H

#include "main.h"
#include "Lock.h"
#include "retarget.h"
#include "stm32f1xx_hal.h"


void My_Flash_ClearPage(uint32_t PageAdd);

uint8_t My_Flash_ReadByte(uint32_t DataAddress);

void My_Flash_WriteByteArray(const uint8_t *Data, uint32_t Length, uint32_t PageAddress, uint32_t DataAddress);

void My_Flash_ReadByteArray(uint8_t *Arr, uint32_t Length, uint32_t DataAddress);

void My_Flash_WriteByte(uint8_t Data, uint32_t PageAddress, uint32_t DataAddress);

void My_Flash_WriteShort(uint16_t Data, uint32_t PageAddress, uint32_t DataAddress);

uint16_t My_Flash_ReadData(uint32_t DataAddress);

void My_FLASH_Write_Array(uint32_t PageAddress, const uint16_t *Array, uint16_t Length, uint32_t BaseAddress);


void My_Flash_WriteByteArray(const uint8_t *Data, uint32_t Length, uint32_t PageAddress, uint32_t DataAddress);


#endif