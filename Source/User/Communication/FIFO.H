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
#ifndef _FIFO_H_
#define _FIFO_H_ 


#include "stm32f10x.h"
#include "Communication\FIFO_IF.H"
/**** Definition of constants ****/


/**** Definition of types ****/ 

/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/

BOOL fifo_is_empty(FIFO_TYPE *this  ) ;

static BOOL fifo_is_full(FIFO_TYPE *this  );
static BOOL fifo_input_byte( FIFO_TYPE *this , unsigned char byte );
static BOOL fifo_output_byte(FIFO_TYPE *this, uint8_t *pdata);
static uint16_t fifo_current_length(FIFO_TYPE *this  );
static uint16_t fifo_have_enough_space(FIFO_TYPE *this , uint8_t nBytes);


#endif /*_FIFO_IF_H_*/

/****************************** END OF FILE ***************************/
