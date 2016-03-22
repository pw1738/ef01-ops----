/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : LiquidCapacity.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/21 14:4:4:298
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/21 14:4:4:298
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __LIQUIDCAPACITY_H__
#define __LIQUIDCAPACITY_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  LIQUIDCAPACITY_GLOBAL
#define LIQUIDCAPACITY_EXT
#else
#define LIQUIDCAPACITY_EXT extern
#endif /* LIQUIDCAPACITY_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdint.h>
#include "LiquidCapacity\CapacityData.h"
#include "LiquidCapacity\CapacityNotifyHandle.h"


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/



typedef struct __ST_LiquidCapacity_Type{
    CapacityData* itsCapData;		
    CapacityNotifyHandle *itsCapNH[SZ_CapacityNotifyHandle_NUM_Max];	

    /* operations */
    void (*ModuleProc)(struct __ST_LiquidCapacity_Type* const me, void *);
    void (*Subscribe)(struct __ST_LiquidCapacity_Type* const me, void * pInstance, CapacityData_Acceptor_Func_Type Accept);
    void (*UnSubscribe)(struct __ST_LiquidCapacity_Type* const me, CapacityData_Acceptor_Func_Type Accept);
}ST_LiquidCapacity_Type;

typedef ST_LiquidCapacity_Type LiquidCapacity;

/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
LiquidCapacity * LiquidCapacity_Create(void);
void LiquidCapacity_Destroy(LiquidCapacity* const me);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LIQUIDCAPACITY_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
