#ifndef __SOFT_I2C_H
#define __SOFT_I2C_H

#include "main.h"
#include "stm32f1xx_hal.h"
#include "Delay.h"

/* DEFINE */

typedef struct {
    GPIO_TypeDef *GPIO_SCL_Port;
    GPIO_TypeDef *GPIO_SDA_Port;
    uint16_t GPIO_SCL_Pin;
    uint16_t GPIO_SDA_Pin;
} SOFT_I2C;

#define SOFT_I2C_SCL_PORT   GPIOB
#define SOFT_I2C_SDA_PORT   GPIOB

#define SOFT_I2C_SCL_PIN   GPIO_PIN_9
#define SOFT_I2C_SDA_PIN   GPIO_PIN_8

#define SOFT_I2C_SCL_SET(SOFT_I2C)        HAL_GPIO_WritePin(SOFT_I2C->GPIO_SCL_Port, SOFT_I2C->GPIO_SCL_Pin, GPIO_PIN_SET)
#define SOFT_I2C_SCL_RESET(SOFT_I2C)      HAL_GPIO_WritePin(SOFT_I2C->GPIO_SCL_Port, SOFT_I2C->GPIO_SCL_Pin, GPIO_PIN_RESET)

#define SOFT_I2C_SDA_SET(SOFT_I2C)        HAL_GPIO_WritePin(SOFT_I2C->GPIO_SDA_Port, SOFT_I2C->GPIO_SDA_Pin, GPIO_PIN_SET)
#define SOFT_I2C_SDA_RESET(SOFT_I2C)      HAL_GPIO_WritePin(SOFT_I2C->GPIO_SDA_Port, SOFT_I2C->GPIO_SDA_Pin, GPIO_PIN_RESET)

#define SOFT_I2C_SDA_W(SOFT_I2C, X)       HAL_GPIO_WritePin(SOFT_I2C->GPIO_SDA_Port, SOFT_I2C->GPIO_SDA_Pin, (GPIO_PinState)X)
#define SOFT_I2C_SCL_W(SOFT_I2C, X)       HAL_GPIO_WritePin(SOFT_I2C->GPIO_SCL_Port, SOFT_I2C->GPIO_SCL_Pin, (GPIO_PinState)X)

#define SOFT_I2C_SDA_R(SOFT_I2C)          HAL_GPIO_ReadPin(SOFT_I2C->GPIO_SDA_Port, SOFT_I2C->GPIO_SDA_Pin)


#include "Soft_I2C.h"

/* FUNC */
void Soft_I2C_GPIO_Init(SOFT_I2C *Soft_I2C);

void SOFT_I2C_Start(SOFT_I2C *Soft_I2C);

void SOFT_I2C_Stop(SOFT_I2C *Soft_I2C);

void SOFT_I2C_SendByte(SOFT_I2C *Soft_I2C, uint8_t SendByte);

uint8_t SOFT_I2C_ReceiveByte(SOFT_I2C *Soft_I2C);

void SOFT_I2C_SendAckBit(SOFT_I2C *Soft_I2C, uint8_t AckBit);

uint8_t SOFT_I2C_ReceiveAckBit(SOFT_I2C *Soft_I2C);

void SOFT_I2C_Master_Transmit(SOFT_I2C *Soft_I2C, uint8_t DevAddress, uint8_t *pData, uint16_t Size);


#endif