#include "AHT20.h"

SOFT_I2C AHT20_Soft_I2C;


static void AHT20_I2C_Init(void) {

    __HAL_RCC_GPIOB_CLK_ENABLE();
    AHT20_Soft_I2C.GPIO_SCL_Port = I2C_AHT20_SCL_PORT;
    AHT20_Soft_I2C.GPIO_SDA_Port = I2C_AHT20_SDA_PORT;

    AHT20_Soft_I2C.GPIO_SCL_Pin = I2C_AHT20_SCL_PIN;
    AHT20_Soft_I2C.GPIO_SDA_Pin = I2C_AHT20_SDA_PIN;

    Soft_I2C_GPIO_Init(&AHT20_Soft_I2C);
}

static uint8_t AHT20_ReadStatusCmd(void) {
    uint8_t Data;
    SOFT_I2C_Read(&AHT20_Soft_I2C, AHT2O_SLAVE_ADDRESS, AHT20_STATUS_REG, 1, &Data);
    return Data;
}

static uint8_t AHT20_ReadCalEnableCmd(void) {
    uint8_t Data;
    Data = AHT20_ReadStatusCmd();
    return (Data >> 3) & (0x01);
}

static uint8_t AHT20_ReadBusyCmd(void) {
    uint8_t Data;
    Data = AHT20_ReadStatusCmd();
    return (Data >> 7) & (0x01);
}

static void AHT20_InitCmd(void) {
    uint8_t InitCmd[2] = {0X08, 0X00};
    SOFT_I2C_Write(&AHT20_Soft_I2C, AHT2O_SLAVE_ADDRESS, AHT20_INIT_REG, InitCmd, 2);
}

static void AHT20_TrigMeasureCmd(void) {
    uint8_t TrigMeasureCmd[2] = {0x33, 0x00};
    SOFT_I2C_Write(&AHT20_Soft_I2C, AHT2O_SLAVE_ADDRESS, AHT20_INIT_REG, TrigMeasureCmd, 2);
}

static void AHT20_SoftResetCmd(void) {
    uint8_t SoftResetCmd;
    SOFT_I2C_Write(&AHT20_Soft_I2C, AHT2O_SLAVE_ADDRESS, AHT20_INIT_REG, &SoftResetCmd, 0);
}

/**
 * 设备初始化
 * @return 0为初始化成功, 1为初始化失败
 */
uint8_t AHT20_Init(void) {
    uint8_t Init_Cnt = 2 + 1;
    uint8_t Reset_Cnt = 2 + 1;

    AHT20_I2C_Init();

    while (--Reset_Cnt) {
        Init_Cnt = 2 + 1;
        HAL_Delay(40);
        while ((!AHT20_ReadCalEnableCmd()) && (--Init_Cnt)) {
            HAL_Delay(10);

            AHT20_InitCmd();
            HAL_Delay(200);
        }
        if (Init_Cnt) {
            break;
        } else {
            AHT20_SoftResetCmd();
            HAL_Delay(200);
        }
    }
    if (Reset_Cnt) {
        HAL_Delay(200);
        return 0;
    } else {
        return 1;
    }
}

uint8_t AHT20_ReadHT(uint32_t *HT) {
    uint8_t Busy_Flag = 3 + 1;
    uint8_t Data[6];
    uint32_t RetuDats = 0;

    AHT20_TrigMeasureCmd();
    do {
        HAL_Delay(75);
    } while (AHT20_ReadBusyCmd() && (--Busy_Flag));

//    if (Busy_Flag) {
    HAL_Delay(5);
    SOFT_I2C_Read(&AHT20_Soft_I2C, AHT2O_SLAVE_ADDRESS, AHT20_STATUS_REG, 6, Data);

    RetuDats = 0;
    RetuDats = (RetuDats | Data[1]) << 8;
    RetuDats = (RetuDats | Data[2]) << 8;
    RetuDats = (RetuDats | Data[3]);
    RetuDats = RetuDats >> 4;
    HT[0] = RetuDats;

    RetuDats = 0;
    RetuDats = (RetuDats | Data[3] << 8);
    RetuDats = (RetuDats | Data[4] << 8);
    RetuDats = (RetuDats | Data[5] << 8);
    RetuDats = RetuDats & 0XFFFFF;
    HT[1] = RetuDats;

    return 0;
//    } else {
    return 1;
//    }
}

uint8_t StandardUnitCon(AHT20 *AHT20) {
    AHT20->RH = (float) ((AHT20->HT[0] * 100) / 1048576);
    AHT20->Temp = (float) ((AHT20->HT[1] * 200 / 1048576) - 50);

    if ((AHT20->RH >= 0) && (AHT20->RH >= 100) && (AHT20->Temp >= -40) && (AHT20->RH <= 85)) {
        AHT20->Flag = 0;
        return 0;
    } else {
        AHT20->Flag = 1;
        return 1;
    }
}



