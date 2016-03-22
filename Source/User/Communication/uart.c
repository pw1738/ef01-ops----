/***********************************************************************
*   Copyright (c) 2015, Shenzhen YIFEIXING Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   :            										   *
************************************************************************
* Object        : 
* Module        : 
* Instance      : 
* Description   : 
*-----------------------------------------------------------------------
* Version: 
* Date: 
* Author: WangWei
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name      Changes and comments
* V1.0				20151215	WangWei
*=====================================================================*/
#include <stdio.h>
#include "stm32f10x.h"
#include "Communication\uart.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#ifdef TRACE_DEBUG_INFO_OPTION  
#ifdef _RAISONANCE_
#define PUTCHAR_PROTOTYPE int putchar (char c)
#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */
#endif	/*TRACE_DEBUG_INFO_OPTION*/



/* Evalboard I/Os configuration */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/***********************************************************************
*  Name        :  Uart_InitStruct
*  Description :  the parameters about uart
*  Parameter   :  none
*  Return      :  none
***********************************************************************/
// USART_InitTypeDef Usart_InitStruct=
// 					{
// 						9600 ,   // BaudRate
// 						USART_WordLength_8b ,        // WordLength
// 						USART_StopBits_1 ,     // StopBits
// 						USART_Parity_No ,     //  Parity
// 						USART_Mode_Rx | USART_Mode_Tx		,								//  Mode
// 						USART_HardwareFlowControl_None  //HardwareFlowControl
// 					};
/**********************************************************
** name:u32tostr
** describe: transefer a 32bits variable dat to a string,such as 1234 to "1234"
** param: dat: long variable
             str:a pointer to char array,to store  
** return: none
***********************************************************/
// void u32tostr(unsigned long dat,char *str) 
// {
// 	char temp[20];
// 	unsigned char i=0,j=0;
// 	i=0;
// 	while(dat)
// 	{
// 		temp[i]=dat%10+0x30;
// 		i++;
// 		dat/=10;
// 	}
// 	j=i;
// 	for(i=0;i<j;i++)
// 	{
// 	  	str[i]=temp[j-i-1];
// 	}
// 	if(!i) {str[i++]='0';}
// 	str[i]=0;
// }
/**********************************************************
** Name: strtou32
** Decription: string to unsigned32
** Parameter : @ *str      
** Return  :  Data of unsigned long   
***********************************************************/
// unsigned long strtou32(char *str) 
// {
// 	unsigned long temp=0;
// 	unsigned long fact=1;
// 	unsigned char len = strlen(str);
// 	unsigned char i;
// 	for(i=len;i>0;i--)
// 	{
// 		temp+=((str[i-1]-0x30)*fact);
// 		fact*=10;
// 	}
// 	return temp;
// }
/***********************************************************
*  Name  :    UART_Disable
*  Description  :  Disable the uart3
*  Parameters  :  cmd
*  Return   :  None
************************************************************/
void UART_Disable(FunctionalState cmd)
{
	USART_Cmd(USART2, cmd) ;
}
/***********************************************************
*  Name  :    USART_Config
*  Description  :  Configure  the usart
*  Parameters  :  None
*  Return   :  None
************************************************************/
void USART_Config ( void )
{   
    GPIO_InitTypeDef GPIO_InitStructure;
    //    NVIC_InitTypeDef NVIC_InitStructure;   //Set the interrupt in the same place with timer and DMA
    USART_InitTypeDef USART_InitStructure;

    	/*   USART3 Pins(PB) Software  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 
    	
    /*Configure USART3 Rx (PB11) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure USART3 Tx (PB10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

    USART_Init(USART3, &USART_InitStructure);
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  //Receive Data register not empty
    USART_ITConfig(USART3, USART_IT_TC, ENABLE);     // Transmit complete 
    /* Enable USART3 */
    USART_Cmd(USART3, ENABLE);
}


//??????,??printf??,??????use MicroLIB	  
/***************************START*********************/
// #if 1
// #pragma import(__use_no_semihosting)             
//??????????                 
// struct __FILE 
// { 
// 	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
//}; 
/* FILE is typedef?d in stdio.h. */ 
// FILE __stdout;       
//??_sys_exit()??????????    
// _sys_exit(int x) 
// { 
// 	x = x; 
// } 
//???fputc?? 
// int fputc(int ch, FILE *f)
// {      
// 	USART3->DR = (u8) ch;      
// 	while((USART2->SR&0X40)==0);//
// 	return ch;
// }
// #endif 
/***************************END*********************/

/**********************************************************
** Name :  USART3_Putc
** Description  :
** Parameter  : 
** Return  : 
***********************************************************/
// void USART_Putc(unsigned char c)
// {
//     USART2->DR = (u8)c; //
// 		while((USART2->SR&0X40)==0); //
// }
/**********************************************************
** Name :  USART3_Puts
** Description : 
** Parameters :
** Return : 
***********************************************************/
// void USART_Puts(char * str)
// {
//     while(*str)    //base of the  string is  negative
//     {
//         USART2->DR= *str++;
// 		while((USART2->SR&0X40)==0);//??????  
//     }
// }
/**********************************************************
** ???:UART_Send_Enter
** ????: ??1??????
** ????: ?
** ????: ?
***********************************************************/
// void UART_Send_Enter(void)
// {
// 	USART_Putc(0x0d);
// 	USART_Putc(0x0a);
// }
/**********************************************************
** ???:UART_Send_Str
** ????: ??1??????,???????
** ????: ??s
** ????: ?
***********************************************************/
// void UART_Send_Str(char *s)
// {
//  
//  	for(;*s;s++)
//  	{
// 	 	if(*s=='\n') 
// 	  		UART_Send_Enter();
// 	 	else
// 	  		USART_Putc(*s);
//  	}
// }
 /**********************************************************
** ???: UART_Put_Num
** ????: STM32F?USART????
** ????: dat:??????
** ????: ?
** ??:???????????????,??????? 4567 ?? "4567" 
***********************************************************/
// void UART_Put_Num(unsigned long dat)
// {
// 	char temp[20];
// 	u32tostr(dat,temp);
// 	UART_Send_Str(temp);
// }

/**********************************************************
** ???: UART_Put_Inf
** ????: STM32F?USART??????
** ????: inf:????????????
             dat:????,???????????????????
** ????: ? 
***********************************************************/
// void UART_Put_Inf(char *inf,unsigned long dat)
// {
// 	UART_Send_Str(inf);
// 	UART_Put_Num(dat);
// 	UART_Send_Str("\n");  
// }

/***********************************************************************
*  Name        : UART_RecvChar   ( There is no need to disable the usart)
*  Description : None
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void UART_RecvChar(uint8_t *recv_char )
{
    *recv_char = USART3 ->DR;
}

/***********************************************************************
*  Name        : UART_SendChar   ( There is no need to disable the usart)
*  Description : None
*  Parameter   : None
*  Returns     : None
***********************************************************************/
void UART_SendChar(uint8_t send_char)
{
	USART3->DR = send_char ;
}

/**************** END OF FILE *****************************************/









