/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Commu_Uart.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/23 14:39:55:258
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/23 14:39:55:258
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __COMMU_UART_H__
#define __COMMU_UART_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  COMMU_UART_GLOBAL
#define COMMU_UART_EXT
#else
#define COMMU_UART_EXT extern
#endif /* COMMU_UART_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdint.h>


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/

/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
void Commu_Uart_Init(void);
void Commu_Uart_SendChar(uint8_t send_char);
void Commu_Uart_RecvChar(uint8_t *recv_char);




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMU_UART_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
