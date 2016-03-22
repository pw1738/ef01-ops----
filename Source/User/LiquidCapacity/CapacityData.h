/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : CapacityData.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/21 14:5:29:458
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/21 14:5:29:458
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __CAPACITYDATA_H__
#define __CAPACITYDATA_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  CAPACITYDATA_GLOBAL
#define CAPACITYDATA_EXT
#else
#define CAPACITYDATA_EXT extern
#endif /* CAPACITYDATA_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdint.h>

/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/

typedef struct{
    float level;
}ST_CapacityData_Type;

typedef ST_CapacityData_Type CapacityData;






/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
/* Constructors and destructors:*/
CapacityData* CapacityData_Create(void);
void CapacityData_Destroy(CapacityData* const me);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CAPACITYDATA_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
