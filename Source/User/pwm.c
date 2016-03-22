/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : pwm.c
 Author      : ranwei
 Version     : 
 Date        : 2016/2/19 16:9:8:452
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/2/19 16:9:8:452
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  PWM_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "pwm.h"
    
/* Private macro&definde------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/



/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    


void PWM_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //Define the basic configuration struct

    /* TIM clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* GPIO clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /* GPIO Configuration:TIM Channel as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    

    TIM_DeInit(TIM1);    //set timer to default value  
    //TIM_InternalClockConfig(TIM1);     //initial TIM3 clock

    //The basic configuration of timer
    TIM_TimeBaseStructure.TIM_Period =14400;//计数值为1000 1000-1    configure the duty ratio of the pwm
    TIM_TimeBaseStructure.TIM_Prescaler = 499;//3分频  3-1   499
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//采样分频0   TIM_CKD_DIV1
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ARRPreloadConfig(TIM1, ENABLE);     
}


void PWM_Config(unsigned int SprayRate, unsigned int PumpRate)
{ 
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //Define the basic configuration struct
	TIM_OCInitTypeDef  TIM_OCInitStructure;    // Define the channel 
//	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;  //
	
	TIM_DeInit(TIM1);    //set timer to default value  
	//TIM_InternalClockConfig(TIM1);     //initial TIM3 clock
	
	//The basic configuration of timer
	TIM_TimeBaseStructure.TIM_Period =14400;//计数值为1000 1000-1    configure the duty ratio of the pwm
	TIM_TimeBaseStructure.TIM_Prescaler = 499;//3分频  3-1   499
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//采样分频0   TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ARRPreloadConfig(TIM1, ENABLE); 
	
			// PWM1 mode channel 1   Pump
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性高
	//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; //
	TIM_OCInitStructure.TIM_Pulse = 144*PumpRate;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	//TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
		// PWM1 mode channel 3   Sprayer
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//定时器配置为PWM1模式
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	
	//TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性高
	
	//TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High; // 
	TIM_OCInitStructure.TIM_Pulse = 144*SprayRate ;
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	//TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);

	// configure the BDTR
// 	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
// 	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable; 
// 	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
// 	TIM_BDTRInitStructure.TIM_DeadTime = 0x90;
// 	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
// 	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
// 	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
// 	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

// 	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_CC3/*TIM_IT_CC2|TIM_IT_CC3|TIM_IT_Update*/);
// 	TIM_ITConfig(TIM1,/*TIM_IT_Update*/TIM_IT_CC1,ENABLE);  
// 	TIM_ITConfig(TIM1,/*TIM_IT_Update*/TIM_IT_CC3,ENABLE);  

	TIM_Cmd(TIM1, ENABLE);//使能定时器2	
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);  //
	
}

void PWM_Disable(void)
{
    TIM_Cmd(TIM1, DISABLE);
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
