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
* Author: wangwei
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name      Changes and comments
* V1.0				20151215	wangwei
*=====================================================================*/
#ifndef _UART_IF_H_
#define _UART_IF_H_

#include "stm32f10x.h"

/**** Definition of constants ****/


/**** Definition of types ****/
//typedef enum
//{
//    UART_BAUDRATE_9600 = APB2_BUS_CLK / ( U32 )9600 - 1,
//    UART_BAUDRATE_14400 = APB2_BUS_CLK / ( U32 )14400 - 1,
//    UART_BAUDRATE_19200 = APB2_BUS_CLK / ( U32 )19200- 1,
//    UART_BAUDRATE_34800 = APB2_BUS_CLK / ( U32 )34800 - 1,
 //   UART_BAUDRATE_38400 = APB2_BUS_CLK / ( U32 )38400 - 1,
 //   UART_BAUDRATE_56000 = APB2_BUS_CLK / ( U32 )56000 - 1,
//    UART_BAUDRATE_57600 = APB2_BUS_CLK / ( U32 )57600 - 1,
 //   UART_BAUDRATE_115200 = APB2_BUS_CLK / ( U32 )115200 - 1,
//} UART_BAUDRATE_TYPE;

/**** Definition of macros ****/
#define     UART_RX_IT              0x0010
#define     UART_TX_IT              0x0008
#define     UART_RX_EN              0x0002
#define     UART_TX_EN              0x0001

#define     UART_DATA_LEN_7         0x0300
#define     UART_DATA_LEN_8         0x0000
#define     UART_DATA_LEN_9         0x0400
#define     UART_PARITY_ODD         0x1800
#define     UART_PARITY_EVEN        0x1000

#define  UART_TYPE                  FM3_MFS47_UART_TypeDef
#define  UART_0                     (UART_TYPE*)FM3_MFS0_UART
#define  UART_1                     (UART_TYPE*)FM3_MFS1_UART
#define  UART_2                     (UART_TYPE*)FM3_MFS2_UART
#define  UART_3                     (UART_TYPE*)FM3_MFS3_UART
#define  UART_4                     (UART_TYPE*)FM3_MFS4_UART
#define  UART_5                     (UART_TYPE*)FM3_MFS5_UART
#define  UART_6                     (UART_TYPE*)FM3_MFS6_UART
#define  UART_7                     (UART_TYPE*)FM3_MFS7_UART

#define IS_UART_BAUDRATE_TYPE(baudrate)     ((baudrate==UART_BAUDRATE_9600)||(baudrate==UART_BAUDRATE_14400)||(baudrate==UART_BAUDRATE_19200)||\
                                             (baudrate==UART_BAUDRATE_38400)||(baudrate==UART_BAUDRATE_56000)||(baudrate==UART_BAUDRATE_57600)||\
                                             (baudrate==UART_BAUDRATE_115200))

#define IS_UART_TYPE(uart)                  ((uart==UART_0)||(uart==UART_1)||(uart==UART_2)||(uart==UART_3)||\
                                             (uart==UART_4)||(uart==UART_5)||(uart==UART_6)||(uart==UART_7))
/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/


#endif
/****************************** END OF FILE ***************************/
