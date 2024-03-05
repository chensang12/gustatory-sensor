#ifndef __OLED_H
#define __OLED_H


#define I2C_HARDWARE_ENABLE 0

#include "main.h"

#if I2C_HARDWARE_ENABLE
#include "i2c.h"
#else

#include "Soft_I2C.h"


#define I2C_OLED_SDA_Port   GPIOB
#define I2C_OLED_SCL_Port   GPIOB

#define I2C_OLED_SDA_Pin   GPIO_PIN_9
#define I2C_OLED_SCL_Pin   GPIO_PIN_8

#endif


void OLED_Init(void);

void OLED_Clear(void);

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);

void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);

void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);


#endif