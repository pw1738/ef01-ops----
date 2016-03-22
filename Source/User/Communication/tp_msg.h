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
#ifndef _TP_UART_MSG_H_
#define _TP_UART_MSG_H_ 


#include "Communication\tp_msg_if.h"   
/**** Definition of constants ****/


/**** Definition of macros ****/

/**** Declaration of constants ****/

/**** Declaration of variables ****/


/**** Declaration of functions ****/
static void msg_init_var (void);
static void uart_msg_frame_mk(uint8_t *frame,uint8_t nArgs,uint8_t *pArg );
static void uart_msg_frame_decode(uint8_t *frame, UART_FRAME  *uart_frame);

void uart_msg_handle(uint8_t *frame);

void heartbeat_send(void);
void broadcast_send(void);
void SprayerStateSwitchMessage_Rep_send(void);
void SprayerInforSetMessage_Rep_send(void);


void set_spayer_status(int a);
void set_pump_status(int a);

#endif /*_TP_UART_MSG_H_*/

/****************************** END OF FILE ***************************/


