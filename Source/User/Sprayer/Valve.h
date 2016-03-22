/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Valve.h
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 15:23:36:231
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/2/19 15:23:36:231
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __VALVE_H__
#define __VALVE_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  VALVE_GLOBAL
#define VALVE_EXT
#else
#define VALVE_EXT extern
#endif /* VALVE_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include "stm32f10x.h"
#include "Sprayer\Dev.h"



/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
typedef enum{
    EN_Valve_Stat_OFF,
    EN_Valve_Stat_ON
}EN_Valve_Stat_Type;


/* ∑ß√≈ */
typedef struct ST_Valve{
    Dev *dev;

    uint32_t gpioCLK;
    uint16_t gpioPin;
    GPIO_TypeDef *gpioPort;

    //void (* init)(struct ST_Valve *this);
    void (* StateSwitch)(struct ST_Valve *this, EN_StateSwitch_Type stat);
}Valve;



/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/
#ifndef  VALVE_GLOBAL
extern Valve g_stDevValve1;
#endif /* VALVE_GLOBAL */



/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VALVE_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
