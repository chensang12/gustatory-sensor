#ifndef __OLED_MEMU_H
#define __OLED_MEMU_H


#include "adc.h"
#include "main.h"
#include "OLED.h"
#include "SC7A20.h"
#include "DS18B20.h"
#include "stm32f1xx_hal.h"
#include "rtc.h"

#define ID_MAX  100
#define MenuItem_SetID(MenuItem, ID)    (MenuItem->id = ID)
#define MenuItem_SetFun(MenuItem, Fun)    (MenuItem->action = Fun)
#define MenuItem_SetParent(MenuItem, Parent) (MenuItem->parent = Parent )
#define MenuItem_SetSubmenu(MenuItem, Submenu) (MenuItem->submenu = Submenu)
#define MenuItem_pxTOP(MenuItem)    (MenuItem = MenuItem_Top(MenuItem, TOP) )
#define MenuItem_pxTAIL(MenuItem)    (MenuItem = MenuItem_Top(MenuItem, TAIL) )
/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct MenuItem {
    uint8_t id; // 菜单项标识符
    char *text; // 菜单项显示文本
    struct MenuItem *submenu; // 指向子菜单的指针
    struct MenuItem *parent; // 指向上一级菜单的指针
    struct MenuItem *next; // 指向下一个同级菜单项的指针
    struct MenuItem *last; // 指向上一个同级菜单项的指针
    void (*action)(void);
} MenuItem;


typedef struct {
    uint8_t Container; // SHULIANG
    MenuItem *pxIndex; //链表节点索引指针
    MenuItem *xListEnd;//链表最后一个节点

} MenuTopItem;


typedef struct {
    uint8_t x0;
    uint8_t y0;
    char *DataBuf0;

    uint8_t x1;
    uint8_t y1;
    char *DataBuf1;

    uint8_t x2;
    uint8_t y2;
    char *DataBuf2;

    uint8_t x3;
    uint8_t y3;
    char *DataBuf3;

} OLED_Mess;

typedef enum {
    TOP = 1,
    TAIL = 2
} Mode;
//
//typedef enum {
//    mainMenu = 0,
//    firstSubMenu = 1,
//    secondSubMenu = 2
//} OLED_MENU;


//extern OLED_Mess OLED_RDraw;
extern char Test1[16];
extern char Test2[16];
extern char Test3[16];
extern char Test4[16];

extern MenuItem Main;
extern MenuItem First;
extern MenuItem Second;


void OLED_MenuItem_Init(void);

void OLED_Mess_Set(OLED_Mess *OLED_Data, uint8_t x, uint8_t y, char *Char);

const MenuItem *MenuItem_Top(MenuItem const *ListItem, Mode Value);

/* USER CODE END ET */



void Main_Test(void);


void DS18B20_Pic(void);

void ACC_Pic(void);

void Hum_Pic(void);

void LDR_Pic(void);

void M5(void);

void ACC_GetData(void);

void LDR_GetData(void);

void M8(void);

void DHTC_GetData(void);

void DS18B20_GetData(void);

#endif