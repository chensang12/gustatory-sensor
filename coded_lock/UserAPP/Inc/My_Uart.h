#ifndef __MY_UART_H
#define __MY_UART_H

#include "main.h"
#include "usart.h"
#include "stm32f1xx_hal.h"


#define HAL_StatusTypeDef_Enable    0

#if HAL_StatusTypeDef_Enable

HAL_StatusTypeDef Uart_Send_Char(char Char, uint32_t Timeout);

HAL_StatusTypeDef Uart_Send_String(char *String, uint32_t Timeout);

HAL_StatusTypeDef Uart_Send_Num(uint64_t Number, uint8_t Length, uint32_t Timeout);

HAL_StatusTypeDef Uart_Send_SignNum(int64_t Number, uint8_t Length, uint32_t Timeout);


#else


void Uart_Send_Char(char Char);

void Uart_Send_String(char *String);


uint64_t Pow(uint32_t x, uint32_t y);

void Uart_Send_Num(uint64_t Number, uint8_t Length);


void Uart_Send_SignNum(int64_t Number, uint8_t Length, uint32_t Timeout);


#endif


#endif