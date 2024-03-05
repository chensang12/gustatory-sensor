#include <REGX52.H>
#include "OneWire.H"

/*DS18B20 Coommad*/
#define	SKIP_ROM			0XCC	//跳过ROM
#define	CONVERT_T			0X44	//开始温度转换
#define	READ_SCRATCHPAD		0XBE	//读取寄存器的值



/**
 * @brief DS18B20初始化(单总线初始化)
 * @param
 */
void DS18B20_Init(void)
{
	OneWire_Init();
}

/**
 * @brief DS18B20开始温度转换
 * @param : 无
 */
void DS18B20_ConvertT(void)
{
	DS18B20_Init();
	OneWire_WriteByte(SKIP_ROM);
	OneWire_WriteByte(CONVERT_T);
}

/**
 * @brief 读取DS18B20获取的温度
 * @param  :无
 * @return :DS18B20的温度数据
 */
float DS18B20_ReadT(void)
{
	unsigned char TH,TL;
	int Temp;
	float T;
	DS18B20_Init();
	OneWire_WriteByte(SKIP_ROM);
	OneWire_WriteByte(READ_SCRATCHPAD);
	TL = OneWire_ReadByte();
	TH = OneWire_ReadByte();
	Temp = (TH << 8) | TL ;
	T = Temp / 16.0;
	return T;
}
