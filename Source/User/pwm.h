/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : pwm.h
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 16:8:51:85
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/2/19 16:8:51:85
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __PWM_H__
#define __PWM_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  PWM_GLOBAL
#define PWM_EXT
#else
#define PWM_EXT extern
#endif /* PWM_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
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
void PWM_Init(void);
void PWM_Config(unsigned int SprayRate, unsigned int PumpRate);
void PWM_Disable(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PWM_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
