/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : FDC1004.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/18 13:49:5:73
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/18 13:49:5:73
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __FDC1004_H__
#define __FDC1004_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  FDC1004_GLOBAL
#define FDC1004_EXT
#else
#define FDC1004_EXT extern
#endif /* FDC1004_GLOBAL */

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
float FDC1004_GetLiquidLevel(void);
void FDC1004_Init(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __FDC1004_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
