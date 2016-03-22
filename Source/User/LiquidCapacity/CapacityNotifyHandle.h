/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : CapacityNotifyHandle.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/21 14:26:57:492
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/21 14:26:57:492
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __CAPACITYNOTIFYHANDLE_H__
#define __CAPACITYNOTIFYHANDLE_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  CAPACITYNOTIFYHANDLE_GLOBAL
#define CAPACITYNOTIFYHANDLE_EXT
#else
#define CAPACITYNOTIFYHANDLE_EXT extern
#endif /* CAPACITYNOTIFYHANDLE_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include "LiquidCapacity\CapacityData.h"


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
#define SZ_CapacityNotifyHandle_NUM_Max 10

typedef void (*CapacityData_Acceptor_Func_Type)(void *, CapacityData *);

typedef struct{
    CapacityData_Acceptor_Func_Type Accept;
    //void (* Accept)(void *, CapacityData *);
    void *pInstance;
}ST_CapacityNotifyHandle_Type;

typedef ST_CapacityNotifyHandle_Type CapacityNotifyHandle;



/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
/* Constructors and destructors:*/
CapacityNotifyHandle* CapacityNotifyHandle_Create(void);
void CapacityNotifyHandle_Destroy(CapacityNotifyHandle* const me);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPACITYNOTIFYHANDLE_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
