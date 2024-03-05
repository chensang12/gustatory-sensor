#include "My_Uart.h"

#if HAL_StatusTypeDef_Enable
HAL_StatusTypeDef Uart_Send_Char(char Char, uint32_t Timeout) {

    return HAL_UART_Transmit(&huart1, (uint8_t *) &Char, 1, Timeout);
}

HAL_StatusTypeDef Uart_Send_String(char *String, uint32_t Timeout) {
    HAL_StatusTypeDef Status;
    while (*String != '\0') {
        Status = Uart_Send_Char(*String, Timeout);
        String++;
    }
    return Status;
}


uint64_t Pow(uint32_t x, uint32_t y) {
    uint64_t i = 1;
    for (int j = 0; j < y; ++j) {
        i *= x;
    }
    return i;
}


HAL_StatusTypeDef Uart_Send_Num(uint64_t Number, uint8_t Length, uint32_t Timeout) {
    HAL_StatusTypeDef Status;
    uint8_t Num;
    for (int i = 0; i < Length; ++i) {
        Num = (Number / Pow(10, Length - i - 1)) % 10 + '0';
        Status = Uart_Send_Char(Num, Timeout / Length);
    }
    return Status;
}


HAL_StatusTypeDef Uart_Send_SignNum(int64_t Number, uint8_t Length, uint32_t Timeout) {
    HAL_StatusTypeDef Status;
    uint8_t Num;
    if (Number > 0) {
        Uart_Send_Char('+', 10);
    } else {
        Number = -Number;
        Uart_Send_Char('-', 10);
    }
    for (int i = 0; i < Length; ++i) {
        Num = (Number / Pow(10, Length - i - 1)) % 10 + '0';
        Status = Uart_Send_Char(Num, Timeout / Length);
    }
    return Status;
}

#else

void Uart_Send_Char(char Char) {

    HAL_UART_Transmit(&huart1, (uint8_t *) &Char, 1, 10);
}

void Uart_Send_String(char *String) {

    while (*String != '\0') {
        Uart_Send_Char(*String);
        String++;
    }

}


uint64_t Pow(uint32_t x, uint32_t y) {
    uint64_t i = 1;
    for (int j = 0; j < y; ++j) {
        i *= x;
    }
    return i;
}


void Uart_Send_Num(uint64_t Number, uint8_t Length) {
    HAL_StatusTypeDef Status;
    uint8_t Num;
    for (int i = 0; i < Length; ++i) {
        Num = (Number / Pow(10, Length - i - 1)) % 10 + '0';
        Uart_Send_Char(Num);
    }
}


void Uart_Send_SignNum(int64_t Number, uint8_t Length, uint32_t Timeout) {

    uint8_t Num;
    if (Number > 0) {
        Uart_Send_Char('+');
    } else {
        Number = -Number;
        Uart_Send_Char('-');
    }
    for (int i = 0; i < Length; ++i) {
        Num = (Number / Pow(10, Length - i - 1)) % 10 + '0';
        Uart_Send_Char(Num);
    }

}


#endif






