/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "fonts.h"
#include "pic.h"
#include "sdio_sdcard.h"
#include "w25qxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
SPIF_HandleTypeDef hSPIF;
FATFS fs;    // 文件系统对象
FIL file;    // 文件对象
FRESULT res; // 文件操作结果
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Size 2048
uint8_t WriteBuf[Size] = {"This write data ok"};
uint8_t ReadBuf[Size]  = {"This read data ok"};
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
volatile uint8_t keyVal      = 0;
volatile uint8_t key_pressed = 0;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void delay_us(uint32_t nus);
void delay_us(uint32_t udelay);
void w25qxx_test(void);
void show_sdcard_info(void);
void sd_test_write(uint32_t secaddr, uint32_t seccnt);
void sdFatfs_test(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 按键中断处理函数
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == KEY0_Pin) {
        // 防抖延时
        HAL_Delay(50);

        if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, GPIO_PIN_0) == GPIO_PIN_SET) {
            keyVal++;
            key_pressed = 1;
        }
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

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
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_SDIO_SD_Init();
  MX_FATFS_Init();
  MX_USART1_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  /* USER CODE BEGIN 2 */
    SPIF_Init(&hSPIF, &hspi1, SPI_CS_GPIO_Port, SPI_CS_Pin);
    SD_Driver.disk_initialize(0);
    ST7735_Init();
    ST7735_Backlight_On();
    ST7735_FillScreen(ST7735_WHITE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    while (sd_init()) {
        printf("check SD error\r\n");
    }

    show_sdcard_info();
    sdFatfs_test();

    while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
        ST7735_DrawString(2, 2, "hello lcd", Font_7x10, ST7735_RED, ST7735_WHITE);
    }

  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void delay_us(uint32_t us)
{
    uint32_t startTick  = SysTick->VAL;
    uint32_t delayTicks = (SystemCoreClock / 1000000) * us;
    while ((SysTick->VAL - startTick) < delayTicks) {
        // 空循环，等待SysTick递减
    }
}

void w25qxx_test(void)
{
    if (SPIF_Init(&hSPIF, &hspi1, SPI_CS_GPIO_Port, SPI_CS_Pin) == 1) {
        printf("w25q128 init success\r\n");
    } else {
        printf("w25q128 init error\r\n");
    }

    // 定义测试数据
    uint8_t writeData[256] = {0}; // 写入数据的缓冲区
    uint8_t readData[256]  = {0}; // 读取数据的缓冲区

    for (int i = 0; i < sizeof(writeData); ++i) {
        writeData[i] = i; // 用一些测试数据填充写入缓冲区
    }

    // 擦除 Flash 部分
    if (!SPIF_EraseSector(&hSPIF, 0)) {
        // 擦除失败
        printf("擦除 Flash失败\r\n  ");
    }

    // 写入数据Flash
    if (!SPIF_WriteAddress(&hSPIF, 0, writeData, sizeof(writeData))) {
        // 写入失败
        printf("写入 Flash失败\r\n  ");
    }

    //  Flash 读取数据
    if (!SPIF_ReadAddress(&hSPIF, 0, readData, sizeof(readData))) {
        // 读取失败
        printf("读取 Flash失败\r\n  ");
    }

    for (int i = 0; i < sizeof(readData); ++i) {
        printf("data:%d ", readData[i]);
    }

    // 验证数据
    for (int i = 0; i < sizeof(writeData); ++i) {
        if (readData[i] != writeData[i]) {

            printf("数据不一致\r\n  ");
        }
    }
}

void show_sdcard_info(void)
{
    HAL_SD_CardCIDTypeDef sd_card_cid;

    HAL_SD_GetCardCID(&g_sdcard_handler, &sd_card_cid); /* 获取CID */
    get_sd_card_info(&g_sd_card_info_handle);           /* 获取SD卡信�? */

    switch (g_sd_card_info_handle.CardType) {
        case CARD_SDSC: {
            if (g_sd_card_info_handle.CardVersion == CARD_V1_X) {
                printf("Card Type:SDSC V1\r\n");
            } else if (g_sd_card_info_handle.CardVersion == CARD_V2_X) {
                printf("Card Type:SDSC V2\r\n");
            }
        } break;

        case CARD_SDHC_SDXC:
            printf("Card Type:SDHC\r\n");
            break;

        default:
            break;
    }

    printf("Card ManufacturerID:%d\r\n", sd_card_cid.ManufacturerID);                 /* 制造商ID */
    printf("Card RCA:%d\r\n", g_sd_card_info_handle.RelCardAdd);                      /* 卡相对地址 */
    printf("LogBlockNbr:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockNbr));     /* 显示逻辑块数 */
    printf("LogBlockSize:%d \r\n", (uint32_t)(g_sd_card_info_handle.LogBlockSize));   /* 显示逻辑块大*/
    printf("Card Capacity:%d MB\r\n", (uint32_t)SD_TOTAL_SIZE_MB(&g_sdcard_handler)); /* 显示容量 */
    printf("Card BlockSize:%d\r\n\r\n", g_sd_card_info_handle.BlockSize);             /* 显示块大小*/
}

void sdFatfs_test(void)
{
    FRESULT mount_res = f_mount(&fs, "0:", 1);
    if (mount_res != FR_OK) {
        printf("fatfs mount error: %d\n", mount_res);
    }
    res = f_open(&file, "0:/test.txt", FA_READ);
    if (res != FR_OK) {
        printf("fatfs open error: %d\n", res);
    }
    UINT br;
    char buffer[16];
    f_read(&file, buffer, sizeof(buffer), &br);
    printf("读取到的数据: %s\n", buffer);
    f_close(&file);
}

void sd_test_write(uint32_t secaddr, uint32_t seccnt)
{
    uint32_t i;
    uint8_t *buf;
    uint8_t sta = 0;

    buf = WriteBuf;

    for (i = 0; i < seccnt * 512; i++) {
        buf[i] = i * 3;
    }

    sta = sd_write_disk(buf, secaddr, seccnt);

    if (sta == 0) {
        printf("Write over!\r\n");
    } else {
        printf("err:%d\r\n", sta);
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
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
