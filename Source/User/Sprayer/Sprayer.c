/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Sprayer.c
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 12:22:18:737
 Description : 该文件定义了喷雾系统的相关动作和属性。喷雾系统包括3部分(水泵、
               离心喷头、电磁阀)。一套喷雾系统包括一个水泵，多个离心喷头和多个电
               磁阀，其中电磁阀和离心喷头是一一对应的。
 Others      : 
 
 History     :
  1.Date         -- 2016/2/19 12:22:18:737
    Author       -- 冉威
    Modification -- 创建文件 
 
*******************************************************************************/
    
#define  SPRAYER_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"
#include "main_conf.h"
#include "Sprayer\Pump.h"
#include "Sprayer\Valve.h"
#include "Sprayer\Nozzle.h"
#include "Sprayer\Sprayer.h"
#include "pwm.h"

    
/* Private macro&definde------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/
static void _Sprayer_Init(struct ST_Sprayer *this);
static void _Sprayer_SetAtomize(Sprayer *this, uint8_t level);
static void _Sprayer_SetFlow(Sprayer *this, uint8_t level);
static void _Sprayer_StateSwitch(struct ST_Sprayer *this, EN_StateSwitch_Type type);






/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/


/* Public variables ----------------------------------------------------------*/
ST_FlowChannel stFlowChannel1 = {
    OFF,
    &g_stDevValve1,
    &g_stDevNozzle1
};


Sprayer g_stDevSprayer = {
    &g_stDevPump,
    {&stFlowChannel1, NULL, NULL, NULL, NULL, NULL, NULL, NULL},
    0,
    0,
    0,
    {0},
    _Sprayer_Init,
    _Sprayer_SetAtomize,
    _Sprayer_SetFlow,
    _Sprayer_StateSwitch
};




/*******************************************************************************
 Prototype    : _Sprayer_Init
 Description  : 
 Input        : 
 Output       : 
 Return Value : 
 Calls        : 
 Called By    : 
 
 History      :
  1.Date         -- 2016/2/19 14:2:25:35
    Author       -- ranwei
    Modification -- Created function

*******************************************************************************/
static void _Sprayer_Init(struct ST_Sprayer *this)
{
    int iLoop;

    PWM_Init();
    
    if(this->itsPump)
    {
        this->itsPump->dev->init(this->itsPump, this->itsPump->dev);
    }

    for(iLoop = 0; iLoop < 8; iLoop++)
    {
        if(this->astFlowChannelArray[iLoop] != NULL)
        {
            if(this->astFlowChannelArray[iLoop]->valve != NULL)
            {
                this->astFlowChannelArray[iLoop]->valve->dev->init(this->astFlowChannelArray[iLoop]->valve, this->astFlowChannelArray[iLoop]->valve->dev);
            }

            if(this->astFlowChannelArray[iLoop]->nozzle != NULL)
            {
                this->astFlowChannelArray[iLoop]->nozzle->dev->init(this->astFlowChannelArray[iLoop]->nozzle, this->astFlowChannelArray[iLoop]->nozzle->dev);
            }
        }
    }
}

static void _Sprayer_StateSwitch(struct ST_Sprayer *this, EN_StateSwitch_Type type)
{
    if(ON == type)
    {
        //_Sprayer_SetFlow(this, this->ucFlowLevel);
        //_Sprayer_SetAtomize(this, this->ucAtomizeLevel);
        //this->itsPump->set(this->itsPump, this->ucFlowLevel);
        //this->astFlowChannelArray[0]->nozzle->set(this->astFlowChannelArray[0]->nozzle, this->ucFlowLevel);
        this->astFlowChannelArray[0]->valve->StateSwitch(this->astFlowChannelArray[0]->valve, ON);
        PWM_Config(this->ucAtomizeLevel, this->ucFlowLevel);
        this->ucIsSprayerOn = ON;
    }
    else /*!< OFF */
    {
        //_Sprayer_SetFlow(this, 0);
        //_Sprayer_SetAtomize(this, 0);  
        this->astFlowChannelArray[0]->valve->StateSwitch(this->astFlowChannelArray[0]->valve, OFF);
        PWM_Disable();
        this->ucIsSprayerOn = OFF;    
    }
}

/*******************************************************************************
 Prototype    : _Sprayer_SetAtomize
 Description  : 
 Input        : Sprayer -- 当前的喷雾系统
                level -- 雾化等级，0~100，其中0代表关闭雾化电机。
 Output       : 
 Return Value : static
 Calls        : 
 Called By    : 
 
 History      :
  1.Date         -- 2016/2/22 10:46:36:113
    Author       -- ranwei
    Modification -- Created function

*******************************************************************************/
static void _Sprayer_SetAtomize(Sprayer *this, uint8_t level)
{ 
    int iLoop;
    
    if(level > 100)
    {
        return;
    }

    for(iLoop = 0; iLoop < 8; iLoop++)
    {
        if(this->astFlowChannelArray[iLoop] != NULL)
        {
            if(this->astFlowChannelArray[iLoop]->nozzle != NULL)
            {
                this->astFlowChannelArray[iLoop]->nozzle->set(this->astFlowChannelArray[iLoop]->nozzle, level);
            }
        }
    }    
}

/*******************************************************************************
 Prototype    : _Sprayer_SetAtomize
 Description  : 
 Input        : Sprayer -- 当前的喷雾系统
                level -- 流量等级，0~100，其中0代表关闭水泵电机。
 Output       : 
 Return Value : static
 Calls        : 
 Called By    : 
 
 History      :
  1.Date         -- 2016/2/22 10:46:36:113
    Author       -- ranwei
    Modification -- Created function

*******************************************************************************/
static void _Sprayer_SetFlow(Sprayer *this, uint8_t level)
{
    if(level > 100)
    {
        return;
    }
    
    if(this->itsPump)
    {
        this->itsPump->set(this->itsPump, level);
    }
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
