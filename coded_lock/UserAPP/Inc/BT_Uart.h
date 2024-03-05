#ifndef __BT_UART_H
#define __BT_UART_H

#include "main.h"
#include "stm32f1xx_hal.h"


uint8_t UsartData_Analysis(uint8_t *Usart_DataBuff, uint8_t *DataBuff, uint8_t Length);

uint8_t String_Compare(const uint8_t *S1, const uint8_t *S2);

uint8_t String_Compare1(const uint8_t *S1, const uint8_t *S2, uint8_t Length);

#endif