#ifndef GUASSIAN_H
#define GUASSIAN_H


#include "stm32f1xx_hal.h"

/**
 * a为曲线尖锋的高度，b为尖峰的中心位置，c表征钟的宽度
 */
typedef struct {

    float o2;    //方差
    float o;     //标准差
    float u;     //期望

    float a;
    float b;
    float c;

} Guassian;


void Guassian_Func(uint16_t *DataBuff, uint16_t DataSize, Guassian *guassian);

#endif
