/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Commu_Uart.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/23 14:39:23:829
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/23 14:39:23:829
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  COMMU_UART_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Communication\Commu_Uart.h"


    
/* Private typedef&macro&definde----------------------------------------------*/
#define COMMU_COM                        USART3
#define COMMU_COM_CLK                    RCC_APB1Periph_USART3
#define COMMU_COM_TX_PIN                 GPIO_Pin_10
#define COMMU_COM_TX_GPIO_PORT           GPIOB
#define COMMU_COM_TX_GPIO_CLK            RCC_APB2Periph_GPIOB
#define COMMU_COM_TX_SOURCE              GPIO_PinSource10
#define COMMU_COM_TX_AF                  
#define COMMU_COM_RX_PIN                 GPIO_Pin_11
#define COMMU_COM_RX_GPIO_PORT           GPIOB
#define COMMU_COM_RX_GPIO_CLK            RCC_APB2Periph_GPIOB
#define COMMU_COM_RX_SOURCE              GPIO_PinSource11
#define COMMU_COM_RX_AF                  
#define COMMU_COM_IRQn                   USART3_IRQn    




/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/


static void _Commu_Uart_LowLevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //    NVIC_InitTypeDef NVIC_InitStructure;   //Set the interrupt in the same place with timer and DMA
    USART_InitTypeDef USART_InitStructure;

    /*   USART3 Pins(PB) Software  */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); 

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Enable the USART3 Interrupt */ 
    //     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    //     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    //     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    //     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //     NVIC_Init(&NVIC_InitStructure); 

    USART_InitStructure.USART_BaudRate = 57600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	

    USART_Init(COMMU_COM, &USART_InitStructure);
    USART_ITConfig(COMMU_COM, USART_IT_RXNE, ENABLE);  //Receive Data register not empty
    USART_ITConfig(COMMU_COM, USART_IT_TC, ENABLE);     // Transmit complete 
    /* Enable USART3 */
    USART_Cmd(COMMU_COM, ENABLE);
}
    

void Commu_Uart_Init(void)
{
    _Commu_Uart_LowLevel_Init();
}


void Commu_Uart_SendChar(uint8_t send_char)
{
    USART_SendData(COMMU_COM, send_char);
}


void Commu_Uart_RecvChar(uint8_t *recv_char)
{
    *recv_char = USART_ReceiveData(COMMU_COM);
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
