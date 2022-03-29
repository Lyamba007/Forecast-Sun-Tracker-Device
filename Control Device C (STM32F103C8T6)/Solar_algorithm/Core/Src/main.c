/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ws2812.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define EPSILON 0.26 // approximate value of sun declination daily change
#define LATITUDE 50  // latitude for Kharkiv 
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint16_t BUF_DMA [ARRAY_LEN]; //Buffer for DMA transfer data to LED strip

typedef enum //enumeration for saving the arrays indexes (time, degrees and beta-coef)
{	
	 SIX, SEVEN, EIGHT, NINE, TEN, ELEVEN, TWELVE, THIRTEEN,
	 FOURTEEN, FIFTEEN, SIXTEEN, SEVENTEEN, EIGHTEEN
}TIME;

//Need to know the indexes for LED ON/OFF
//volatile for ability to change the variable in the interrupt
//
const uint8_t SIZE 		= 13; 																					 //array size
uint8_t time[SIZE] 		= {6,7,8,9,10,11,12,13,14,15,16,17,18}; 				 //time array
uint8_t degrees[SIZE] = {0,15,30,45,60,75,90,105,120,135,150,165,180}; //degrees array
double beta[SIZE]	  	= {1/6,2/6,3/6,4/6,5/6,1,1,5/6,4/6,3/6,2/6,1/6}; //beta-coefficient size
	
const uint16_t MARCH 		 = 80; 			//the 21.03 day of year	(not leap)
const uint16_t JUNE 		 = 173; 		//the 22.06 day of year	(not leap)
const uint16_t SEPTEMBER = 265; 		//the 23.09 day of year (not leap)
	 
volatile uint8_t LED 		 = 0;	  	//variable of LED ON/OFF each hour
volatile uint8_t dayTime = SIX;	//variable of day time (from seven to eighteen)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_NVIC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_TIM3_Init();
  MX_TIM4_Init();

  /* Initialize interrupts */
  MX_NVIC_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief NVIC Configuration.
  * @retval None
  */
static void MX_NVIC_Init(void)
{
  /* TIM3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) //Timer interrupt handler(callback)
{	
	//The variable for counting the result distance between chosen day and the 21.03 or 23.09
	//
	uint16_t resultDay = 0;
	uint8_t maternityTime = time[dayTime]; //variable for understanding current day time
	uint8_t azimuth = degrees[dayTime]; 	 //azimuth degree in current day time
	
	if(htim->Instance == TIM3)
	{
		HAL_TIM_Base_Stop_IT(&htim3); //Stop the timer and forbid the interrupt (for insurance)
		
		//Count the chosen day in the year (03.09)
		//
		uint16_t chosenDay = CalculateDayInYear(3,9);
		
		if(chosenDay > JUNE) //if the distance between current day and 22.06 (summer solstice) is bigger
		{
			resultDay = SEPTEMBER-chosenDay; //than we need to calculate relating the last date (23.09)
		}
		else
		{
			resultDay = chosenDay - MARCH; //else we need to calculate relating the first date (21.09)
		}
		
		//Calculate the zenith angle 
		//
		double zenith = ((90-LATITUDE)+EPSILON*resultDay)*beta[dayTime];
		
		
		//Every 10 seconds of interrupt is equal to 1 hour
		//
		switch(LED)
		{
			case 0:
				ws2812_clear(); //6:00. the gray sky. People can not see smth yet
			break;
			
			case 1:
				ws2812_pixel_rgb_to_buf_dma(150,0,204,LED); //7:00. the violet symbolizes the rising
			break;
			
			case 2:
				ws2812_pixel_rgb_to_buf_dma(255,0,0,LED); //8:00. the red sky during the sunrise
			break;
			
			case 3:
				ws2812_pixel_rgb_to_buf_dma(0,0,255,LED); //9:00. the blue sky. All is clear but the sun is not high yet
			break;
			
			case 4:
				ws2812_pixel_rgb_to_buf_dma(81,234,255,LED); //10:00. the light blue sky. The real sky color
			break;
			
			case 5:
				ws2812_pixel_rgb_to_buf_dma(255,255,0,LED); //11:00. the yellow color. maximum approximate point to zenith
			break;
			
			case 6:
				ws2812_pixel_rgb_to_buf_dma(255,96,0,LED); //12:00. the orange color. maximum sunny bright
			break;
			
			case 7:
				ws2812_pixel_rgb_to_buf_dma(255,255,0,LED); //13:00. the yellow color signalizes for starting the sun shifting
			break;
			
			case 8:
				ws2812_pixel_rgb_to_buf_dma(81,234,255,LED); //14:00. light blue color. The visible sun changing in the sky 
			break;
																														
			case 9:
				ws2812_pixel_rgb_to_buf_dma(0,0,255,LED);	//15:00. blue color means starting to sunset
			break;
			
			case 10:
				ws2812_pixel_rgb_to_buf_dma(255,0,0,LED); //16:00. red color means the real sunset
			break;
																																				
			case 11:	
				ws2812_pixel_rgb_to_buf_dma(150,0,204,LED); //17:00. purple color means the nearest point to sunset
			break;
																																							
			case 12:
				ws2812_clear(); //18:00. The sun is not in the vision and we suppose that is the full sunset
			break;
			
			default:
				for(uint8_t i = 0; i<LED_COUNT;i++)
				{
					ws2812_pixel_rgb_to_buf_dma(255,255,255,i); //default case for unpredictable program behaviour. White color
				}
			break;
		}


		//While the data is not transfer to LED strip
		//waiting for this
		//
		while(HAL_TIM_PWM_Start_DMA(&htim4,TIM_CHANNEL_1,(uint32_t*)&BUF_DMA,ARRAY_LEN));
		
		LED = (LED+1)%SIZE; 						//%SIZE(13) gives the LED value from 0 to 12
		dayTime = (dayTime+1)%SIZE; //calculate the time from rising to sunset (12 hours)
		
		__HAL_TIM_SET_AUTORELOAD(&htim3, PERIOD); //Reload the timer for interrupt restarting
		HAL_TIM_Base_Start_IT(&htim3); //return the interrupt mode
	}
}


/*
	Calculating the day's position in the year from 21.03 to 23.09 (a leap year isn't consider)
	1. Enter the month ordinal (from 3 to 9)
	2. Add 1 to the month number.
	3. Multiply by 30.6.
	4. Select the whole part.
	5. Subtract 63
	6. Add the number corresponding to this day. 
	As a result, the ordinal number of the day in the year is obtained. 

*/
uint16_t CalculateDayInYear(uint8_t day, uint8_t month)
{
	//Simple checking if the data is between 21.03 and 23.09
	//e.g. if it is 20.02 then the data is transform to 21.03
	//
	if(month < 3)
	{
		if(day < 21)
		{
			day = 21; 
		}
		month = 3;
	}
	
	if(month > 9)
	{
		if(day > 23)
		{
			day = 23;
		}
		month = 9;
	}
	
	uint16_t dayOfYear = 0; 

	dayOfYear = (month + 1)*30.6; //2. 3. 4.
	dayOfYear = dayOfYear - 63 + day;	//5. 6.
	
	return dayOfYear;
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
