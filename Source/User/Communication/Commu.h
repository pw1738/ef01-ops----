/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Commu.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/25 20:18:47:391
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/25 20:18:47:391
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __COMMU_H__
#define __COMMU_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  COMMU_GLOBAL
#define COMMU_EXT
#else
#define COMMU_EXT extern
#endif /* COMMU_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdint.h>
#include "stm32f10x.h"


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/

/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
void Commu_Init(void);
void Commu_Proc(void *arg);
BOOL Commu_Send(uint8_t nArgs, uint8_t *pArg);
void Commu_Send_HeartbeatPKT(void);
void Commu_ISR_TX_Proc(void);
void Commu_ISR_RX_Proc(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __COMMU_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
