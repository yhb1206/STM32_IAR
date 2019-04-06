  
#include "Yhb_SysTick.h"
#include "core_cm3.h"
#include "misc.h"
 
/**
  * @brief  �����жϵ� SysTick������
  * @param  ��
  * @retval ��
  */
void Yhb_SysTick_Init(void)
{
	/*����ʱ��Ƶ�ʽ�������systick����֤��Ӧ�κ�ϵͳʱ�� 
	 *SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
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

/*�ȵ���Yhb_SysTick_Init ��systick����Ϊ���жϣ�
�ٵ��ô˺�������systick�������ڣ����ڵ��˱�־���𣬶�ȡ��λ����*/
void Yhb_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;
    // ���� counter ��������ֵ
    SysTick->VAL   = 0;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* Enable SysTick Timer,But diable SysTick IRQ*/
    for(i=0;i<ms;i++)
    {
        // ����������ֵ��С��0��ʱ��CRTL�Ĵ�����λ16����1
        // ����1ʱ����ȡ��λ����0
        while( !((SysTick->CTRL)&(1<<16)) );
    }
    // �ر�SysTick��ʱ��
    SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;
}


/*********************************************END OF FILE**********************/
