/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Fifo.h
 Author      : rw
 Version     : 
 Date        : 2016/3/8 22:34:50:274
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/8 22:34:50:274
    Author       -- rw
    Modification -- Created file

*******************************************************************************/
#ifndef __FIFO_H__
#define __FIFO_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  FIFO_GLOBAL
#define FIFO_EXT
#else
#define FIFO_EXT extern
#endif /* FIFO_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdint.h>


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
typedef struct 
{
    uint8_t  *pucFifoBuffer;
    uint16_t  usFifoReadPtr;   
    uint16_t  usFifoWritePtr;  
    uint16_t  usFifoSize;
}ST_Fifo_Type;



/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
void Fifo_Init(ST_Fifo_Type * this, uint8_t * pBuf, uint16_t nSize);
BOOL Fifo_Input_Frame(ST_Fifo_Type * this, uint8_t * pFrame, uint8_t length);
BOOL Fifo_Output_Frame(ST_Fifo_Type * this, uint8_t * pFrame, uint8_t * pLength);
BOOL Fifo_IS_Empty(ST_Fifo_Type *this);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FIFO_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
