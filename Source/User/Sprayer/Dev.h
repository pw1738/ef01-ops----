/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Dev.h
 Author      : ranwei
 Version     : 
 Date        : 2016/2/24 16:29:28:817
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/2/24 16:29:28:817
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __DEV_H__
#define __DEV_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  DEV_GLOBAL
#define DEV_EXT
#else
#define DEV_EXT extern
#endif /* DEV_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/

/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
typedef struct{
    void *child;
    void (*init)(void *child, void *this);
    void (*disable)(void *this);
    void (*enable)(void *this);    
}ST_Dev;

typedef ST_Dev Dev;

/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __DEV_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
