#include "My_Flash.h"

/**
 * 内部FLASH删除一页
 * @param PageAdd :页起始地址
 */
void My_Flash_ClearPage(uint32_t PageAdd) {

    uint32_t PageError;
    FLASH_EraseInitTypeDef pEraseInit;

    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.NbPages = 1;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = PageAdd;

    HAL_FLASH_Unlock();
    if (HAL_FLASHEx_Erase(&pEraseInit, &PageError) != HAL_OK) {
        printf("擦除错误 PageError = %lX \n", PageError);
    }
    HAL_FLASH_Lock();

}

/**
 * 写入内部FLASH(1Byte)数组
 * @param Data 要写入的数据
 * @param PageAddress 页起始基地址
 * @param DataAddress 数据偏移地址(uint8_t *)
 */

void My_Flash_WriteByte(uint8_t Data, uint32_t PageAddress, uint32_t DataAddress) {
    uint16_t PageData[512] = {0};

    uint32_t P_Page = PageAddress;
    /* 读取一页FLASH占存在缓存数组*/
    for (int i = 0; i < 512; ++i) {
        PageData[i] = *((__IO uint16_t *) P_Page);
        P_Page += 2;
    }

    /* 擦除一页 */
    My_Flash_ClearPage(PageAddress);
    /* 在缓存里面改变数据再写入*/
    if (DataAddress % 2 == 0) {
        PageData[DataAddress / 2] &= (uint16_t) Data;
    } else {
        PageData[DataAddress / 2] |= ((uint16_t) (Data << 8));
    }

    P_Page = PageAddress;
    HAL_FLASH_Unlock();
    for (int i = 0; i < 512; ++i) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, P_Page, PageData[i]) != HAL_OK) {
            printf("FLASH 地址写入错误, 地址为 %lX\n", (PageAddress + (i * 2)));
        }
        P_Page += 2;
    }

    HAL_FLASH_Lock();
}

/**
 * 写入内部FLASH(Byte)数组
 * @param Data 数组指针
 * @param Length 数组长度
 * @param PageAddress 页基地址
 * @param DataAddress 数据保存的偏移地址
 */

void My_Flash_WriteByteArray(const uint8_t *Data, uint32_t Length, uint32_t PageAddress, uint32_t DataAddress) {
    uint8_t DataBuf[1024] = {0};
    for (int i = 0; i < 1024; ++i) {
        DataBuf[i] = My_Flash_ReadByte(PageAddress + i);
    }

    My_Flash_ClearPage(PageAddress);

    for (uint32_t i = DataAddress; i < DataAddress + Length; ++i) {
        DataBuf[i] = Data[i - DataAddress];
    }

    HAL_FLASH_Unlock();
    for (int i = 0; i < 1024; i += 2) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, (PageAddress + i),
                              (uint16_t) (DataBuf[i + 1] << 8 | DataBuf[i])) != HAL_OK) {
            printf("FLASH 地址写入错误, 地址为 %lX\n", (PageAddress + i / 2));

        }
    }
    HAL_FLASH_Lock();

}

/**
 * 读取内部FLASH数据
 * @param DataAddress FLASH地址
 * @return FLASH地址的变量
 */
uint8_t My_Flash_ReadByte(uint32_t DataAddress) {
    return (uint8_t) (*(__IO uint8_t *) DataAddress);
}

/**
 * 读取指定长度数据(Byte)到数组里面
 * @param Arr 数组接收缓冲区地址
 * @param Length 数组长度
 * @param DataAddress 数据起始地址
 */
void My_Flash_ReadByteArray(uint8_t *Arr, uint32_t Length, uint32_t DataAddress) {

    for (int i = 0; i < Length; ++i) {
        *Arr = My_Flash_ReadByte(DataAddress + i);
        *Arr++;

    }
}

/**
 * 写入内部FLASH
 * @param Data 要写入的数据
 * @param PageAddress FLASH页起始地址
 * @param DataAddress 数据的偏移地址 (偏移量为 uint16_t)
 */
void My_Flash_WriteShort(uint16_t Data, uint32_t PageAddress, uint32_t DataAddress) {

    uint16_t PageData[512] = {0};

    uint32_t P_Page = PageAddress;
    /* 读取一页FLASH占存在缓存数组*/
    for (int i = 0; i < 512; ++i) {
        PageData[i] = *((__IO uint16_t *) P_Page);
        P_Page += 2;
    }
    /* 擦除一页 */
    My_Flash_ClearPage(PageAddress);
    /* 在缓存里面改变数据再写入*/

    //PageData[(DataAddress > 0) ? (DataAddress - 1) : DataAddress] = Data;
    PageData[DataAddress] = Data;

    P_Page = PageAddress;
    HAL_FLASH_Unlock();
    for (int i = 0; i < 512; ++i) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, P_Page, PageData[i]) != HAL_OK) {
            printf("FLASH 地址写入错误, 地址为 %lX\n", (PageAddress + (i * 2)));
        }
        P_Page += 2;
    }
    HAL_FLASH_Lock();
}

/**
 * FLASH读取2Byte数据
 * @param DataAddress 数据地址
 * @return FLASH地址下保存的数据
 */
uint16_t My_Flash_ReadData(uint32_t DataAddress) {
    return *((__IO uint16_t *) DataAddress);
}


/**
 * 写入一个数组(16位)到内部FLASH
 * @param PageAddress 起始页地址
 * @param Array 数组指针
 * @param Length 数组长度
 * @param BaseAddress 偏移地址
 */
void My_FLASH_Write_Array(uint32_t PageAddress, const uint16_t *Array, uint16_t Length, uint32_t BaseAddress) {

    uint16_t PageData[512] = {0};

    uint32_t P_Page = PageAddress;
    /* 读取一页FLASH占存在缓存数组*/
    for (int i = 0; i < 512; ++i) {
        PageData[i] = *((__IO uint16_t *) P_Page);
        P_Page += 2;
    }

    My_Flash_ClearPage(PageAddress);


    for (uint32_t i = BaseAddress; i < (BaseAddress + Length); ++i) {
        //PageData[i] = Array[i];
        PageData[i] = Array[BaseAddress - i];
    }
    HAL_FLASH_Unlock();

    for (int i = 0; i < 512; ++i) {
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, PageAddress, PageData[i]);
        PageAddress += 2;
    }
    HAL_FLASH_Lock();
}


