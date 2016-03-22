/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Nozzle.c
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 15:24:41:750
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/2/19 15:24:41:750
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  NOZZLE_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "pwm.h"
#include "Sprayer\Nozzle.h"
    
/* Private macro&definde------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


/* Private functions ---------------------------------------------------------*/
static void _Nozzle_Init(void *nozzle, void *dev);
static void _Nozzle_Disable(void *dev);
static void _Nozzle_Enable(void *dev);
static void _Nozzle_Set(Nozzle *this, uint8_t level);



/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    
/* Public variables ----------------------------------------------------------*/
Dev stNozzleDev = {
    NULL,
    _Nozzle_Init,
    _Nozzle_Disable,
    _Nozzle_Enable
};

Nozzle g_stDevNozzle1 = {
    &stNozzleDev,
    
    RCC_APB2Periph_TIM1,
    RCC_APB2Periph_GPIOA,
    GPIO_Pin_10,
    GPIOA,
    TIM1,
    3,
    
    _Nozzle_Set
};


static void _Nozzle_Init(void *nozzle, void *dev)
{
    Nozzle *pNozzle = (Nozzle *)nozzle;
    Dev *pDev = (Dev *)dev;
    
    pDev->child = (void *)pNozzle;
    //PWM_Init(pNozzle->timX, pNozzle->timerCLK, pNozzle->gpioCLK, pNozzle->gpioPin, pNozzle->gpioPort);
}

static void _Nozzle_Disable(void *dev)
{
    //Dev *pDev = (Dev *)dev;
}

static void _Nozzle_Enable(void *dev)
{
    //Dev *pDev = (Dev *)dev;
}



static void _Nozzle_Set(Nozzle *this, uint8_t level)
{ 
    if(level > 100)
    {
        return;
    }
    
	//PWM_Set(this->timX, this->timChNum, level);
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
