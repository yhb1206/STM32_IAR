/***************************************************************************
*FILE NAME:  main.c
*CREATE DATE: 2018年4月16日
*AUTHOR:  Yun huibin
*PURPOSE: 
*LAST MODIFY DATE: 
*REMARK: 

****************************************************************************/
/*GPIO相关例程*/
#define  GPIO_OUT_DEMO          (0)/*GPIO输出例程*/
#define  GPIO_IN_DEMO           (1)/*GPIO输入例程*/
#define  GPIO_BIT_BAND_DEMO     (2)/*位带操作例程*/
#define  GPIO_BEEP_DEMO         (3)/*beep例程*/

/*GPIO相关例程选择*/
#define  GPIO_DEMO_MODE  GPIO_IN_DEMO

/*各例程*/
#if GPIO_DEMO_MODE == GPIO_OUT_DEMO
/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"

#define SOFT_DELAY Delay(0x0FFFFF);

void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();	 

	while (1)
	{
		LED1_ON;			  // 亮
		SOFT_DELAY;
		LED1_OFF;		   // 灭

		LED2_ON;			 // 亮
		SOFT_DELAY;
		LED2_OFF;		   // 灭

		LED3_ON;			 // 亮
		SOFT_DELAY;
		LED3_OFF;		   // 灭	 

		/*轮流显示 红绿蓝黄紫青白 颜色*/
		LED_RED;
		SOFT_DELAY;
		
		LED_GREEN;
		SOFT_DELAY;
		
		LED_BLUE;
		SOFT_DELAY;
		
		LED_YELLOW;
		SOFT_DELAY;
		
		LED_PURPLE;
		SOFT_DELAY;
				
		LED_CYAN;
		SOFT_DELAY;
		
		LED_WHITE;
		SOFT_DELAY;
		
		LED_RGBOFF;
		SOFT_DELAY;		
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

#elif  GPIO_DEMO_MODE == GPIO_IN_DEMO

/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试按键
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "bsp_led.h"  
#include "bsp_key.h" 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	
	/* LED端口初始化 */
	LED_GPIO_Config();
	LED1_ON;

	/* 按键端口初始化 */
	Key_GPIO_Config();
	
	/* 轮询按键状态，若按键按下则反转LED */
	while(1)                            
	{	   
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			/*LED1反转*/
			LED1_TOGGLE;
		} 

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			/*LED2反转*/
			LED2_TOGGLE;
		}		
	}
}

#elif GPIO_DEMO_MODE == GPIO_BIT_BAND_DEMO

#include "stm32f10x.h"

// 这里只定义了 GPIO ODR和IDR这两个寄存器的位带别名区地址，其他寄存器的没有定义

//SRAM 位带区:    0X2000 0000~0X200F FFFF
//SRAM 位带别名区:0X2200 0000~0X23FF FFFF

//外设 位带区:    0X4000 0000~0X400F FFFF
//外设 位带别名区:0X4200 0000~0X43FF FFFF

// 把“位带地址+位序号”转换成别名地址的宏
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x000FFFFF)<<5)+(bitnum<<2)) 
/*
 *addr & 0xF0000000，取地址的高4位，看看是2还是4，用于区分SRAM和外设地址，
 *如果是2，+0x02000000则=0X2200 0000，即是SRAM，如果是4，+0x02000000则=0X4200 0000，即是外设
 *
 *addr & 0x000FFFFF，屏蔽掉高12位，相当于-0X2000 0000或者-0X4000 0000，结果表示偏移位带区多少个字节
 *<<5  等于*8*4，因为位带区一个地址表示一个字节，一个字节有8个bit，一个bit可以膨胀成一个字，即4个字节
 *<<2 等于*4，因为一个位可以膨胀成一个字，即4个字节
 *
 *分解成两条公式应该就是这样：
 *SRAM位带别名地址
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 *外设位带别名地址
 *AliasAddr= 0x42000000+((A-0x40000000)*8+n)*4 =0x42000000+ (A-0x40000000)*8*4 +n*4
 */


// 把一个地址转换成一个指针
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 

// 把位带别名区地址转换成指针
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))   

// GPIO ODR 和 IDR 寄存器地址映射 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C   
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C   
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C   
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C   
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C   
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C      
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C      
  
#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808   
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08   
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008   
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408   
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808   
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08   
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 


// 单独操作 GPIO的某一个IO口，n(0,1,2...16),n表示具体是哪一个IO口
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入  

void SOFT_Delay(__IO uint32_t nCount);	
void LED_GPIO_Config_Yhb(void);

int main(void)
{	
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用
	// SystemInit()函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
	
	LED_GPIO_Config_Yhb();
	
	while( 1 )
	{
		// PB0 = 0,点亮LED
		PBout(0)= 0;		
		SOFT_Delay(0x0FFFFF);
		
		// PB1 = 1,熄灭LED		
		PBout(0)= 1;
		SOFT_Delay(0x0FFFFF);		
	}
}

	

void LED_GPIO_Config_Yhb(void)
{		
		// 定义一个GPIO_InitTypeDef类型的结构体
		GPIO_InitTypeDef GPIO_InitStructure;

		// 开启GPIOB的时钟
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

		// 选择要控制的IO口													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

		// 设置引脚为推挽输出
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		// 设置引脚速率为50MHz
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);		  

		// 关闭LED
		GPIO_SetBits(GPIOB, GPIO_Pin_0); 
}

// 简陋的软件延时函数
void SOFT_Delay(__IO uint32_t nCount)	
{
	for(; nCount != 0; nCount--);
}

#elif GPIO_DEMO_MODE == GPIO_BEEP_DEMO

/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   测试led
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_beep.h"

void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* BEEP GPIO 初始化 */
	BEEP_GPIO_Config();	
	
	while(1)
	{		
		BEEP( ON ); 			  // 响
		Delay(0x0FFFFF);	
		BEEP( OFF );		  // 不响
		Delay(0x0FFFFF);
	}
}

void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif
/*********************************************END OF FILE**********************/

#endif

/*********************************************END OF FILE**********************/
