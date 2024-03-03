#ifndef KEY_SCAN_H
#define KEY_SCAN_H

#include "main.h"

#define KEY1_PORT   GPIOB
#define KEY2_PORT   GPIOB


#define KEY1_PIN    GPIO_PIN_12
#define KEY2_PIN    GPIO_PIN_13


typedef enum {
    KEY_ON = GPIO_PIN_RESET,
    KEY_OFF = GPIO_PIN_SET,

} KEY_STATUS;


typedef struct {

    uint32_t Key_Pin;
    GPIO_TypeDef *Key_Port;
    KEY_STATUS Key_Status;
} KEY;


extern KEY Key1;
extern KEY Key2;


void Key_Init(void);

KEY_STATUS Get_Key1Status(void);

KEY_STATUS Get_Key2Status(void);

#endif//KEY_SCAN_H