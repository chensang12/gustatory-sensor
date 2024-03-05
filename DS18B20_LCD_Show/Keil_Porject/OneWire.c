/*
 * @Author: Chen 2603148225@qq.com
 * @Date: 2023-03-16 20:20:56
 * @LastEditors: Chen 2603148225@qq.com
 * @LastEditTime: 2023-03-17 12:49:15
 * @FilePath: \DS18B20温度测量\OneWire.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <REGX52.H>

sbit DQ = P3^7;

/**
 * @brief 单总线通信初始化
 * @param  无
 * @return AckBit:从机应答信号 0为应答,1为非应答
 */
unsigned char OneWire_Init(void)
{
	unsigned char i;
	unsigned char AckBit;
	DQ = 1;
	DQ = 0;
	i = 240;while (--i);    //Delay480us
	DQ = 1;
	i = 30;while (--i);		//Delay75us
	AckBit = DQ;
	i = 250;while (--i);	//Delay500us
	DQ = 1;
	return AckBit;
}


/**
 * @brief 单总线发送1Bit数据
 * @param Bit :需要发送的Bit数据
 */
void OneWire_WriteBit(unsigned char Bit)
{
	unsigned char i;
	DQ = 0;
	i = 5;while (--i); 		//Delay11us
	DQ = Bit;
	i = 29;while (--i);		//Delay60us
	DQ = 1;
}

/**
 * @brief 单总线读取一位数据
 * @param  无
 * @return 读取的数据
 */
unsigned char OneWire_ReadBit(void)
{
	unsigned char i;
	unsigned char Bit;
	DQ = 0;
	i = 2;while (--i);	   //Delay5us
	DQ = 1;
	i = 2;while (--i);	   //Delay5us
	Bit = DQ;
	i = 27;while (--i);	   //Delay60us
	return Bit;

}

/**
 * @brief 单总线写一字节数据
 * @param Byte :需要写入的数据
 */
void OneWire_WriteByte(unsigned char Byte)
{
	unsigned char i;
	for(i = 0; i < 8; i++)
	{
		OneWire_WriteBit(Byte & (0X01 << i));
	}
}


/**
 * @brief 单总线读取一字节数据
 * @param  无
 * @return 读取的数据
 */
unsigned char OneWire_ReadByte(void)
{
	unsigned char i;
	unsigned char Byte = 0X00;
	for(i = 0; i < 8; i++)
	{
		if(OneWire_ReadBit()) {Byte |= (0X01 << i);}
	}
	return Byte;
}






