/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Valve.c
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 15:23:15:286
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/2/19 15:23:15:286
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  VALVE_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdio.h>
#include "Sprayer\Valve.h"
    
/* Private macro&definde------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static void _Valve_Init(void *valve, void *dev);
static void _Valve_Disable(void *dev);
static void _Valve_Enable(void *dev);
static void _Valve_StateSwitch(Valve *this, EN_StateSwitch_Type stat);




/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    
/* Public variables ----------------------------------------------------------*/
Dev stValveDev = {
    NULL,
    _Valve_Init,
    _Valve_Disable,
    _Valve_Enable
};


Valve g_stDevValve1 = {
    &stValveDev,
    
    RCC_APB2Periph_GPIOA,
    GPIO_Pin_9,
    GPIOA,
    
    _Valve_StateSwitch

};

static void _Valve_Init(void *valve, void *dev)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    Valve *pValve = (Valve *)valve;
    Dev *pDev = (Dev *)dev;
    
    pDev->child = (void *)pValve;

    /* GPIO clock enable */
    RCC_APB2PeriphClockCmd(pValve->gpioCLK, ENABLE);

    /* Switch  PA9 configuration */
    GPIO_InitStructure.GPIO_Pin =  pValve->gpioPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    GPIO_Init(pValve->gpioPort, &GPIO_InitStructure);   // Initialize GPIO A    
}

static void _Valve_Disable(void *dev)
{
    //Dev *pDev = (Dev *)dev;
}

static void _Valve_Enable(void *dev)
{
    //Dev *pDev = (Dev *)dev;
}


static void _Valve_StateSwitch(Valve *this, EN_StateSwitch_Type stat)
{ 
	if(ON == stat)
	{
        GPIO_SetBits(this->gpioPort, this->gpioPin);
	}
    else
    {
        GPIO_ResetBits(this->gpioPort, this->gpioPin);
    }
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
