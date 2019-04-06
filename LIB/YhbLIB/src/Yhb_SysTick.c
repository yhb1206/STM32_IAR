  
#include "Yhb_SysTick.h"
#include "core_cm3.h"
#include "misc.h"
 
/**
  * @brief  不开中断的 SysTick的配置
  * @param  无
  * @retval 无
  */
void Yhb_SysTick_Init(void)
{
	/*根据时钟频率进行设置systick，保证适应任何系统时钟 
	 *SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
    RCC_ClocksTypeDef  RccClocks;
    uint32_t SystemClockFrequency = 0, u32Cnt = 0;

    RCC_GetClocksFreq(&RccClocks);

    SystemClockFrequency = RccClocks.HCLK_Frequency;
 
    u32Cnt = SystemClockFrequency/1000;
    
    if ((u32Cnt - 1) > SysTick_LOAD_RELOAD_Msk)    /* Reload value impossible */
    { 
        /* Capture error */ 
        while (1);
    }
    else
    {
        SysTick->LOAD  = u32Cnt  - 1;                                  /* set reload register */
        NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1);  /* set Priority for Systick Interrupt */
        SysTick->VAL   = 0;                                          /* Load the SysTick Counter Value */
        SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk;                                               
        SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk); /* Disable SysTick IRQ and SysTick Timer */
    }
}

/*先调用Yhb_SysTick_Init 将systick配置为非中断，
再调用此函数设置systick计数周期，周期到了标志置起，读取该位会清*/
void Yhb_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;
    // 配置 counter 计数器的值
    SysTick->VAL   = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Enable SysTick Timer,But diable SysTick IRQ*/
    for(i=0;i<ms;i++)
    {
        // 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
        // 当置1时，读取该位会清0
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    // 关闭SysTick定时器
    SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}


/*********************************************END OF FILE**********************/
