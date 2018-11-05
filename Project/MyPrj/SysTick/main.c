
#include "stm32f10x.h"
#include "bsp_SysTick.h"
#include "bsp_led.h"
#include "Yhb_SysTick.h"

/*
 * t : 定时时间 
 * Ticks : 多少个时钟周期产生一次中断 
 * f : 时钟频率 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();

#if 0
	/* 配置SysTick 为10us中断一次 */
	SysTick_Init();	
	for(;;)
	{

		LED1( ON ); 
	    Delay_us(100000);    	// 100000 * 10us = 1000ms
		//Delay_ms(100);
		LED1( OFF );
	  
		LED2( ON );
	    Delay_us(100000);		// 100000 * 10us = 1000ms
		//Delay_ms(100);
		LED2( OFF );
	
		LED3( ON );
	    Delay_us(100000);		// 100000 * 10us = 1000ms
		//Delay_ms(100);
		LED3( OFF );
	}     
#else // 不使用中断，使用查询的方法
    
    Yhb_SysTick_Init();
    
	for(;;)
	{

		LED1( ON ); 
        Yhb_Delay_Ms(1000);
//		SysTick_Delay_Ms( 1000 );
		LED1( OFF );
	  
		LED2( ON );
        Yhb_Delay_Ms(1000);
//		SysTick_Delay_Ms( 1000 );
		LED2( OFF );
	
		LED3( ON );
        Yhb_Delay_Ms(1000);
//      SysTick_Delay_Ms( 1000 );
		LED3( OFF );
	} 
#endif	
}


/*********************************************END OF FILE**********************/
