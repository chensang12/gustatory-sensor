/*
 * @Author: error: git config user.name && git config user.email & please set dead value or install git
 * @Date: 2023-01-02 23:10:22
 * @LastEditors: error: git config user.name && git config user.email & please set dead value or install git
 * @LastEditTime: 2023-01-02 23:34:13
 * @FilePath: \AT24C02数据保存\I2C.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <REGX52.H>

//typedef unsigned char unsigned char;
//typedef unsigned int  unsigned int;

sbit I2C_SDA = P2^0;
sbit I2C_SCL = P2^1;

void I2C_Start(void)
{
     I2C_SDA = 1;
     I2C_SCL = 1;

     I2C_SDA = 0;
     I2C_SCL = 0;
}


void I2C_SendByte(unsigned char Byte)
{
     unsigned char i;
     for ( i = 0; i < 8; i++)
     {
          
          I2C_SDA = Byte & (0X80>>i);
          I2C_SCL = 1;
          I2C_SCL = 0;
     }
}


unsigned char I2C_ReceiveByte(void)
{
     unsigned char i,Byte=0x00;
     I2C_SDA = 1;
     for ( i = 0; i < 8; i++)
     {
          I2C_SCL = 1;
          if(I2C_SDA){Byte |= (0x80>>i);}
          I2C_SCL = 0;
     }
     return Byte;    
}

unsigned char I2C_Receive_ACKBit(void)
{
     unsigned char AckBit;
     I2C_SDA = 1;
     I2C_SCL = 1;
     AckBit = I2C_SDA;
     I2C_SCL = 0;
     return AckBit;
}

void I2C_Send_ACKBit(unsigned char AckBit)
{
     I2C_SDA = AckBit;
     I2C_SCL = 1;
     I2C_SCL = 0;
}

void I2C_Stop(void)
{
     I2C_SDA = 0;
     I2C_SCL = 1;
     
     I2C_SDA = 1;
}

