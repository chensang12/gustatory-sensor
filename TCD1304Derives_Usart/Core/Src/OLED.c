#include "OLED.h"
#include "OLED_Font.h"

uint8_t OLED_GRAM[128][8];

#if I2C_HARDWARE_ENABLE

void OLED_I2C_Init(void) {
    MX_I2C1_Init();
}

void OLED_WriteCommand(uint8_t Command)
{
    uint8_t DataBuff[2] = {0X00, Command};
    HAL_I2C_Master_Transmit(&hi2c1, 0X78, DataBuff, 2, 20);
}

void OLED_WriteData(uint8_t Data)
{
    uint8_t DataBuff[2] = {0X40, Data};
    HAL_I2C_Master_Transmit(&hi2c1, 0X78, DataBuff, 2, 20);
}
#else

SOFT_I2C OLED_Soft_I2C;

void OLED_I2C_Init(void) {
    __HAL_RCC_GPIOB_CLK_ENABLE();
    OLED_Soft_I2C.GPIO_SCL_Port = I2C_OLED_SDA_Port;
    OLED_Soft_I2C.GPIO_SDA_Port = I2C_OLED_SCL_Port;

    OLED_Soft_I2C.GPIO_SCL_Pin = I2C_OLED_SCL_Pin;
    OLED_Soft_I2C.GPIO_SDA_Pin = I2C_OLED_SDA_Pin;

    Soft_I2C_GPIO_Init(&OLED_Soft_I2C);
}

void OLED_WriteCommand(SOFT_I2C *Soft_I2C, uint8_t Command) {
//    uint8_t DataBuff[2] = {0X00, Command};
//    SOFT_I2C_Master_Transmit(0X78, DataBuff, 2);
    SOFT_I2C_Start(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, 0X78);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, 0X00);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, Command);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
    SOFT_I2C_Stop(Soft_I2C);
}

void OLED_WriteData(SOFT_I2C *Soft_I2C, uint8_t Data) {
//    uint8_t DataBuff[2] = {0X40, Data};
//    SOFT_I2C_Master_Transmit(0X78, DataBuff, 2);
    SOFT_I2C_Start(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, 0X78);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, 0X40);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
    SOFT_I2C_SendByte(Soft_I2C, Data);
    SOFT_I2C_ReceiveAckBit(Soft_I2C);
    SOFT_I2C_Stop(Soft_I2C);
}

void OLED_WriteData1(uint8_t Data) {
    OLED_WriteData(&OLED_Soft_I2C, Data);
}

#endif


/**
  * @brief  OLED设置光标位置
  * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
  * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
  * @retval 无
  */
void OLED_SetCursor(uint8_t Y, uint8_t X) {
    OLED_WriteCommand(&OLED_Soft_I2C, 0xB0 | Y);                    //设置Y位置
    OLED_WriteCommand(&OLED_Soft_I2C, 0x10 | ((X & 0xF0) >> 4));    //设置X位置低4位
    OLED_WriteCommand(&OLED_Soft_I2C, 0x00 | (X & 0x0F));            //设置X位置高4位
}


/**
  * @brief  OLED清屏
  * @param  无
  * @retval 无
  */
void OLED_Clear(void) {
    uint8_t i, j;
    for (j = 0; j < 8; j++) {
        OLED_SetCursor(j, 0);
        for (i = 0; i < 128; i++) {
            OLED_WriteData(&OLED_Soft_I2C, 0x00);
        }
    }
}

void OLED_ClearBuff(void) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 128; ++j) {
            OLED_GRAM[j][i] = 0;
        }
    }
    OLED_ReDraw();
}


/**
 *OLED 屏幕旋转180
 * @param i 0为正常显示, 1为旋转180
 */
void OLED_DisplayTurn(uint8_t i) {
    if (i == 0) {
        OLED_WriteCommand(&OLED_Soft_I2C, 0XC8);   //正常显示
        OLED_WriteCommand(&OLED_Soft_I2C, 0XA1);
    }
    if (i == 1) {
        OLED_WriteCommand(&OLED_Soft_I2C, 0XC0);
        OLED_WriteCommand(&OLED_Soft_I2C, 0XA0);
    }
}


/**
 * 更新显存
 */
void OLED_Refresh(void) {
    for (int i = 0; i < 8; ++i) {
        OLED_WriteCommand(&OLED_Soft_I2C, 0XB0 + i);   //行起始地址
        OLED_WriteCommand(&OLED_Soft_I2C, 0X00);       //低列起始地址
        OLED_WriteCommand(&OLED_Soft_I2C, 0X10);       //高列起始地址

        for (int j = 0; j < 128; ++j) {
            OLED_WriteData(&OLED_Soft_I2C, OLED_GRAM[j][i]);
        }
    }
}
/**
 * 更新显存
 */
void OLED_ReDraw(void) {
    for (int i = 0; i < 8; ++i) {
        OLED_SetCursor(i, 0);
        for (int j = 0; j < 128; ++j) {
            OLED_WriteData(&OLED_Soft_I2C, OLED_GRAM[j][i]);
        }
    }
}

/**
 * OLED 画点函数
 * @param x 行位置 0~127
 * @param y 列位置 0~63
 */
void OLED_DrawPoint(uint8_t x, uint8_t y) {
    OLED_GRAM[x][y / 8] |= (1 << (y % 8));
}

/**
 * OLED 清点函数
 * @param x 行位置 0~127
 * @param y 列位置 0~63
 */
void OLED_ClearPoint(uint8_t x, uint8_t y) {
    OLED_GRAM[x][y / 8] &= ~(1 << (y % 8));
}

/**
 * OLED 画一条直线
 * @param x1 横轴的起始位置
 * @param y1 竖轴的起始位置
 * @param x2 横轴的结束位置
 * @param y2 竖轴的结束位置
 */


void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    for (int i = 0; (y1 > y0) ? i < (y1 - y0) : i < (y0 - y1); ++i) {
        OLED_DrawPoint(x1, (y1 > y0) ? y0 + i : y1 + i);
    }
    for (int i = 0; (x1 > x0) ? i < (x1 - x0) : i < (x0 - x1); ++i) {
        OLED_DrawPoint((x1 > x0) ? (x0 + i) : (x1 + i), y1);
    }
}




//void OLED_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
//    uint8_t k;
//    uint8_t startX;
//    uint8_t startY;
//
//    uint8_t ValueX;
//    uint8_t ValueY;
//
//    if (x1 > x0) {
//        startX = x0;
//        ValueX = x1 - x0;
//    } else {
//        startX = x1;
//        ValueX = x0 - x1;
//    }
//    if (y1 > y0) {
//        startY = y0;
//        ValueY = y1 - y0;
//    } else {
//        startY = y1;
//        ValueY = y0 - y1;
//    }
//    k = (ValueY * 10) / ValueX;
//
//    if ((ValueX != 0) && (ValueY != 0)) {
//        for (int i = 0; i < ValueX; ++i) {
//            OLED_DrawPoint(startX + i, k * (startX + i) / 10);
//        }
//    } else if (ValueX != 0) {
//        for (int i = 0; i < ValueX; ++i) {
//            OLED_DrawPoint(startX + i, startY);
//        }
//    } else if (ValueY != 0) {
//        for (int i = 0; i < ValueY; ++i) {
//            OLED_DrawPoint(startX, startY + i);
//        }
//    } else {
//        return;
//    }
//
//}

/**
 * OLED画圆函数
 * @param x 圆点x坐标
 * @param y 圆点y坐标
 * @param r 圆的半径
 */
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r) {
    int a, b, num;
    a = 0;
    b = r;
    while ((2 * b * b) >= (r * r)) {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);

        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);

        a++;
        num = (a * a + b * b) - r * r;
        if (num > 0) {
            a--;
            b--;
        }
    }
}

/**
 * 在OLED屏幕上显示一个ASCII字符
 * @param x 起始横坐标
 * @param y 起始列坐标
 * @param Char 需要显示的字符
 * @param Font_Size 字体大小
 */
void OLED_ShowChar(uint8_t x, uint8_t y, char Char, uint8_t Font_Size) {
    uint8_t temp, size;
    uint8_t y0 = y;
    /**
     * 计算当前字体对应的点阵集所占的字节
     * 先计算一页是否放的进去, (Font_Size / 8)计算是否满足一页, (Font_Size % 8 )计算溢出的页数
     *当前页数 * (字体大小 / 2)  例如 12 * 6  16 * 8   24 * 12
     */
    size = ((Font_Size / 8 + ((Font_Size % 8) ? 1 : 0)) * (Font_Size / 2));//

    for (int i = 0; i < size; ++i) {
        if (Font_Size == 12) {
            temp = ASCII_1206[Char - ' '][i];
        } else if (Font_Size == 16) {
            temp = ASCII_1608[Char - ' '][i];
        } else if (Font_Size == 24) {
            temp = ASCII_2412[Char - ' '][i];
        } else {
            return;
        }
        /*将数据点更新到缓存*/
        for (int i = 0; i < 8; ++i) {
            if (temp & 0x80) {
                OLED_DrawPoint(x, y);
            } else {
                OLED_ClearPoint(x, y);
            }
            temp <<= 1;
            y++;
            /*判断当前页是否打完*/
            if ((y - y0) == Font_Size) {
                y = y0;
                x++;
                break;
            }
        }
    }
}


/**
  * @brief  OLED显示字符串
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串，范围：ASCII可见字符
  * @retval 无
  */
void OLED_ShowString(uint8_t x, uint8_t y, char *String, uint8_t Font_Size) {
    while ((*String >= ' ') && (*String <= '~')) {
        OLED_ShowChar(x, y, *String, Font_Size);
        /*按照字符的大小依次左移*/
        x += (Font_Size / 2);
        /*判断是否需要换行*/
        if (x > (128 - Font_Size)) {
            x = 0;
            y += 2;
        }
        /*更新地址*/
        String++;
    }
}

/**
  * @brief  OLED次方函数
  * @retval 返回值等于X的Y次方
  */
uint32_t OLED_Pow(uint32_t X, uint32_t Y) {
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

/**
  * @brief  OLED显示数字（十进制，正数）
  * @param  x 起始行位置，范围：1~4
  * @param  y 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~4294967295
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowNum(uint8_t x, uint8_t y, uint32_t Number, uint8_t Length, uint8_t Font_Size) {
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(x + (i * 8), y, Number / OLED_Pow(10, Length - i - 1) % 10 + '0', Font_Size);
    }
}

/**
  * @brief  OLED显示数字（十进制，带符号数）
  * @param  x 起始行位置，范围：1~4
  * @param  y 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-2147483648~2147483647
  * @param  Length 要显示数字的长度，范围：1~10
  * @retval 无
  */
void OLED_ShowSignedNum(uint8_t x, uint8_t y, int32_t Number, uint8_t Length, uint8_t Font_Size) {
    uint8_t i;
    uint32_t Number1;
    if (Number >= 0) {
        OLED_ShowChar(x, y, '+', Font_Size);
        Number1 = Number;
    } else {
        OLED_ShowChar(x, y, '-', Font_Size);
        Number1 = -Number;
    }
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(x + 8 + (i * 8), y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0', Font_Size);
    }
}

/**
  * @brief  OLED显示数字（十六进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
  * @param  Length 要显示数字的长度，范围：1~8
  * @retval 无
  */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Font_Size) {
    uint8_t i, SingleNumber;
    for (i = 0; i < Length; i++) {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;
        if (SingleNumber < 10) {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0', Font_Size);
        } else {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A', Font_Size);
        }
    }
}

/**
  * @brief  OLED显示数字（二进制，正数）
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length, uint8_t Font_Size) {
    uint8_t i;
    for (i = 0; i < Length; i++) {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0', Font_Size);
    }
}

//void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[]) {
//    uint32_t i = 0;
//    for (uint8_t y = y0; y < y1; ++y) {
//        {
//            OLED_SetCursor(y, x0);
//            for (uint8_t x = x0; x < x1; ++x) {
//                OLED_WriteData(&OLED_Soft_I2C, BMP[i]);
//                i++;
//            }
//        }
//    }
//}
///**
// * OLED上显示图片, 图片的分辨率要满足 (x1 - x0) * (y1 -y0)
// * @param x0 图片起始x坐标
// * @param y0 图片起始x坐标
// * @param x1 图片终点x坐标
// * @param y1 图片终点x坐标
// * @param BMP 图片数组
// */
//void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[]) {
//    uint8_t BMP_GRAM[128][8] = {0};
//    /* 判断起始y坐标对应的页*/
//    for (int y = y0 / 8 + ((y0 % 8) ? 1 : 0); y < y1 / 8 + ((y1 % 8) ? 1 : 0); ++y) {
//        for (int x = x0; x < x1; ++x) {
//            for (int i = 0; i < 8; ++i) {
//                /* 取对应的位赋值给缓存数组 */
//                if (BMP[(x - x0) + (x1 - x0) * (y - (y0 / 8 + ((y0 % 8) ? 1 : 0)))] & (0x01 << i))
//                    BMP_GRAM[x][y + (i + 4) / 8] |= 0x01 << ((i + 4) % 8);
//                else
//                    BMP_GRAM[x][y + (i + 4) / 8] &= ~(0x01 << ((i + 4) % 8));
//            }
//        }
//    }
//
//    for (int y = y0 / 8 + ((y0 % 8) ? 1 : 0); y < y1 / 8 + ((y1 % 8) ? 1 : 0) + 1; ++y) {
//        OLED_SetCursor(y, x0);
//        for (int x = x0; x < x1; ++x) {
//            OLED_WriteData(&OLED_Soft_I2C, BMP_GRAM[x][y]);
//        }
//    }
//}


/**
 * OLED上显示图片, 图片的分辨率要满足 (x1 - x0) * (y1 -y0)
 * @param x0 图片起始x坐标
 * @param y0 图片起始x坐标
 * @param x1 图片终点x坐标
 * @param y1 图片终点x坐标
 * @param BMP 图片数组
 */
void OLED_ShowPicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[]) {
    /* 判断起始y坐标对应的页*/
    for (int y = y0 / 8 + ((y0 % 8) ? 1 : 0); y < y1 / 8 + ((y1 % 8) ? 1 : 0); ++y) {
        for (int x = x0; x < x1; ++x) {
            for (int i = 0; i < 8; ++i) {
                /* 取对应的位赋值给缓存数组 */
                if (BMP[(x - x0) + (x1 - x0) * (y - (y0 / 8 + ((y0 % 8) ? 1 : 0)))] & (0x01 << i))
                    OLED_GRAM[x][y + (i + 4) / 8] |= 0x01 << ((i + 4) % 8);
                else
                    OLED_GRAM[x][y + (i + 4) / 8] &= ~(0x01 << ((i + 4) % 8));
            }
        }
    }
}


void
OLED_MovePicture(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, const uint8_t BMP[], uint8_t NextX, uint8_t NextY) {

    for (int y = y0 / 8 + ((y0 % 8) ? 1 : 0); y < y1 / 8 + ((y1 % 8) ? 1 : 0) + 1; ++y) {
        OLED_SetCursor(y, x0);
        for (int x = x0; x < x1; ++x) {
            OLED_WriteData(&OLED_Soft_I2C, 0);
        }
    }
    OLED_ShowPicture(NextX, NextY, NextX + (x1 - x0), NextY + (y1 - y0), BMP);

}

/**
  * @brief  OLED初始化
  * @param  无
  * @retval 无
  */
void OLED_Init(void) {
    uint32_t i, j;

    for (i = 0; i < 1000; i++)            //上电延时
    {
        for (j = 0; j < 1000; j++);
    }

    OLED_I2C_Init();            //端口初始化

    OLED_WriteCommand(&OLED_Soft_I2C, 0xAE);    //关闭显示
    OLED_WriteCommand(&OLED_Soft_I2C, 0xD5);    //设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(&OLED_Soft_I2C, 0x80);

    OLED_WriteCommand(&OLED_Soft_I2C, 0xA8);    //设置多路复用率
    OLED_WriteCommand(&OLED_Soft_I2C, 0x3F);

    OLED_WriteCommand(&OLED_Soft_I2C, 0xD3);    //设置显示偏移
    OLED_WriteCommand(&OLED_Soft_I2C, 0x00);

    OLED_WriteCommand(&OLED_Soft_I2C, 0x40);    //设置显示开始行

    OLED_WriteCommand(&OLED_Soft_I2C, 0xA1);    //设置左右方向，0xA1正常 0xA0左右反置

    OLED_WriteCommand(&OLED_Soft_I2C, 0xC8);    //设置上下方向，0xC8正常 0xC0上下反置

    OLED_WriteCommand(&OLED_Soft_I2C, 0xDA);    //设置COM引脚硬件配置
    OLED_WriteCommand(&OLED_Soft_I2C, 0x12);

    OLED_WriteCommand(&OLED_Soft_I2C, 0x81);    //设置对比度控制
    OLED_WriteCommand(&OLED_Soft_I2C, 0xFF);

    OLED_WriteCommand(&OLED_Soft_I2C, 0xD9);    //设置预充电周期
    OLED_WriteCommand(&OLED_Soft_I2C, 0xF1);

    OLED_WriteCommand(&OLED_Soft_I2C, 0xDB);    //设置VCOMH取消选择级别
    OLED_WriteCommand(&OLED_Soft_I2C, 0x30);

    OLED_WriteCommand(&OLED_Soft_I2C, 0xA4);    //设置整个显示打开/关闭

    OLED_WriteCommand(&OLED_Soft_I2C, 0xA6);    //设置正常/倒转显示

    OLED_WriteCommand(&OLED_Soft_I2C, 0x8D);    //设置充电泵
    OLED_WriteCommand(&OLED_Soft_I2C, 0x14);

    OLED_WriteCommand(&OLED_Soft_I2C, 0xAF);    //开启显示

    OLED_Clear();                //OLED清屏
}

void OLED_DisPlay_On(void) {
    OLED_WriteCommand(&OLED_Soft_I2C, 0X8D);   //电荷泵使能
    OLED_WriteCommand(&OLED_Soft_I2C, 0X8D);   //开启电荷泵
    OLED_WriteCommand(&OLED_Soft_I2C, 0XAF);   //点亮屏幕
}


void OLED_DisPlay_Off(void) {
    OLED_WriteCommand(&OLED_Soft_I2C, 0X8D);
    OLED_WriteCommand(&OLED_Soft_I2C, 0X10);
    OLED_WriteCommand(&OLED_Soft_I2C, 0XAF);
}


void tian(uint8_t x, uint8_t y) {
    for (int i = x; i < 64; i += 2) {
        for (int j = y; j < 128; j += 2) {
            OLED_DrawPoint(j, i);
        }
    }
}

void PWM_DisPlay(void) {
    static uint8_t Value[128] = {50};
    for (int i = 127; i > 0; --i) {
        Value[i] = Value[i - 1];
    }
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == RESET) {
        Value[0] = 10;
    } else {
        Value[0] = 50;
    }
    OLED_ClearBuff();
    for (int i = 0; i < 127; ++i) {
        //OLED_DrawPoint(i, 63 - Value[i]);
        OLED_DrawLine(i, 63 - Value[i], i + 1, 63 - Value[i + 1]);
    }
    OLED_Refresh();
    HAL_Delay(20);
}