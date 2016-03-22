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
#ifndef _UART_TP_IF_H_
#define _UART_TP_IF_H_ 

/**** Definition of constants ****/
#include <stdint.h>
#include "stm32f10x.h"


#define UART_FRAME_SIZE 30

#define TBOX_CMD_TYPE        0
#define TBOX_CMD_ID_H       1
#define TBOX_CMD_ID_L        2
#define TBOX_CMD_SIZE        3
#define TBOX_CMD_PRIO		 		 4
#define TBOX_CMD_OPTION      11
#define TBOX_CMD_PARAM_START 		12
#define TBOX_UART_BASE_LEN 4

#define TBOX_CMD_DATA_1 		 5
#define TBOX_CMD_DATA_2 		 6
#define TBOX_CMD_DATA_3 		 7
#define TBOX_CMD_DATA_4 		 8
#define TBOX_CMD_DATA_5 		 9

/**** Definition of types ****/ 

typedef struct 
{
    BOOL bValid; /** if valid data? **/
    uint8_t length;
    uint8_t  buff[UART_FRAME_SIZE];   
} UART_FRAME_TYPE;

/**** Definition of macros ****/

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void uart_create(void);
extern void uart_polling(void);
extern void uart_read_polling(void);
extern void uart_send_polling(void);

extern BOOL uart_send_frame(uint8_t *frame,uint8_t length);

extern void uart_tx_proc(void);
extern void uart_rx_proc(void);

#endif /*_UART_TP_IF_H_*/
/****************************** END OF FILE ***************************/

