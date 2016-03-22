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
#ifndef _FIFO_IF_H_
#define _FIFO_IF_H_ 

#include <stdint.h>
#include "stm32f10x.h"

/**** Definition of constants ****/


/**** Definition of types ****/ 
typedef struct 
{
    uint8_t  *mpFIFOBuffer;
    uint16_t  mFIFOHead;   
    uint16_t  mFIFOTail;  
    uint16_t  mFIFOSize;
} FIFO_TYPE;

/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern void fifo_init( FIFO_TYPE *this, uint8_t *pBuf, uint16_t nSize);
extern BOOL fifo_input_frame( FIFO_TYPE *this ,   uint8_t *pFrame, uint8_t length);
extern BOOL fifo_output_frame( FIFO_TYPE *this,  uint8_t *pFrame, uint8_t *length);
#ifdef TP_UART_COMMUNICATION
extern BOOL fifo_is_empty(FIFO_TYPE *this  );
#endif	/*TP_UART_COMMUNICATION*/
#endif /*_FIFO_IF_H_*/

/****************************** END OF FILE ***************************/
