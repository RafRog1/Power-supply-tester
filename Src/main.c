/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "HD44780.h"
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
uint32_t adc[4];
double onePointADC = 3.3/4095;
uint8_t enableExti = 1;
uint8_t choseProgram = 0;
uint8_t aceptProgram = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(enableExti == 1){
		enableExti = 0;
		if(GPIO_Pin == P1_Pin){
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			if (aceptProgram == 1){
				aceptProgram = 0;
			}
			else{
				if (choseProgram > 0)
				choseProgram--;
			}

		}
		else if(GPIO_Pin == P3_Pin){
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			if(aceptProgram != 1)
				choseProgram++;
		}
		else if(GPIO_Pin == P2_Pin){
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			if(choseProgram != 0)
				aceptProgram = 1;
		}
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	static uint16_t a = 0;
	if(htim->Instance == TIM7 || enableExti == 0){
		a++;
		if(a == 750){
			a = 0;
			enableExti = 1;
		}
	}
}

static void LCD_double(uint32_t a){
	double x = (3.3 * a)/4095;
	uint8_t p;
	if(x > 1){
		p = x;
		LCD_WriteData(p + '0');
	}

	p = x * 10;
	if (p%10 > 0){
		LCD_WriteData(',');
		p = p%10;
		LCD_WriteData(p + '0');
	}
	else{
		LCD_WriteData(',');
		LCD_WriteData('0');
	}

	p = x * 100;
	if (p%10 > 0){
		p = p%10;
		LCD_WriteData(p + '0');
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
	uint8_t prevProgram = 0xFF;
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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  MX_DAC1_Init();
  MX_TIM7_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  LCD_Initalize();
  LCD_GoTo(0, 0);
  HAL_ADC_Start_DMA(&hadc1, adc, 4);
  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(choseProgram == 0){
		  if (prevProgram != choseProgram){
			  LCD_Clear();
			  LCD_GoTo(0, 0);
			  LCD_WriteText("Wybierz program");
			  LCD_GoTo(1, 0);
			  LCD_WriteText("<--  !OK!  -->");
			  prevProgram = choseProgram;
		  }
	  }
	  else if(choseProgram == 1){
		  if (prevProgram != choseProgram){
			  LCD_Clear();
			  LCD_GoTo(0, 0);
			  LCD_WriteText("Pomiar ADC");
			  LCD_GoTo(1, 0);
			  LCD_WriteText("<--   OK   -->");
			  prevProgram = choseProgram;
		  }
		  if(aceptProgram){
			  prevProgram = 0xFF;
			  LCD_Clear();
			  LCD_GoTo(0, 0);
			  LCD_WriteText("A1:");
			  LCD_GoTo(0, 8);
			  LCD_WriteText("A2:");
			  LCD_GoTo(1, 0);
			  LCD_WriteText("A3:");
			  LCD_GoTo(1, 8);
			  LCD_WriteText("A4:");
			  while(aceptProgram){
				  LCD_GoTo(0, 3);
				  LCD_double(adc[0]);
				  LCD_GoTo(0, 11);
				  LCD_double(adc[1]);
				  LCD_GoTo(1, 3);
				  LCD_double(adc[2]);
				  LCD_GoTo(1, 11);
				  LCD_double(adc[3]);
				  HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
				  HAL_Delay(100);
			  }
		  }
	  }
	  else if(choseProgram == 2){
		  if (prevProgram != choseProgram){
			  LCD_Clear();
			  LCD_GoTo(0, 0);
			  LCD_WriteText("Generowanie PWM");
			  LCD_GoTo(1, 0);
			  LCD_WriteText("<--   OK   -->");
			  prevProgram = choseProgram;
		  }
	  }
	  else if(choseProgram == 3){
		  if (prevProgram != choseProgram){
			  LCD_Clear();
			  LCD_GoTo(0, 0);
			  LCD_WriteText("Test wy. przek.");
			  LCD_GoTo(1, 0);
			  LCD_WriteText("<--   OK   -->");
			  prevProgram = choseProgram;
		  }
		  if(aceptProgram){
			  prevProgram = 0xFF;
			  LCD_Clear();
			  LCD_GoTo(0, 0);
			  LCD_WriteText("P1: 0");
			  LCD_GoTo(0, 6);
			  LCD_WriteText("P2: 0");
			  LCD_GoTo(1, 0);
			  LCD_WriteText("P3: 0");
			  LCD_GoTo(1, 6);
			  LCD_WriteText("P4: 0");
			  while(aceptProgram){
				  LCD_GoTo(0, 4);
				  LCD_WriteText("1");
				  HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_SET);
				  HAL_Delay(1000);
				  LCD_GoTo(0, 4);
				  LCD_WriteText("0");
				  HAL_GPIO_WritePin(switch1_GPIO_Port, switch1_Pin, GPIO_PIN_RESET);
				  LCD_GoTo(0, 10);
				  LCD_WriteText("1");
				  HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_SET);
				  HAL_Delay(1000);
				  LCD_GoTo(0, 10);
				  LCD_WriteText("0");
				  HAL_GPIO_WritePin(switch2_GPIO_Port, switch2_Pin, GPIO_PIN_RESET);
				  LCD_GoTo(1, 4);
				  LCD_WriteText("1");
				  HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_SET);
				  HAL_Delay(1000);
				  LCD_GoTo(1, 4);
				  LCD_WriteText("0");
				  HAL_GPIO_WritePin(switch3_GPIO_Port, switch3_Pin, GPIO_PIN_RESET);
				  LCD_GoTo(1, 10);
				  LCD_WriteText("1");
				  HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_SET);
				  HAL_Delay(1000);
				  LCD_GoTo(1, 10);
				  LCD_WriteText("0");
				  HAL_GPIO_WritePin(switch4_GPIO_Port, switch4_Pin, GPIO_PIN_RESET);
				  HAL_Delay(500);
			  }
		  }
	  }
	  HAL_Delay(1);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /**Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_I2C1
                              |RCC_PERIPHCLK_ADC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
  PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 16;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_ADC1CLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
  /**Configure the main internal regulator output voltage 
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /**Enable MSI Auto calibration 
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
