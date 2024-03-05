/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2023-01-02 23:10:29
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2023-01-03 22:47:32
 * @FilePath: \AT24C02数据保存\I2C.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __I2C_H
#define __I2C_H

typedef unsigned char uchar;
typedef unsigned int  uint;

void I2C_Start(void);
void I2C_SendByte(uchar Byte);
uchar I2C_ReceiveByte(void);
uchar I2C_Receive_ACKBit(void);
void I2C_Send_ACKBit(uchar AckBit);
void I2C_Stop(void);

#endif 