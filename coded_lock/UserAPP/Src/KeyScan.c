#include "KeyScan.h"


/*按键列表*/
Key Key_list[8] = {0};
/*矩阵按键对应的键值*/
uint8_t KeyValue[4][4] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, ' ', ' ', 'E', 'X', 'B', ' '};

/**
 * 按键初始化函数
 */
void Key_Init(void) {

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

    GPIO_InitStructure.Pin = KEY0_PIN;

    HAL_GPIO_Init(KEY0_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = KEY1_PIN;
    HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = KEY2_PIN;
    HAL_GPIO_Init(KEY2_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = KEY3_PIN;
    HAL_GPIO_Init(KEY3_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = KEY4_PIN;
    HAL_GPIO_Init(KEY4_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = KEY5_PIN;
    HAL_GPIO_Init(KEY5_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = KEY6_PIN;
    HAL_GPIO_Init(KEY6_PORT, &GPIO_InitStructure);


    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Pin = KEY7_PIN;
    HAL_GPIO_Init(KEY7_PORT, &GPIO_InitStructure);

    for (int i = 0; i < 8; ++i) {
        switch (i) {
            case 0:
                Key_list[i].Key_Pin = KEY0_PIN;
                Key_list[i].Key_Port = KEY0_PORT;
                break;
            case 1:
                Key_list[i].Key_Pin = KEY1_PIN;
                Key_list[i].Key_Port = KEY1_PORT;
                break;
            case 2:
                Key_list[i].Key_Pin = KEY2_PIN;
                Key_list[i].Key_Port = KEY2_PORT;
                break;
            case 3:
                Key_list[i].Key_Pin = KEY3_PIN;
                Key_list[i].Key_Port = KEY3_PORT;
                break;
            case 4:
                Key_list[i].Key_Pin = KEY4_PIN;
                Key_list[i].Key_Port = KEY4_PORT;
                break;
            case 5:
                Key_list[i].Key_Pin = KEY5_PIN;
                Key_list[i].Key_Port = KEY5_PORT;
                break;
            case 6:
                Key_list[i].Key_Pin = KEY6_PIN;
                Key_list[i].Key_Port = KEY6_PORT;
                break;
            case 7:
                Key_list[i].Key_Pin = KEY7_PIN;
                Key_list[i].Key_Port = KEY7_PORT;
                break;
            default:
                break;
        }
    }

}

/**
 * 获取按键是否按下
 * @param Key 按键结构体对应的地址
 * @return 按键按下返回 0, 无按下返回1
 */
GPIO_PinState Get_KeyValue(Key *Key) {
    uint32_t Timeout = 0;
    if (HAL_GPIO_ReadPin(Key->Key_Port, Key->Key_Pin) == 0) {
        Delay_ms(10);
        if (HAL_GPIO_ReadPin(Key->Key_Port, Key->Key_Pin) == 0) {
            Delay_ms(10);
            while (HAL_GPIO_ReadPin(Key->Key_Port, Key->Key_Pin) == 0 && (Timeout < 90000)) {
                Timeout++;
            }
            return 0;
        } else {
            return 1;
        }
    }
    return 1;
}

/**
 * 矩阵按键扫描
 * @return 根据按键按下的位置返回对应的键值Key_Value[i][j]
 */
uint8_t KeyScan(void) {
    R_GPIO_PULLUP();
    C_GPIO_PULLDOWN();
    for (int i = 0; i < 4; ++i) {

        /* 横列扫描 , 纵列要拉低 */
        if (Get_KeyValue((Key *) &Key_list[i]) == 0) {
            C_GPIO_PULLUP();
            R_GPIO_PULLDOWN();
            for (int j = 4; j < 8; ++j) {
                if (Get_KeyValue((Key *) &Key_list[j]) == 0) {
                    return KeyValue[i][j - 4];
                }
            }
        }
    }
    return 0XFF;
}

void R_GPIO_PULLUP(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    for (int i = 0; i < 4; ++i) {
        GPIO_InitStructure.Pin = Key_list[i].Key_Pin;
        HAL_GPIO_Init(Key_list[i].Key_Port, &GPIO_InitStructure);
    }
}

void R_GPIO_PULLDOWN(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    for (int i = 0; i < 4; ++i) {
        GPIO_InitStructure.Pin = Key_list[i].Key_Pin;
        HAL_GPIO_Init(Key_list[i].Key_Port, &GPIO_InitStructure);
        HAL_GPIO_WritePin(Key_list[i].Key_Port, Key_list[i].Key_Pin, 0);
    }
}


void C_GPIO_PULLUP(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    for (int i = 0; i < 4; ++i) {
        GPIO_InitStructure.Pin = Key_list[4 + i].Key_Pin;
        HAL_GPIO_Init(Key_list[4 + i].Key_Port, &GPIO_InitStructure);
    }

}

void C_GPIO_PULLDOWN(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    for (int i = 0; i < 4; ++i) {
        GPIO_InitStructure.Pin = Key_list[i + 4].Key_Pin;
        HAL_GPIO_Init(Key_list[i + 4].Key_Port, &GPIO_InitStructure);
        HAL_GPIO_WritePin(Key_list[i + 4].Key_Port, Key_list[i + 4].Key_Pin, 0);
    }
}

