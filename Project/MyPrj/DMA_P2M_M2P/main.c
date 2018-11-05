// DMA 外设（串口）到存储器数据传输实验

#include "stm32f10x.h"
#include "./usart/bsp_usart_dma.h"
#include "./led/bsp_led.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
static void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  uint16_t i;
  /* 初始化USART */
  USART_Config(); 

  /* 配置使用DMA模式 */
  USARTx_DMA_Config();
  
  /* 配置RGB彩色灯 */
  LED_GPIO_Config();

#if(DMA_TRAN_DIR == M2P_TRAN_DIR)/*内存到外设*/
  printf("\r\n USART1 DMA TX 内存到外设 M2P测试\r\n");
  /*填充将要发送的数据*/
  for(i=0;i<SENDBUFF_SIZE;i++)
  {
    SendBuff[i]	 = 'P';
    
  }
  i = 0;
  
  /*为演示DMA持续运行而CPU还能处理其它事情，持续使用DMA发送数据，量非常大，
  *长时间运行可能会导致电脑端串口调试助手会卡死，鼠标乱飞的情况，
  *或把DMA配置中的循环模式改为单次模式*/		
  
  /* USART1 向 DMA发出TX请求 */
  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);

  /* 此时CPU是空闲的，可以干其他的事情 */  
  //例如同时控制LED
  while(1)
  {
    LED1_TOGGLE
    Delay(0xFFFFF);
    i++;
    if(i == 3)
    {
        DMA_Cmd (USART_TX_DMA_CHANNEL,DISABLE);
        printf("\r\n M2P测试暂停，休息一会马上继续，请您等待片刻\r\n");

    }
    
    if(i >= 60)
    {
        printf("\r\n M2P测试再次开始\r\n");
        DMA_Cmd (USART_TX_DMA_CHANNEL,ENABLE);
        i = 0;
    }

  }

#elif(DMA_TRAN_DIR == P2M_TRAN_DIR)/*外设到内存*/
    printf("\r\n USART1 DMA RX 外设到内存 P2M测试 \r\n");
    /*填充将要接受的数据*/
    for(i=0;i<SENDBUFF_SIZE;i++)
    {
      SendBuff[i]  = '\0';
      
    }
    
  /* USART1 向 DMA发出RX请求 */
    USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);

    /* 对于串口的这个RX这个时候，只有CPU跑起来接受数据了，DMA才会般它，其实它已经在工作了只是这需要cpu的配合 */  
    //例如同时控制LED
    while(1)
    {
      LED1_TOGGLE
      Delay(0xFFFFF);
      //DMA_Cmd (USART_TX_DMA_CHANNEL,DISABLE);
    }
#endif 
}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
