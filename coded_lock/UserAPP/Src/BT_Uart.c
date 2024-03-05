
#include "BT_Uart.h"


/*一帧数据包拆解*/
uint8_t UsartData_Analysis(uint8_t *Usart_DataBuff, uint8_t *DataBuff, uint8_t Length) {
    uint8_t Status = 0;

    if (Usart_DataBuff[0] == 0XFA) {
        Status = 1;
    }
    if (Status && (Usart_DataBuff[Length - 1] == 0XFE)) {
        Status = 2;
    } else {
        Status = 0;
    }
    for (uint8_t i = 1; i < Length - 1; i++) {
        DataBuff[i - 1] = Usart_DataBuff[i];
    }

    return Status;

}

uint8_t String_Compare1(const uint8_t *S1, const uint8_t *S2, uint8_t Length) {
    uint8_t i;
    for (i = 0; i < Length; ++i) {
        if (*S1 == *S2) {
            S1++;
            S2++;
        } else {
            break;
        }
    }
    return (i < Length) ? 1 : 0;
}

/**
 * 字符串比较函数(字符串格式为"sadasd\n"结尾必须为'\n'), 长度必须一致
 * @param S1 字符串1地址
 * @param S2 字符串1地址
 * @return
 */
uint8_t String_Compare(const uint8_t *S1, const uint8_t *S2) {
    uint8_t i = 0;
    uint8_t S1_Length = 0, S2_Length = 0;
    uint8_t *Ps = (uint8_t *) S1;
    while (*Ps != '\n') {
        Ps++;
        S1_Length++;
    }
    Ps = (uint8_t *) S2;
    while (*Ps != '\n') {
        Ps++;
        S2_Length++;
    }
    if (S1_Length == S2_Length) {
        for (i = 0; i < S1_Length; ++i) {
            if (*S1 == *S2) {
                S1++;
                S2++;
            } else {
                break;
            }
        }
        return (i < S1_Length) ? 1 : 0;
    }
    return 1;
}

//void Array_Shift(uint8_t *Arr, uint8_t Offset) {
//    uint8_t *p = Arr;
//    uint8_t Length = 0;
//    /* 计算数组格式 */
//    while (*p != '\n') {
//        *p++;
//        Length++;
//    }
//    uint8_t *TempBuff = (uint8_t *) malloc(sizeof(uint8_t) * Length);
//    TempBuff = (Arr + Offset);
//
//    for (int i = 0; i < Length; ++i) {
//
//    }
//
//}

