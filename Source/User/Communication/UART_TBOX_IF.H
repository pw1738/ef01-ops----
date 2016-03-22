/***********************************************************************
*   Copyright (C) Huizhou Desay SV Automotive Co., Ltd.				   *
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
* Author: 
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name    Changes and comments
* 
*=====================================================================*/
#ifndef _UART_TBOX_IF_H_
#define _UART_TBOX_IF_H_ 

/**** Definition of constants ****/

#define UART_TBOX_FRAME_SIZE 150


/**** Definition of types ****/ 

typedef struct 
{
    BOOL bValid; /** if valid data? **/
    U8 length;
    U8  buff[UART_TBOX_FRAME_SIZE];   
} UART_TBOX_FRAME_TYPE;

/**** Definition of macros ****/

/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void uart_tbox_create(void);
extern void uart_tbox_polling(void);
extern void uart_tbox_read_polling(void);
extern void uart_tbox_send_polling(void);
extern BOOL uart_tbox_send_frame(U8 const *frame,U8 length);
extern void uart_tbox_rx_proc(void);
extern void uart_tbox_tx_proc(void);
extern void uart_tbox_down(void);
#endif /*_UART_TBOX_IF_H_*/
/****************************** END OF FILE ***************************/

