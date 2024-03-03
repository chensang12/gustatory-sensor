#include "Gaussian.h"
#include "math.h"

/**
 * 高斯函数的计算，用来描述一组数据的分布情况
 * @param DataBuff 采集到的数据
 * @param DataSize 数据长度
 * @param guassian 高斯参数结构体
 */
void Guassian_Func(uint16_t *DataBuff, uint16_t DataSize, Guassian *guassian) {
    uint16_t *pData = DataBuff;
    guassian->u = 0;
    /* 期望计算 u = 1/n + */
    for (int i = 0; i < DataSize; ++i) {
        guassian->u += (float) (1.0 / DataSize) * (float) (*pData);
        pData++;
    }
    pData = DataBuff;
    /* 方差计算 */
    for (int i = 0; i < DataSize; ++i) {
        guassian->o2 += (float) (1.0 / DataSize) * (((float) (*pData) - guassian->u) *
                                                       ((float) (*pData) - guassian->u));

        pData++;
    }
    guassian->o = sqrtf(guassian->o2);
    guassian->a = 1 / (guassian->o * sqrtf((float) (2 * 3.14)));
    guassian->b = guassian->u;
    guassian->c = guassian->o;
}