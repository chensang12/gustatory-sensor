#ifndef __AHT2O_H
#define __AHT2O_H


#include "main.h"

#include "Soft_I2C.h"

#define AHT2O_SLAVE_ADDRESS (0X38 << 1)


/**
 *AHT2O内部地址
 */
#define AHT20_STATUS_REG        0X00    //状态字 寄存器地址
#define AHT20_INIT_REG          0XBE    //初始化 寄存器地址
#define AHT20_SOFTRESET         0XBA    //软复位 单指令
#define AHT20_TRIGMEASURE_REG   0XAC    //触发测量  寄存器地址

/**
 * 软件I2C管脚定义
 */


#define I2C_AHT20_SDA_PORT   GPIOB
#define I2C_AHT20_SCL_PORT   GPIOB

#define I2C_AHT20_SDA_PIN    GPIO_PIN_9
#define I2C_AHT20_SCL_PIN    GPIO_PIN_8
/**
 * 存储AHT20传感器信息的结构体
 */
typedef struct {
    uint8_t Alive;  //器件是否存在
    uint8_t Flag;   //读取/计算错误标志位。 0_读取/计算数据正常 1_读取/计算设备失败
    uint32_t HT[2]; //湿度，温度原始传感器的值，20Bit

    float RH;
    float Temp;
} AHT20;


uint8_t AHT20_Init(void);

uint8_t AHT20_ReadHT(uint32_t *HT);

uint8_t StandardUnitCon(AHT20 *AHT20);

#endif /* __AHT2O_H */