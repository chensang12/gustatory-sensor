#include "KeyScan.h"


KEY Key1 = {0};
KEY Key2 = {0};

/**
 * 按键初始化
 * @param Key 按键状态结构体
 */
void Key_GPIO_Init(KEY *Key) {


    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pin = Key->Key_Pin;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;

    HAL_GPIO_Init(Key->Key_Port, &GPIO_InitStructure);

}
/**
 * 初始化外部接口
 */
void Key_Init(void) {
    Key1.Key_Pin = KEY1_PIN;
    Key1.Key_Port = KEY1_PORT;
    Key1.Key_Status = KEY_OFF;

    Key2.Key_Pin = KEY2_PIN;
    Key2.Key_Port = KEY2_PORT;
    Key2.Key_Status = KEY_OFF;

    Key_GPIO_Init(&Key1);
    Key_GPIO_Init(&Key2);
}



/**
 * 按键检测函数
 * @param Key 按键信息结构体,每次会更新状态
 */
void Key_Scan(KEY *Key) {

    if (HAL_GPIO_ReadPin(Key->Key_Port, Key->Key_Pin) == KEY_ON) {
        HAL_Delay(10);
        if (HAL_GPIO_ReadPin(Key->Key_Port, Key->Key_Pin) == KEY_ON) {
            while (HAL_GPIO_ReadPin(Key->Key_Port, Key->Key_Pin) == KEY_ON);
            HAL_Delay(10);
            Key->Key_Status = KEY_ON;
            return;
        }
    }
    Key->Key_Status = KEY_OFF;

}
/**
 *外部读取按键状态接口函数
 * @return 按键当前状态
 */
KEY_STATUS Get_Key1Status(void) {
    Key_Scan(&Key1);
    return Key1.Key_Status;
}

/**
 * 外部读取按键状态接口函数
 * @return 按键当前状态
 */
KEY_STATUS Get_Key2Status(void) {
    Key_Scan(&Key2);
    return Key2.Key_Status;
}