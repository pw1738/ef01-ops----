/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Sprayer.h
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 12:22:39:439
 Description : 
 Others      : 

 History     :
  1.Date         -- 2016/2/19 12:22:39:439
    Author       -- 冉威
    Modification -- 创建文件 
    
*******************************************************************************/
#ifndef __SPRAYER_H__
#define __SPRAYER_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  SPRAYER_GLOBAL
#define SPRAYER_EXT
#else
#define SPRAYER_EXT extern
#endif /* SPRAYER_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include "Sprayer\Pump.h"
#include "Sprayer\Valve.h"
#include "Sprayer\Nozzle.h"



/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
#pragma pack(push, 1)

typedef struct{
    uint8_t status;         
    Valve *valve;
    Nozzle *nozzle;
}ST_FlowChannel;

typedef struct ST_Sprayer{
    Pump *itsPump;
    ST_FlowChannel *astFlowChannelArray[8];
    uint8_t ucIsSprayerOn;
    uint8_t ucFlowLevel;
    uint8_t ucAtomizeLevel;
    union _U_SprayerChannelSwitchState{
        struct _ST_SprayerChannelSwitchState{
            uint8_t bSprayerChannel1SwitchState : 1; /*!< */
            uint8_t bSprayerChannel2SwitchState : 1;
            uint8_t bSprayerChannel3SwitchState : 1;
            uint8_t bSprayerChannel4SwitchState : 1;
            uint8_t bSprayerChannel5SwitchState : 1;
            uint8_t bSprayerChannel6SwitchState : 1;
            uint8_t bSprayerChannel7SwitchState : 1;
            uint8_t bSprayerChannel8SwitchState : 1;            
        }stSprayerChannelSwitchState;
        uint8_t ucSprayerChannelSwitchState;
    }uSprayerChannelSwitchState;


    void (*init)(struct ST_Sprayer *this);
    void (*setAtomize)(struct ST_Sprayer *this, uint8_t level);
    void (*setFlow)(struct ST_Sprayer *this, uint8_t level);
    void (*stateSwitch)(struct ST_Sprayer *this, EN_StateSwitch_Type type);
}Sprayer;
#pragma pack(pop)



/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/
#ifndef  SPRAYER_GLOBAL
extern Sprayer g_stDevSprayer;
#endif /* SPRAYER_GLOBAL */



/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/





#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SPRAYER_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
