/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Pump.h
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 14:20:23:206
 Description : 
 Others      : 
  
 History     :
  1.Date         -- 2016/2/19 14:20:23:206
    Author       -- 冉威
    Modification -- 创建文件 
    
*******************************************************************************/
#ifndef __PUMP_H__
#define __PUMP_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  PUMP_GLOBAL
#define PUMP_EXT
#else
#define PUMP_EXT extern
#endif /* PUMP_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdint.h>
#include "stm32f10x.h"
#include "Sprayer\Dev.h"


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
/* 水泵 */
typedef struct ST_Pump{
    Dev *dev;
    
    uint32_t timerCLK;
    uint32_t gpioCLK;
    uint16_t gpioPin;
    GPIO_TypeDef *gpioPort;
    TIM_TypeDef *timX;
    uint8_t timChNum; // pwm通道号，1~4

    //void (* init)(struct ST_Pump *this);
    void (* set)(struct ST_Pump *this, uint8_t level);

}Pump;


/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/
#ifndef  PUMP_GLOBAL
extern Pump g_stDevPump;
#endif /* PUMP_GLOBAL */


/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __PUMP_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
