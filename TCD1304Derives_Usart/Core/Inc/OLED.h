#ifndef __OLED_H
#define __OLED_H

#include "main.h"

#include "Soft_I2C.h"


#define I2C_HARDWARE_ENABLE 0

#define OLED_GRAM_ENABLE    0

#if OLED_GRAM_ENABLE



void OLED_DisplayTurn(uint8_t i);

void OLED_ReDraw(void);
void OLED_ClearBuff(void);
void OLED_DrawPoint(uint8_t x, uint8_t y);
void OLED_ClearPoint(uint8_t x, uint8_t y);
void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);


#else


#define I2C_OLED_SDA_Port   GPIOB
#define I2C_OLED_SCL_Port   GPIOB

#define I2C_OLED_SDA_Pin   GPIO_PIN_9
#define I2C_OLED_SCL_Pin   GPIO_PIN_8


void OLED_I2C_Init(void);

void OLED_WriteData1(uint8_t Data);

void OLED_DisplayTurn(uint8_t i);

void OLED_Refresh(void);

void OLED_ReDraw(void);

void OLED_ClearBuff(void);

void OLED_DrawPoint(uint8_t x, uint8_t y);

void OLED_ClearPoint(uint8_t x, uint8_t y);

void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);


void OLED_Init(void);

void OLED_Clear(void);

void OLED_DisPlay_On(void);

void OLED_DisPlay_Off(void);

void OLED_ShowChar(uint8_t x, uint8_t y, char Char, uint8_t Font_Size);

void OLED_ShowString(uint8_t x, uint8_t y, char *String, uint8_t Font_Size);

void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t Number, uint8_t Length, uint8_t Font_Size);

void OLED_ShowSignedNum(uint8_t x, uint8_t y, int32_t Number, uint8_t Length, uint8_t Font_Size);

void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Font_Size);

void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Font_Size);

void OLED_Shift(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t Fo);

void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[]);

void
OLED_MovePicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[], uint8_t NextX, uint8_t NextY);

void tian(uint8_t x, uint8_t y);

void PWM_DisPlay(void);

#endif


#endif