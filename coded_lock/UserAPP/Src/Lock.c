#include "Lock.h"

/* Lock信息 */
const uint8_t Pass[] = "Pass:";
const uint8_t Admin[] = "Admin:";
const uint8_t Reset[] = "Reset";
const uint8_t Modify[] = "Modify:";
const uint8_t GetStatus[] = "GetStatus";

const uint8_t *LOCK_STRING[LOCK_STRING_SIZE] = {Pass, Admin, Reset, Modify, GetStatus};


void OLED_ShowLockStatus(volatile Lock *Lock) {
    switch (Lock->Status) {
        case LOCK:
            OLED_ShowString(4, 1, "LOCK  ");
            break;
        case ADMIN:
            OLED_ShowString(4, 1, "ADMIN ");
            break;
        case UNLOCK:
            OLED_ShowString(4, 1, "UNLOCK");
            break;
        case UNLOCK1:
            OLED_ShowString(4, 1, "UNLOCK1");
            break;
        case LOCK_ERROR:
            OLED_ShowString(4, 1, "ERROR  ");
            break;
        case LOCK_RESET:
            OLED_ShowString(4, 1, "RESET ");
            break;
        case BUSY:
            OLED_ShowString(4, 1, "BUSY  ");
            break;
        case IDLE:
            OLED_ShowString(4, 1, "IDLE  ");
            break;
    }
}


void User_Lock_Save_Data(Lock *User_Lock) {
    //My_FLASH_Write_Array(USER_LOCK_DATA_ADDRESS, User_Lock->Open_Password, 8, 0);
    //My_FLASH_Write_Array(USER_LOCK_DATA_ADDRESS, User_Lock->Admin_Password, 8, 10);
}


