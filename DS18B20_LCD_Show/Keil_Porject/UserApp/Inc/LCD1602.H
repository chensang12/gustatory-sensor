#ifndef __LCD_1602_H
#define __LCD_1602_H


unsigned int LCD_Pow(unsigned char x,unsigned char y);
void LCD1602_Init(void);
void LCD_ShowChar(unsigned char Line, unsigned char Column,char Char);
void LCD_ShowString(unsigned char Line, unsigned char Column, unsigned char *String);
void LCD_ShowNum(unsigned char Line, unsigned char Column, unsigned int Nunber, unsigned char Length);
void LCD_ShowSignedNum(unsigned char Line, unsigned char Column, int Nunber, unsigned char Length);


#endif


