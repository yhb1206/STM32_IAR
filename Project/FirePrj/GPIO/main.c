/***************************************************************************
*FILE NAME:  main.c
*CREATE DATE: 2018��4��16��
*AUTHOR:  Yun huibin
*PURPOSE: 
*LAST MODIFY DATE: 
*REMARK: 

****************************************************************************/
/*GPIO�������*/
#define  GPIO_OUT_DEMO          (0)/*GPIO�������*/
#define  GPIO_IN_DEMO           (1)/*GPIO��������*/
#define  GPIO_BIT_BAND_DEMO     (2)/*λ����������*/
#define  GPIO_BEEP_DEMO         (3)/*beep����*/

/*GPIO�������ѡ��*/
#define  GPIO_DEMO_MODE  GPIO_IN_DEMO

/*������*/
#if GPIO_DEMO_MODE == GPIO_OUT_DEMO
/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_led.h"

#define SOFT_DELAY Delay(0x0FFFFF);

void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	/* LED �˿ڳ�ʼ�� */
	LED_GPIO_Config();	 

	while (1)
	{
		LED1_ON;			  // ��
		SOFT_DELAY;
		LED1_OFF;		   // ��

		LED2_ON;			 // ��
		SOFT_DELAY;
		LED2_OFF;		   // ��

		LED3_ON;			 // ��
		SOFT_DELAY;
		LED3_OFF;		   // ��	 

		/*������ʾ ������������� ��ɫ*/
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

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
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
  * @brief   ���԰���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "stm32f10x.h"
#include "bsp_led.h"  
#include "bsp_key.h" 

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */ 
int main(void)
{	
	/* LED�˿ڳ�ʼ�� */
	LED_GPIO_Config();
	LED1_ON;

	/* �����˿ڳ�ʼ�� */
	Key_GPIO_Config();
	
	/* ��ѯ����״̬��������������תLED */
	while(1)                            
	{	   
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			/*LED1��ת*/
			LED1_TOGGLE;
		} 

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			/*LED2��ת*/
			LED2_TOGGLE;
		}		
	}
}

#elif GPIO_DEMO_MODE == GPIO_BIT_BAND_DEMO

#include "stm32f10x.h"

// ����ֻ������ GPIO ODR��IDR�������Ĵ�����λ����������ַ�������Ĵ�����û�ж���

//SRAM λ����:    0X2000 0000~0X200F FFFF
//SRAM λ��������:0X2200 0000~0X23FF FFFF

//���� λ����:    0X4000 0000~0X400F FFFF
//���� λ��������:0X4200 0000~0X43FF FFFF

// �ѡ�λ����ַ+λ��š�ת���ɱ�����ַ�ĺ�
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x000FFFFF)<<5)+(bitnum<<2)) 
/*
 *addr & 0xF0000000��ȡ��ַ�ĸ�4λ��������2����4����������SRAM�������ַ��
 *�����2��+0x02000000��=0X2200 0000������SRAM�������4��+0x02000000��=0X4200 0000����������
 *
 *addr & 0x000FFFFF�����ε���12λ���൱��-0X2000 0000����-0X4000 0000�������ʾƫ��λ�������ٸ��ֽ�
 *<<5  ����*8*4����Ϊλ����һ����ַ��ʾһ���ֽڣ�һ���ֽ���8��bit��һ��bit�������ͳ�һ���֣���4���ֽ�
 *<<2 ����*4����Ϊһ��λ�������ͳ�һ���֣���4���ֽ�
 *
 *�ֽ��������ʽӦ�þ���������
 *SRAMλ��������ַ
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 *����λ��������ַ
 *AliasAddr= 0x42000000+((A-0x40000000)*8+n)*4 =0x42000000+ (A-0x40000000)*8*4 +n*4
 */


// ��һ����ַת����һ��ָ��
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 

// ��λ����������ַת����ָ��
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))   

// GPIO ODR �� IDR �Ĵ�����ַӳ�� 
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


// �������� GPIO��ĳһ��IO�ڣ�n(0,1,2...16),n��ʾ��������һ��IO��
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //���   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //����   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //���   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //����   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //���   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //����   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //���   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //����   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //���   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //����  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //���   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //����  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //���   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //����  

void SOFT_Delay(__IO uint32_t nCount);	
void LED_GPIO_Config_Yhb(void);

int main(void)
{	
	// ��������main����֮ǰ�������ļ���statup_stm32f10x_hd.s�Ѿ�����
	// SystemInit()������ϵͳʱ�ӳ�ʼ����72MHZ
	// SystemInit()��system_stm32f10x.c�ж���
	// ����û����޸�ϵͳʱ�ӣ������б�д�����޸�
	
	LED_GPIO_Config_Yhb();
	
	while( 1 )
	{
		// PB0 = 0,����LED
		PBout(0)= 0;		
		SOFT_Delay(0x0FFFFF);
		
		// PB1 = 1,Ϩ��LED		
		PBout(0)= 1;
		SOFT_Delay(0x0FFFFF);		
	}
}

	

void LED_GPIO_Config_Yhb(void)
{		
		// ����һ��GPIO_InitTypeDef���͵Ľṹ��
		GPIO_InitTypeDef GPIO_InitStructure;

		// ����GPIOB��ʱ��
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

		// ѡ��Ҫ���Ƶ�IO��													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	

		// ��������Ϊ�������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		// ������������Ϊ50MHz
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*���ÿ⺯������ʼ��GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);		  

		// �ر�LED
		GPIO_SetBits(GPIOB, GPIO_Pin_0); 
}

// ��ª�������ʱ����
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
  * @brief   ����led
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� �Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://fire-stm32.taobao.com  *
  ******************************************************************************
  */ 
	
#include "stm32f10x.h"
#include "bsp_beep.h"

void Delay(__IO u32 nCount); 

/**
  * @brief  ������
  * @param  ��  
  * @retval ��
  */
int main(void)
{	
	/* BEEP GPIO ��ʼ�� */
	BEEP_GPIO_Config();	
	
	while(1)
	{		
		BEEP( ON ); 			  // ��
		Delay(0x0FFFFF);	
		BEEP( OFF );		  // ����
		Delay(0x0FFFFF);
	}
}

void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
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
