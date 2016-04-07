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




    
/* Private macro&definde------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;



/* Private functions ---------------------------------------------------------*/
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
    //_Sprayer_Init,
    _Sprayer_SetAtomize,
    _Sprayer_SetFlow,
    _Sprayer_StateSwitch
};


TIM_OCInitTypeDef  TIM_OCInitStructure;  
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


static void _Sprayer_Lowlevel_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
      
    /*!< clock enable */ 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);  
    /*!< PCLK1经过2倍频后作为TIM3的时钟源等于72MHz*/  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);       
 
    /*!< GPIOA Configuration: TIM3 channel 1 and 3 as alternate function push-pull */ 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           // 复用推挽输出 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);   

    /*!< Switch  PA9 configuration */
    //GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
    //GPIO_Init(GPIOA, &GPIO_InitStructure);   // Initialize GPIO A      

    /* TIM1 Configuration ---------------------------------------------------
       Generate 7 PWM signals with 4 different duty cycles:
       TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
       SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
       and Connectivity line devices and to 24 MHz for Low-Density Value line and
       Medium-Density Value line devices

       The objective is to generate 7 PWM signal at 16 Hz:
       - TIM1_Period = (SystemCoreClock /((499 + 1)* 16)) - 1
       The channel 1 and channel 1N duty cycle is set to 50%
       The channel 2 and channel 2N duty cycle is set to 37.5%
       The channel 3 and channel 3N duty cycle is set to 25%
       The channel 4 duty cycle is set to 12.5%
       The Timer pulse is calculated as follows:
       - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
       ----------------------------------------------------------------------- */

    /* Compute the value to be set in ARR regiter to generate signal frequency at 16 hz */
    TimerPeriod = (SystemCoreClock / ((499 + 1) * 16) ) - 1;
    /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
    //Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
    Channel1Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);
    /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
    //Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
    Channel2Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);
    /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
    //Channel3Pulse = (uint16_t) (((uint32_t) 25 * (TimerPeriod - 1)) / 100);
    Channel3Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod - 1)) / 100);
    /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
    //Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);
    Channel4Pulse = (uint16_t) (((uint32_t) 0 * (TimerPeriod- 1)) / 100);

    TIM_DeInit(TIM1);
    /* Time Base configuration */
    TIM_TimeBaseStructure.TIM_Prescaler = 499;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1, 2,3 and 4 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
    
    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);    
}


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
void Sprayer_Init(Sprayer *this)
{
    _Sprayer_Lowlevel_Init();
}

static void _Sprayer_PWMSet(uint8_t ucPumpLevel, uint8_t ucNozzleLevel, uint8_t ucWalveLevel)
{

    /* TIM1 Deinit */
    TIM_DeInit(TIM1);
    
    /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
    Channel1Pulse = (uint16_t) (((uint32_t) ucPumpLevel * (TimerPeriod - 1)) / 100);
    /* Compute CCR2 value to generate a duty cycle at 37.5%  for channel 2 and 2N */
    Channel2Pulse = (uint16_t) (((uint32_t) ucWalveLevel * (TimerPeriod - 1)) / 100);
    /* Compute CCR3 value to generate a duty cycle at 25%  for channel 3 and 3N */
    Channel3Pulse = (uint16_t) (((uint32_t) ucNozzleLevel * (TimerPeriod - 1)) / 100);
    /* Compute CCR4 value to generate a duty cycle at 12.5%  for channel 4 */
    Channel4Pulse = (uint16_t) (((uint32_t) 125 * (TimerPeriod- 1)) / 1000);

    /* Time Base configuration */
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel3Pulse;
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
    TIM_OC4Init(TIM1, &TIM_OCInitStructure);

    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);

    /* TIM1 Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);       
}

static void _Sprayer_StateSwitch(struct ST_Sprayer *this, EN_StateSwitch_Type type)
{
    if(ON == type)
    {
        //_Sprayer_SetFlow(this, this->ucFlowLevel);
        //_Sprayer_SetAtomize(this, this->ucAtomizeLevel);
        //this->itsPump->set(this->itsPump, this->ucFlowLevel);
        //this->astFlowChannelArray[0]->nozzle->set(this->astFlowChannelArray[0]->nozzle, this->ucFlowLevel);
        //this->astFlowChannelArray[0]->valve->StateSwitch(this->astFlowChannelArray[0]->valve, ON);
        //PWM_Config(50, 80);
        _Sprayer_PWMSet(this->ucFlowLevel, this->ucAtomizeLevel, 100);
        //_Sprayer_PWMSet(60, 80, 100);
        this->ucIsSprayerOn = ON;
    }
    else /*!< OFF */
    {
        //_Sprayer_SetFlow(this, 0);
        //_Sprayer_SetAtomize(this, 0);  
        if(this->ucIsSprayerOn != OFF)
        {					
            //this->astFlowChannelArray[0]->valve->StateSwitch(this->astFlowChannelArray[0]->valve, OFF);
            //PWM_Disable();
            //PWM_Config(0, 0);
            _Sprayer_PWMSet(0, 0, 0);
            this->ucIsSprayerOn = OFF;    
        }
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
