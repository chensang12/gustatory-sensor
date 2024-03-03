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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "Gaussian.h"
#include "KeyScan.h"
#include "OLED.h"


volatile uint8_t DMA_FLAG = 0;
volatile uint16_t CCD_DataBUff[CCD_LENGTH];
volatile uint8_t Uart_CCD_DataBUff[CCD_LENGTH * 2];

volatile CCD_STATUS CCD_Status = IDLE;
Guassian BaselineGaussian_Buff[10] = {0};
Guassian TestLiquidGaussian_Buff[10] = {0};

Guassian BaselineGaussian = {0};
Guassian TestLiquidGaussian = {0};
Guassian ErrGaussian = {0};

//volatile uint16_t CCD_DataBUff[CCD_LENGTH];
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
void CCD_GetData(Guassian *_Guassian);

void OLED_ShowCCD_Status(CCD_STATUS Status);

void Get_Err(Guassian *Err, Guassian *B, Guassian *T);
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

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM1_Init();
    MX_TIM2_Init();
    MX_TIM3_Init();
    MX_TIM4_Init();
    MX_ADC1_Init();
    MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    Key_Init();
    OLED_Init();


    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);//CLK

    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) CCD_DataBUff, CCD_LENGTH);
    HAL_TIM_OnePulse_Start(&htim1, TIM_CHANNEL_1);//ICG
    HAL_TIM_OnePulse_Start(&htim3, TIM_CHANNEL_1);//SH

    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

    CCD_Status = IDLE;
    /* 获取基准 */
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        switch (CCD_Status) {
            case IDLE:
                if (Get_Key2Status() == KEY_ON) {
                    CCD_Status = CLEAR_AND_SET_BASELINE;
                } else if (Get_Key1Status() == KEY_ON) {
                    CCD_Status = START_MEASUREMENT;
                }
                break;
            case CLEAR_AND_SET_BASELINE:
                CCD_GetData(&BaselineGaussian);
                while (DMA_FLAG == 1);
                if (Get_Key2Status() == KEY_ON) {
                    CCD_Status = IDLE;
                } else if (Get_Key1Status() == KEY_ON) {
                    CCD_Status = START_MEASUREMENT;
                }
                break;
            case START_MEASUREMENT:
                CCD_GetData(&TestLiquidGaussian);
                while (DMA_FLAG == 1);
                CCD_Status = OVER;
                break;
            case OVER:
                Get_Err(&ErrGaussian, &BaselineGaussian, &TestLiquidGaussian);

                OLED_ShowSignedNum(0, 16, (ErrGaussian.a) * 10000, 6, 16);
                OLED_ShowSignedNum(0, 32, ErrGaussian.b, 6, 16);
                OLED_ShowSignedNum(0, 48, ErrGaussian.c, 8, 16);
                OLED_ReDraw();
                while (1) {
                    if (Get_Key2Status() == KEY_ON) {
                        CCD_Status = IDLE;
                        OLED_ClearBuff();
                        OLED_Clear();
                        break;
                    }

                }
        }
        OLED_ShowCCD_Status(CCD_Status);
        OLED_ReDraw();
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
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

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
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    if (hadc == &hadc1) {
        DMA_FLAG = 0;
        HAL_DMA_Start(&hdma_memtomem_dma1_channel2, (uint32_t) CCD_DataBUff, (uint32_t) Uart_CCD_DataBUff,
                      CCD_LENGTH * 2);
        HAL_UART_Transmit_DMA(&huart1, (uint8_t *) Uart_CCD_DataBUff, CCD_LENGTH * 2);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    const uint8_t Data[] = "数据传输完成 \r\n";
    if (huart == &huart1) {
        HAL_UART_Transmit(&huart1, Data, sizeof(Data), 20);
    }
}

#if 0
void CCD_GetData(Guassian *_Guassian) {

    Guassian Guassian_Buff[10];
    for (int i = 0; i < 10; ++i) {

        __HAL_TIM_ENABLE(&htim1);
        __HAL_TIM_ENABLE(&htim3);
        HAL_ADC_Start_DMA(&hadc1, (uint32_t *) CCD_DataBUff, CCD_LENGTH);
        Guassian_Func((uint16_t *) CCD_DataBUff, CCD_LENGTH, (Guassian *) &Guassian_Buff[i]);
    }
    for (int i = 0; i < 10; ++i) {
        _Guassian->c += Guassian_Buff[i].c;
        _Guassian->u += Guassian_Buff[i].u;
        _Guassian->a += Guassian_Buff[i].a;
        _Guassian->o += Guassian_Buff[i].o;
        _Guassian->b += Guassian_Buff[i].b;
        _Guassian->o2 += Guassian_Buff[i].o2;

    }
    _Guassian->c /= 10;
    _Guassian->u /= 10;
    _Guassian->a /= 10;
    _Guassian->o /= 10;
    _Guassian->b /= 10;
    _Guassian->o2 /= 10;

}
#else

void OLED_ShowCCD_Status(CCD_STATUS Status) {
    switch (Status) {
        case IDLE:
            OLED_ShowString(0, 0, "IDLE ", 16);
            break;
        case CLEAR_AND_SET_BASELINE:
            OLED_ShowString(0, 0, "CLEAR", 16);
            break;
        case START_MEASUREMENT:
            OLED_ShowString(0, 0, "START", 16);
            break;
        case OVER:
            OLED_ShowString(0, 0, "OVER ", 16);
            break;
    }
}

/**
 * 开始产生CCD驱动信号
 * @param _Guassian
 */
void CCD_GetData(Guassian *_Guassian) {
    DMA_FLAG = 1;
    __HAL_TIM_ENABLE(&htim1);
    __HAL_TIM_ENABLE(&htim3);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t *) CCD_DataBUff, CCD_LENGTH);
    Guassian_Func((uint16_t *) CCD_DataBUff, CCD_LENGTH, _Guassian);
}

void Get_Err(Guassian *Err, Guassian *B, Guassian *T) {
    Err->u = (T->u) - (B->u);
    Err->o = (T->o) - (B->o);
    Err->a = (T->a) - (B->a);
    Err->b = (T->b) - (B->b);
    Err->c = (T->c) - (B->c);
    Err->o2 = (T->o2) - (B->o2);
}

#endif

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
