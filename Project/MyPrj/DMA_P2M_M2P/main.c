// DMA ���裨���ڣ����洢�����ݴ���ʵ��

#include "stm32f10x.h"
#include "./usart/bsp_usart_dma.h"
#include "./led/bsp_led.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
static void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
  uint16_t i;
  /* ��ʼ��USART */
  USART_Config(); 

  /* ����ʹ��DMAģʽ */
  USARTx_DMA_Config();
  
  /* ����RGB��ɫ�� */
  LED_GPIO_Config();

#if(DMA_TRAN_DIR == M2P_TRAN_DIR)/*�ڴ浽����*/
  printf("\r\n USART1 DMA TX �ڴ浽���� M2P����\r\n");
  /*��佫Ҫ���͵�����*/
  for(i=0;i<SENDBUFF_SIZE;i++)
  {
    SendBuff[i]	 = 'P';
    
  }
  i = 0;
  
  /*Ϊ��ʾDMA�������ж�CPU���ܴ����������飬����ʹ��DMA�������ݣ����ǳ���
  *��ʱ�����п��ܻᵼ�µ��Զ˴��ڵ������ֻῨ��������ҷɵ������
  *���DMA�����е�ѭ��ģʽ��Ϊ����ģʽ*/		
  
  /* USART1 �� DMA����TX���� */
  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);

  /* ��ʱCPU�ǿ��еģ����Ը����������� */  
  //����ͬʱ����LED
  while(1)
  {
    LED1_TOGGLE
    Delay(0xFFFFF);
    i++;
    if(i == 3)
    {
        DMA_Cmd (USART_TX_DMA_CHANNEL,DISABLE);
        printf("\r\n M2P������ͣ����Ϣһ�����ϼ����������ȴ�Ƭ��\r\n");

    }
    
    if(i >= 60)
    {
        printf("\r\n M2P�����ٴο�ʼ\r\n");
        DMA_Cmd (USART_TX_DMA_CHANNEL,ENABLE);
        i = 0;
    }

  }

#elif(DMA_TRAN_DIR == P2M_TRAN_DIR)/*���赽�ڴ�*/
    printf("\r\n USART1 DMA RX ���赽�ڴ� P2M���� \r\n");
    /*��佫Ҫ���ܵ�����*/
    for(i=0;i<SENDBUFF_SIZE;i++)
    {
      SendBuff[i]  = '\0';
      
    }
    
  /* USART1 �� DMA����RX���� */
    USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);

    /* ���ڴ��ڵ����RX���ʱ��ֻ��CPU���������������ˣ�DMA�Ż��������ʵ���Ѿ��ڹ�����ֻ������Ҫcpu����� */  
    //����ͬʱ����LED
    while(1)
    {
      LED1_TOGGLE
      Delay(0xFFFFF);
      //DMA_Cmd (USART_TX_DMA_CHANNEL,DISABLE);
    }
#endif 
}

static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
