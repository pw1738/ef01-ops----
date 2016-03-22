/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Led.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/16 10:46:54:805
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/16 10:46:54:805
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  LED_GLOBAL
#define LED_EXT
#else
#define LED_EXT extern
#endif /* LED_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdint.h>


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
typedef enum{
    EN_LED_LED1,
    EN_LED_LED2,
    SZ_LED_NUM
}EN_LED_Type;

typedef enum{
    EN_LED_Switch_Off,
    EN_LED_Switch_On,
    EN_LED_Switch_Blink
}EN_LED_Switch_Type;

#pragma pack(push, 1)

typedef struct{
    EN_LED_Switch_Type aLedSwitchTypeArray[SZ_LED_NUM];
    uint8_t bISLed1On : 1; 
    uint8_t bISLed2On : 1;
    uint8_t reserved : 6; 
}ST_Led_CB;

#pragma pack(pop)


/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/
LED_EXT ST_Led_CB g_stLedCB;


/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
void Led_Init(void);
void LED_Switch(uint8_t led, EN_LED_Switch_Type type);
void LED_Loop_Proc(void);



#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LED_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
