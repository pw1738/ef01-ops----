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
#ifndef __USART__H
#define __USART__H

#include <stdint.h>
#include "stm32f10x.h"

typedef enum { UART1_FLAG_TXE   = (uint16_t)0x0080, /*!< Transmit Data Register Empty flag */
               UART1_FLAG_TC    = (uint16_t)0x0040, /*!< Transmission Complete flag */
               UART1_FLAG_RXNE  = (uint16_t)0x0020, /*!< Read Data Register Not Empty flag */
               UART1_FLAG_IDLE  = (uint16_t)0x0010, /*!< Idle line detected flag */
               UART1_FLAG_OR    = (uint16_t)0x0008, /*!< OverRun error flag */
               UART1_FLAG_NF    = (uint16_t)0x0004, /*!< Noise error flag */
               UART1_FLAG_FE    = (uint16_t)0x0002, /*!< Framing Error flag */
               UART1_FLAG_PE    = (uint16_t)0x0001, /*!< Parity Error flag */
               UART1_FLAG_LBDF  = (uint16_t)0x0210, /*!< Line Break Detection Flag */
               UART1_FLAG_SBK   = (uint16_t)0x0101  /*!< Send Break characters Flag */
             } UART1_Flag_TypeDef;

#define dma_len 100  //????DMA??????
void UART_SendChar(uint8_t data) ;
void UART_RecvChar(uint8_t *recv_char ) ;
//extern u8 USART1_DMA_Buf1[dma_len]; //BUF1
//extern u8 USART1_DMA_Buf2[dma_len];	//BUF2
typedef enum {BUF_NO1=0,BUF_NO2=1}BUF_NO;
extern BUF_NO Free_Buf_No;
extern BOOL Buf_Ok;
void USART_Config(void);

void UART_Send_Byte(char c);
void UART_Send_Enter(void);

void UART_Send_Str(char *s);

//void UART_Put_Num(unsigned long dat);
//void UART_Put_Inf(char *inf,unsigned long dat);

// void u32tostr(unsigned long dat,char *str);
// unsigned long strtou32(char *str) ;

void USART_Puts( char * str);
void USART_Putc(unsigned char c);

void UART_Disable(FunctionalState cmd);
void USART_DMAToBuf1(void);
#endif



