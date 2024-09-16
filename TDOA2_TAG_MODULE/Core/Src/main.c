#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "uwb.h"

extern UART_HandleTypeDef huart2;

void SystemClock_Config(void);

int main(void)
{
  HAL_Init();

  SystemClock_Config();
  MX_USART2_UART_Init();
  dwStart();

      while (1)
  {
    dwLoop();
  }
}

  void SystemClock_Config(void)
  {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
      Error_Handler();
    }
  }


  /* Function required to use "printf" to print on serial console */
  int _write (int fd, const void *buf, size_t count)
  {
    // stdout
    if (fd == 1) {
      HAL_UART_Transmit(&huart2, (uint8_t *)buf, count, HAL_MAX_DELAY);
    }

    // stderr
    if (fd == 2) {
      HAL_UART_Transmit(&huart2, (uint8_t *)buf, count, HAL_MAX_DELAY);
    }

    return count;
  } 
  

  /**
   * @brief  Period elapsed callback in non blocking mode
   * @note   This function is called  when TIM1 interrupt took place, inside
   * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
   * a global variable "uwTick" used as application time base.
   * @param  htim : TIM handle
   * @retval None
   */
  void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
  {
    if (htim->Instance == TIM1)
    {
      HAL_IncTick();
    }
  }

  /**
   * @brief  This function is executed in case of error occurrence.
   * @retval None
   */
  void Error_Handler(void)
  {
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */
