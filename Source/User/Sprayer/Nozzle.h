/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Nozzle.h
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 15:24:53:798
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/2/19 15:24:53:798
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __NOZZLE_H__
#define __NOZZLE_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  NOZZLE_GLOBAL
#define NOZZLE_EXT
#else
#define NOZZLE_EXT extern
#endif /* NOZZLE_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include "stm32f10x.h"
#include "Sprayer\Valve.h"
#include "Sprayer\Dev.h"


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
/* 离心喷头 */
typedef struct ST_Nozzle{
    Dev *dev;

    uint32_t timerCLK;
    uint32_t gpioCLK;
    uint16_t gpioPin;
    GPIO_TypeDef *gpioPort;
    TIM_TypeDef *timX;
    uint8_t timChNum; // pwm通道号，1~4

    //void (* init)(struct ST_Nozzle *this);
    void (* set)(struct ST_Nozzle *this, uint8_t level);
}Nozzle;



/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/
#ifndef  NOZZLE_GLOBAL
extern Nozzle g_stDevNozzle1;
#endif /* NOZZLE_GLOBAL */



/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NOZZLE_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
