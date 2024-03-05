#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H


#include "stm32f1xx_hal.h"
#include "Delay.h"

/* 按键接口定义 */
#define KEY0_PORT       GPIOA
#define KEY1_PORT       GPIOA
#define KEY2_PORT       GPIOA
#define KEY3_PORT       GPIOA
#define KEY4_PORT       GPIOA
#define KEY5_PORT       GPIOA
#define KEY6_PORT       GPIOA
#define KEY7_PORT       GPIOB


#define KEY0_PIN        GPIO_PIN_1
#define KEY1_PIN        GPIO_PIN_2
#define KEY2_PIN        GPIO_PIN_3
#define KEY3_PIN        GPIO_PIN_4
#define KEY4_PIN        GPIO_PIN_5
#define KEY5_PIN        GPIO_PIN_6
#define KEY6_PIN        GPIO_PIN_7
#define KEY7_PIN        GPIO_PIN_0


typedef struct {
    uint16_t Key_Pin;
    GPIO_TypeDef *Key_Port;
} Key;


void Key_Init(void);

GPIO_PinState Get_KeyValue(Key *Key);

uint8_t KeyScan(void);


void R_GPIO_PULLUP(void);

void R_GPIO_PULLDOWN(void);

void C_GPIO_PULLUP(void);

void C_GPIO_PULLDOWN(void);

uint8_t KeyScan1(void);

#endif