#ifndef LOCK_H
#define LOCK_H


#include "main.h"

#define LOCK_STRING_SIZE    5
#define LOCK_INIT_FLAG      0XAABB


/*63K地址*/
#define LOCK_FLAG_ADDRESS   0X800F800


/* 60k 地址*/
#define USER_LOCK_DATA_ADDRESS  0X800F000

typedef enum {
    BUSY,
    IDLE,
    LOCK,
    ADMIN,
    UNLOCK,
    UNLOCK1,
    LOCK_ERROR,
    LOCK_RESET,


} LockStatus;


typedef struct {

    uint8_t ErrFlag;
    LockStatus Status;
    uint8_t Open_Password[8];
    uint8_t Admin_Password[8];

} Lock;


extern const uint8_t *LOCK_STRING[LOCK_STRING_SIZE];

void OLED_ShowLockStatus(volatile Lock *Lock);

void User_Lock_Save_Data(Lock *User_Lock);


#endif