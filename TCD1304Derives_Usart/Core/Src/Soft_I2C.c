#include "Soft_I2C.h"

#if 1

/**
 * 软件模拟I2C的GPIO外设初始化
 */

void Soft_I2C_GPIO_Init(SOFT_I2C *Soft_I2C) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.Pin = Soft_I2C->GPIO_SCL_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(Soft_I2C->GPIO_SCL_Port, &GPIO_InitStructure);

    GPIO_InitStructure.Pin = Soft_I2C->GPIO_SDA_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStructure.Pull = GPIO_NOPULL;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

    HAL_GPIO_Init(Soft_I2C->GPIO_SDA_Port, &GPIO_InitStructure);

    HAL_GPIO_WritePin(Soft_I2C->GPIO_SDA_Port, Soft_I2C->GPIO_SDA_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(Soft_I2C->GPIO_SCL_Port, Soft_I2C->GPIO_SCL_Pin, GPIO_PIN_SET);
}


/**
 * 软件模拟I2C开始
 */
void SOFT_I2C_Start(SOFT_I2C *Soft_I2C) {
    SOFT_I2C_SDA_SET(Soft_I2C);
    SOFT_I2C_SCL_SET(Soft_I2C);

    SOFT_I2C_SDA_RESET(Soft_I2C);
    SOFT_I2C_SCL_RESET(Soft_I2C);

}

/**
 * 软件模拟I2C结束
 */
void SOFT_I2C_Stop(SOFT_I2C *Soft_I2C) {
    SOFT_I2C_SDA_RESET(Soft_I2C);
    SOFT_I2C_SCL_SET(Soft_I2C);

    SOFT_I2C_SDA_SET(Soft_I2C);
}

/**
 * 软件模拟I2C发送一字节
 * @param SendByte 要发送的字节
 */
void SOFT_I2C_SendByte(SOFT_I2C *Soft_I2C, uint8_t SendByte) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        SOFT_I2C_SDA_W(Soft_I2C, SendByte & (0x80 >> i));
        SOFT_I2C_SCL_W(Soft_I2C, 1);
        SOFT_I2C_SCL_W(Soft_I2C, 0);
    }

}

/**
 * 软件模拟I2C接收一字节
 * @return 接收到的字节数据
 */
uint8_t SOFT_I2C_ReceiveByte(SOFT_I2C *Soft_I2C) {
    uint8_t i;
    uint8_t ReceiveByte = 0x00;

    for (i = 0; i < 8; i++) {
        SOFT_I2C_SCL_SET(Soft_I2C);
        if (SOFT_I2C_SDA_R(Soft_I2C)) { ReceiveByte |= (0x80 >> i); }
        SOFT_I2C_SCL_RESET(Soft_I2C);
    }
    return ReceiveByte;
}

/**
 * 模拟I2C发送主机接收应答位 0为应答, 1为非应答
 * @param AckBit 要发送的应答数据
 */
void SOFT_I2C_SendAckBit(SOFT_I2C *Soft_I2C, uint8_t AckBit) {
    SOFT_I2C_SCL_W(Soft_I2C, AckBit);
    SOFT_I2C_SCL_SET(Soft_I2C);
    SOFT_I2C_SCL_RESET(Soft_I2C);

}

/**
 * 软件模拟I2C接收从机应答数据(Bit)
 * @return 接收到的应答位
 */
uint8_t SOFT_I2C_ReceiveAckBit(SOFT_I2C *Soft_I2C) {
    uint8_t AckBit;
    SOFT_I2C_SDA_SET(Soft_I2C);
    SOFT_I2C_SCL_SET(Soft_I2C);

    AckBit = SOFT_I2C_SDA_R(Soft_I2C);

    SOFT_I2C_SCL_RESET(Soft_I2C);

    return AckBit;
}

void SOFT_I2C_Master_Transmit(SOFT_I2C *Soft_I2C, uint8_t DevAddress, uint8_t *pData, uint16_t Size) {
    uint8_t *i = pData;
    SOFT_I2C_Start(Soft_I2C);
    //SOFT_I2C_SendByte((DevAddress << 1) & 0xFE);
    SOFT_I2C_SendByte(Soft_I2C, DevAddress);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
    for (int j = 0; j < Size; ++j) {
        SOFT_I2C_SendByte(Soft_I2C, *i);
        SOFT_I2C_ReceiveAckBit(Soft_I2C);
        i++;
    }
    SOFT_I2C_Stop(Soft_I2C);
}

/**
 * 向I2C设备连续写地数据
 * @param Soft_I2C I2C管脚配置结构体
 * @param DevAddress I2C从机地址
 * @param RegAddress I2C从机寄存器地址
 * @param pData 要发送的数据数组
 * @param Len 要发送数据的长度
 * @return 0 正常,1写入失败
 */
uint8_t SOFT_I2C_Write(SOFT_I2C *Soft_I2C, uint8_t DevAddress, uint8_t RegAddress, uint8_t *pData, uint16_t Len) {
    SOFT_I2C_Start(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, DevAddress);
    if (!SOFT_I2C_ReceiveAckBit(Soft_I2C)) {
        SOFT_I2C_Stop(Soft_I2C);
        return 1;
    }

    SOFT_I2C_SendByte(Soft_I2C, RegAddress);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);

    for (int i = 0; i < Len; ++i) {
        SOFT_I2C_SendByte(Soft_I2C, *pData);
        if (!SOFT_I2C_ReceiveAckBit(Soft_I2C)) {
            SOFT_I2C_Stop(Soft_I2C);
            return 1;
        }
    }
    SOFT_I2C_Stop(Soft_I2C);

    return 0;
}

/**
 * 向I2C设备连续读地址
 * @param Soft_I2C I2C管脚配置结构体
 * @param DevAddress I2C从机地址
 * @param RegAddress I2C从机寄存器地址
 * @param Len 要读取的数据长度
 * @param DataBuff 读取数据缓冲区
 * @return 0为正常， 1为异常
 */
uint8_t SOFT_I2C_Read(SOFT_I2C *Soft_I2C, uint8_t DevAddress, uint8_t RegAddress, uint8_t Len, uint8_t *DataBuff) {
    SOFT_I2C_Start(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, DevAddress);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
//    if (!SOFT_I2C_ReceiveAckBit(Soft_I2C)) {
//        SOFT_I2C_Stop(Soft_I2C);
//        return 1;
//    }
    SOFT_I2C_SendByte(Soft_I2C, RegAddress);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);

    SOFT_I2C_Start(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, DevAddress | 0X01);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);

    for (int i = 0; i < Len; ++i) {
        *DataBuff = SOFT_I2C_ReceiveByte(Soft_I2C);

        if (i == Len - 1) {
            SOFT_I2C_SendAckBit(Soft_I2C, 1);
        } else {
            SOFT_I2C_SendAckBit(Soft_I2C, 0);
        }
        DataBuff++;
    }
    SOFT_I2C_Stop(Soft_I2C);

    return 0;
}

#else
void Soft_I2C_GPIO_Init(void)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();
    HAL_GPIO_WritePin(GPIOB, SOFT_I2C_SCL_PIN | SOFT_I2C_SDA_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin = SOFT_I2C_SCL_PIN| SOFT_I2C_SDA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SOFT_I2C_Start(void)
{
    SOFT_I2C_SDA_W(1);
    SOFT_I2C_SCL_W(1);
    SOFT_I2C_SDA_W(0);
    SOFT_I2C_SCL_W(0);
}

void SOFT_I2C_Stop(void)
{
    SOFT_I2C_SDA_W(0);
    SOFT_I2C_SCL_W(1);
    SOFT_I2C_SDA_W(1);
}

void SOFT_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for ( i = 0; i < 8; i++)
    {
        SOFT_I2C_SDA_W(Byte  & (0X80 >> i));
        SOFT_I2C_SCL_W(1);
        SOFT_I2C_SCL_W(0);
    }

}

uint8_t SOFT_I2C_ReceiveByte(void)
{
    uint8_t i;
    uint8_t Byte = 0X00;
    SOFT_I2C_SDA_W(1);
    for (i = 0; i < 8; i++)
    {
        SOFT_I2C_SCL_W(1);
        if(SOFT_I2C_SDA_R()){Byte |= (0X80 >> i);}
        SOFT_I2C_SCL_W(0);
    }
    return Byte;
}


void SOFT_I2C_SendAckBit(uint8_t AckBit)
{
    SOFT_I2C_SDA_W(AckBit);
    SOFT_I2C_SCL_W(1);
    SOFT_I2C_SCL_W(0);
}

uint8_t SOFT_I2C_ReceiveAckBit(void)
{
    uint8_t AckBit;
    SOFT_I2C_SDA_W(1);
    SOFT_I2C_SCL_W(1);
    AckBit = SOFT_I2C_SDA_R();
    SOFT_I2C_SCL_W(0);
    return AckBit;
}

#endif
























