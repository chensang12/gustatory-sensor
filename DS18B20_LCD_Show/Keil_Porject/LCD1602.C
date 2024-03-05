#include <REGX52.H>


#define LCD_Data  	P0

sbit LCD_RS = P2^6;   //数据/指令选择，1为数据，0为指令
sbit LCD_RW = P2^5;   //读/写选择，1为读，0为写
sbit LCD_EN = P2^7;   //使能，1为数据有效，下降沿执行命令



/**
 * @brief 12MHZ,ms延时函数
 * @param 1ms延时的时间
 */
void LCD_Delay()
{
	unsigned char i, j;

	i = 2;
	j = 239;
	do
	{
		while (--j);
	} while (--i);
}

/**
 * @brief LCD1602写命令
 * @param Commad 需要写入的命令
 */
void LCD_WriteCommad(unsigned char Commad)
{
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_Data = Commad;
	LCD_EN = 1;
	LCD_Delay();
	LCD_EN = 0;
	LCD_Delay();
}

/**
 * @brief LCD1602写数据
 * @param Data 需要写入的数据
 */
void LCD_WriteData(unsigned char Data)
{
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_Data = Data;
	LCD_EN = 1;
	LCD_Delay();
	LCD_EN = 0;
	LCD_Delay();
}


/**
 * @brief LCD1602初始化
 * @param  无
 */
void LCD1602_Init(void)
{
	LCD_WriteCommad(0X38);			//显示设置
	LCD_WriteCommad(0X0C);			//显示开关
	LCD_WriteCommad(0X06);			//光标设置
	LCD_WriteCommad(0X01);			//清屏
}


/**
 * @brief LCD1602光标位置设置
 * @param Line 行位置 范围:1 ~ 2
 * @param Column 列位置 范围:1 ~ 16
 */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line == 1)
	{
		LCD_WriteCommad(0X80 | (Column - 1));
	}
	else if(Line == 2)
	{
		LCD_WriteCommad(0x80 | (Column - 1 + 0x40));
	}
}


/**
 * @brief LCD1602显示一个字符
 * @param Line 显示的起始行位置
 * @param Column 显示的起始列位置
 * @param Char 需要显示的字符
 */
void LCD_ShowChar(unsigned char Line, unsigned char Column,char Char)
{
	LCD_SetCursor(Line,Column);
	LCD_WriteData(Char);
}

/**
 * @brief LCD1602上显示一个字符串
 * @param Line 显示的起始行位置
 * @param Column 显示的起始列位置
 * @param String 需要显示的字符串
 */
void LCD_ShowString(unsigned char Line, unsigned char Column, unsigned char *String)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i = 0; String[i] != '\0';i++)
	{
		LCD_WriteData(String[i]);
	}
}

/**
 * @brief x的y次方函数
 * @param x
 * @param y
 * @return
 */
unsigned int LCD_Pow(unsigned char x,unsigned char y)
{
	unsigned char i;
	unsigned int Result = 1;
	for(i = 0; i < y; i++)
	{
		Result = Result * x;
	}
	return Result;
}


/**
 * @brief LCD1602上显示数字(无符号十进制)
 * @param Line 显示的起始行地址
 * @param Column 显示起始的列地址
 * @param Nunber 需要显示的数字
 * @param Length 数字的长度
 */
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Nunber, unsigned char Length)
{
	unsigned char i;
	LCD_SetCursor(Line,Column);
	for(i = 0; i < Length; i++)
	{
		LCD_WriteData((Nunber / LCD_Pow(10,Length - 1 - i) % 10) +'0');
	}
}



/**
 * @brief LCD1602上显示数字(带符号十进制)
 * @param Line 显示的起始行地址
 * @param Column 显示起始的列地址
 * @param Nunber 需要显示的数字
 * @param Length 数字的长度
 */
void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, int Nunber, unsigned char Length)
{
	unsigned char i;
	int Number1;
	LCD_SetCursor(Line,Column);
	if(Nunber >= 0)
	{
		LCD_WriteData('+');
		Number1 = Nunber;
	}
	else
	{
		LCD_WriteData('-');
		Number1 = -Nunber;
	}
	for(i = 0; i < Length; i++)
	{
		LCD_WriteData(Number1 / LCD_Pow(10,Length - 1 - i) % 10 + '0');
	}
}




















