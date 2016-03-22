/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Pump.c
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 14:19:56:981
 Description : 
 Others      : 
 
 History     :
  1.Date         -- 2016/2/19 14:19:56:981
    Author       -- 冉威
    Modification -- 创建文件 
    
*******************************************************************************/
    
#define  PUMP_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "pwm.h"
#include "Sprayer\Pump.h"

    
/* Private macro&definde------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
static void _Pump_Init(void *pump, void *dev);
static void _Pump_Disable(void *dev);
static void _Pump_Enable(void *dev);
static void _Pump_Set(Pump *this, uint8_t level);





/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/
Dev stPumDev = {
    NULL,
    _Pump_Init,
    _Pump_Disable,
    _Pump_Enable
};

Pump g_stDevPump = {
    &stPumDev,
    RCC_APB2Periph_TIM1,
    RCC_APB2Periph_GPIOA,
    GPIO_Pin_8,
    GPIOA,
    TIM1,
    1,

    _Pump_Set
};


static void _Pump_Init(void *pump, void *dev)
{
    Pump *pPump = (Pump *)pump;
    Dev *pDev = (Dev *)dev;
    
    pDev->child = (void *)pPump;
    //PWM_Init(pPump->timX, pPump->timerCLK, pPump->gpioCLK, pPump->gpioPin, pPump->gpioPort);
    //PWM_Set(TIM1, 1, 100);
}

static void _Pump_Disable(void *dev)
{
    //Dev *pDev = (Dev *)dev;
}

static void _Pump_Enable(void *dev)
{
    //Dev *pDev = (Dev *)dev;
}


static void _Pump_Set(Pump *this, uint8_t level)
{ 
    if(level > 100)
    {
        return;
    }
    
    //PWM_Set(this->timX, this->timChNum, level);
    //PWM_Set(TIM1, 1, 100);
    //PWM_Set(TIM1, 1, 100);
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
