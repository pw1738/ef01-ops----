/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : OS.h
 Author      : ranwei
 Version     : 
 Date        : 2016/1/21 16:5:1:93
 Description : 
 Others      : 
*******************************************************************************/
#ifndef __OS_H__
#define __OS_H__

#ifdef __cplusplus
extern "C"{
#endif

#ifdef  OS_GLOBAL
#define OS_EXT
#else
#define OS_EXT extern
#endif /* __cplusplus */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/



/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/



/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/



/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
void OS_Init(void);
void OS_Loop(void);
void OS_Time_Update(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __OS_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
