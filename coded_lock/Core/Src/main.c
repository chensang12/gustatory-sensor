/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t Uart_RxBuff[RX_BUFF_SIZE];
uint8_t Uart_RxBuff2[RX_BUFF_SIZE];

/* 锁结构体定义 */
volatile Lock MyLock = {0};
/* 锁是否第一次次使用标志位 */
volatile uint16_t Lock_Flag;
/* 按键输入密码缓冲区 */
volatile uint8_t Key_Value;
volatile uint8_t Key_In_Length = 0;
volatile uint8_t Key_Buff[RX_BUFF_SIZE] = {0};
volatile uint8_t KeyPassBuff[PASS_LENGTH] = {0};

/* 首次上电初始化任务初始化任务 */
const uint8_t PassBuff[PASS_LENGTH] = {'1', '2', '3', '4', '5', '6', '7', '8'};
const uint8_t AdminBuff[PASS_LENGTH] = {'1', '2', '3', '4', '5', '6', '7', '8'};

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */
    RetargetInit(&huart1);
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    /* USER CODE BEGIN 2 */
    OLED_Init();
    Key_Init();


    MyLock.ErrFlag = 0;
    MyLock.Status = LOCK;
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Uart_RxBuff, sizeof(Uart_RxBuff));
    /*读取Flash来判断是否第一次进入密码锁系统*/

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);

    Lock_Flag = My_Flash_ReadData(LOCK_FLAG_ADDRESS);

    if (Lock_Flag != LOCK_INIT_FLAG) {
        printf("第一次进入系统， 进行FLASH 初始初始化 \n");
        printf("........\n");
        OLED_ShowString(2, 1, "Init Time");

        /* 第一次使用该产品(全部按照默认参数来设置) */
        My_Flash_ClearPage(LOCK_FLAG_ADDRESS);
        My_Flash_ClearPage(USER_LOCK_DATA_ADDRESS);
        /*写入初始化标志位*/
        My_Flash_WriteShort(LOCK_INIT_FLAG, LOCK_FLAG_ADDRESS, 0);
        /* 写入初始化Lock信息 */
        My_Flash_WriteByteArray((uint8_t *) PassBuff, PASS_LENGTH, USER_LOCK_DATA_ADDRESS, 0);
        My_Flash_WriteByteArray((uint8_t *) AdminBuff, PASS_LENGTH, USER_LOCK_DATA_ADDRESS, 10);
    } else {

        printf("不是第一次初始化, 直接进入循环 \n");
    }
    /* 读取FLASH数据保存到保存到数据结构体里面 */
    My_Flash_ReadByteArray((uint8_t *) MyLock.Open_Password, PASS_LENGTH, USER_LOCK_DATA_ADDRESS);
    My_Flash_ReadByteArray((uint8_t *) MyLock.Admin_Password, PASS_LENGTH, USER_LOCK_DATA_ADDRESS + 10);


    HAL_TIM_Base_Start_IT(&htim4);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        /* 判断按键是否按下，然后再设定范围 */
        Key_Value = KeyScan();
        if (Key_Value != 0XFF) {
            if (Key_Value == 'E') {
                for (int i = 0; i < PASS_LENGTH; ++i) {
                    KeyPassBuff[i] += '0';
                }
                if (!String_Compare1((uint8_t *) KeyPassBuff, (uint8_t *) MyLock.Open_Password, PASS_LENGTH)) {
                    MyLock.Status = UNLOCK;

                } else {
                    Key_In_Length = 0;
                    memset((uint8_t *) KeyPassBuff, 0, 8);
                    MyLock.ErrFlag++;
                }

            } else if (Key_Value == 'X') {

                if (Key_In_Length == 0) {
                    Key_In_Length = 0;
                } else {
                    KeyPassBuff[(Key_In_Length > 7) ? 7 : Key_In_Length] = 0;
                    Key_In_Length--;
                }
            } else if (Key_Value == 'B') {
                Key_In_Length = 0;
                OLED_Clear();
                MyLock.Status = LOCK;
            } else if (Key_Value != ' ') {
                if (Key_In_Length >= 8) {
                    Key_In_Length = 0;
                } else {
                    KeyPassBuff[Key_In_Length] = Key_Value % 10;
                    Key_In_Length++;
                }
            }
        }
        /*  */
        if (MyLock.Status == LOCK_ERROR) {
            MyLock.ErrFlag = 0;
            HAL_UART_DMAStop(&huart1);

            Buzzer_On();
            for (int i = 0; i < 10; ++i) {
                printf(" 请 %d 秒后再试", 10 - i);
                Delay_ms(200);
            }
            Buzzer_Off();
            MyLock.Status = LOCK;
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Uart_RxBuff, sizeof(Uart_RxBuff));
        } else if (MyLock.Status == UNLOCK) {
            Servo_SetAngle(0);
        } else if (MyLock.Status == LOCK) {
            Servo_SetAngle(180);
        }
    }

    /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim4) {
        /* 每xx ms刷新MyLock的状态 */
        if (MyLock.Status == UNLOCK) {
            Key_In_Length = 0;
            MyLock.ErrFlag = 0;
            memset((uint8_t *) KeyPassBuff, 0, 8);
        } else if (MyLock.Status == LOCK) {
            //Servo_SetAngle(180);
        }
        for (int i = 0; i < 8; ++i) {
            if (i < Key_In_Length) {
                OLED_ShowChar(1, 1 + i, '*');
            } else {
                OLED_ShowChar(1, 1 + i, ' ');
            }
        }
        if (MyLock.ErrFlag >= 3)MyLock.Status = LOCK_ERROR;
        OLED_ShowLockStatus(&MyLock);
    }
}

/**
 * 使用DMA + 串口空闲中断回调函数(接收完成)
 * @param huart 串口句柄
 * @param Size 接收到的字节
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {

    /*数据接收预处处理 */

    /* 判断接收到的数据格式是否是解锁指令 Pass:xxxxxxxx*/
    if (!String_Compare1(Uart_RxBuff, LOCK_STRING[0], 5)) {
        for (int i = 0; i < Size; ++i) {
            Uart_RxBuff2[i] = Uart_RxBuff[i + 5];
        }
        if (!String_Compare1(Uart_RxBuff2, (uint8_t *) MyLock.Open_Password, 8) && (MyLock.Status == LOCK)) {
            MyLock.Status = UNLOCK;
            printf("解锁成功 \n");
        } else if (MyLock.Status != UNLOCK) {
            MyLock.ErrFlag++;

            printf(" 你还剩余 %d 次输入机机会, 超过错误要次数需要等30S \n", (3 - MyLock.ErrFlag));
        } else {
            printf(" 门已打开不请不要重复输入 \n");
        }
    } else if (!String_Compare1(Uart_RxBuff, LOCK_STRING[1], 6)) {
        if (MyLock.Status != UNLOCK) {
            printf(" 您需要先解锁才能进入管理员状态 \n");
        } else {
            for (int i = 0; i < PASS_LENGTH; ++i) {
                Uart_RxBuff2[i] = Uart_RxBuff[6 + i];
            }
            if (!String_Compare1(Uart_RxBuff2, (uint8_t *) MyLock.Admin_Password, 8)) {
                MyLock.Status = ADMIN;
                printf("您已进入管理员模式, 可使用指(Modify:XXXXXXXX) (Reset)来更新密码跟重置默认密码 \r\n");
            } else {
                printf("管理员密码输入错误 请稍后再试\n");
            }
        }
    } else if (MyLock.Status == ADMIN) {
        if ((!String_Compare1(Uart_RxBuff, LOCK_STRING[2], 5))) {
            My_Flash_ClearPage(LOCK_FLAG_ADDRESS);
            printf("手动断电重启完成复位 \n");
        } else if (((!String_Compare1(Uart_RxBuff, LOCK_STRING[3], 7)))) {
            for (int i = 0; i < PASS_LENGTH; ++i) {
                Uart_RxBuff2[i] = Uart_RxBuff[7 + i];
            }
            My_Flash_WriteByteArray(Uart_RxBuff2, PASS_LENGTH, USER_LOCK_DATA_ADDRESS, 0);
            printf("手动断电重启完成密码修改 \n");
        }
    } else {
        MyLock.Status = LOCK;
    }
    /* 清清除缓存区 */
    memset(Uart_RxBuff, 0, sizeof(Uart_RxBuff));
    memset(Uart_RxBuff2, 0, sizeof(Uart_RxBuff2));
    //OLED_ShowLockStatus(&MyLock);
    /* 重新启动DMA 转运数据 */
    HAL_UARTEx_ReceiveToIdle_DMA(&huart1, Uart_RxBuff, sizeof(Uart_RxBuff));
}
/**
 * 产生蜂鸣器驱动信号
 */
void Buzzer_On(void) {
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
}
/**
 * 关闭蜂鸣器输出信号
 */
void Buzzer_Off(void) {
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_4);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
