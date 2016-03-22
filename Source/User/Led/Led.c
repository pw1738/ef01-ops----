/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Led.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/16 10:46:12:285
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/16 10:46:12:285
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  LED_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "Led.h"
    
/* Private typedef&macro&definde----------------------------------------------*/
#ifdef SERIAL_DEBUG
    #define __LED_PROMPT__
    #define __LED_ASSERT__
#endif



/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/


static void LED_Prompt(int line, const char *func, const char *format, ...)
#ifdef __LED_PROMPT__
{
    va_list args;

    va_start(args, format);
    __Prompt__(__FILE__, line, func, format, args);
    va_end(args);    
}
#else
{
}
#endif

 
static void LED_Assert(int line, const char *func, const char *format, ...)
#ifdef __LED_ASSERT__
{
    va_list args;

    va_start(args, format);
    __Assert__(__FILE__, line, func, format, args);
    va_end(args);    
}
#else
{
}
#endif


static void _Led_Var_Init(void)
{
    int iLoop;
    for(iLoop = 0; iLoop < SZ_LED_NUM; iLoop++)
    {
        g_stLedCB.aLedSwitchTypeArray[iLoop] = EN_LED_Switch_Off;
    }

    g_stLedCB.bISLed1On = 0;
    g_stLedCB.bISLed2On = 0;
}

void Led_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /*  Configuration  LED1(PB9) LED2(PB8) 	*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_8 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   // Initialize GPIO B    

    _Led_Var_Init();
}


void LED_Switch(uint8_t led, EN_LED_Switch_Type type)
{
    if(led >= SZ_LED_NUM)
    {
        LED_Assert(__LINE__, __FUNCTION__, "invalid paramter led(%d) -_-", led);
    }

    switch(type)
    {
    case EN_LED_Switch_Off:
        if(g_stLedCB.aLedSwitchTypeArray[led] != EN_LED_Switch_Off)
        {
            g_stLedCB.aLedSwitchTypeArray[led] = type;
        }
        break;
        
    case EN_LED_Switch_On:
        if(g_stLedCB.aLedSwitchTypeArray[led] != EN_LED_Switch_On)
        {
            g_stLedCB.aLedSwitchTypeArray[led] = type;
        }        
        break;

    case EN_LED_Switch_Blink:
        if(g_stLedCB.aLedSwitchTypeArray[led] != EN_LED_Switch_Blink)
        {
            g_stLedCB.aLedSwitchTypeArray[led] = type;
        }         
        break;
    default:
        break;
    }    
}

void LED_Loop_Proc(void)
{
    if(EN_LED_Switch_Off == g_stLedCB.aLedSwitchTypeArray[0])
    {
        if(g_stLedCB.bISLed1On != 0)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_9);
            g_stLedCB.bISLed1On = 0;
        }
    }

    if(EN_LED_Switch_On == g_stLedCB.aLedSwitchTypeArray[0])
    {
        if(g_stLedCB.bISLed1On != 1)
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_9);
            g_stLedCB.bISLed1On = 1;
        }
    }   

    if(EN_LED_Switch_Blink == g_stLedCB.aLedSwitchTypeArray[0])
    {
        if(1 == g_stLedCB.bISLed1On)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_9);
            g_stLedCB.bISLed1On = 0;
        }
        else
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_9);
            g_stLedCB.bISLed1On = 1;
        }
    }   

    if(EN_LED_Switch_Off == g_stLedCB.aLedSwitchTypeArray[1])
    {
        if(g_stLedCB.bISLed2On != 0)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_8);
            g_stLedCB.bISLed2On = 0;
        }
    }

    if(EN_LED_Switch_On == g_stLedCB.aLedSwitchTypeArray[1])
    {
        if(g_stLedCB.bISLed2On != 1)
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_8);
            g_stLedCB.bISLed2On = 1;
        }
    }   

    if(EN_LED_Switch_Blink == g_stLedCB.aLedSwitchTypeArray[1])
    {
        if(1 == g_stLedCB.bISLed2On)
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_8);
            g_stLedCB.bISLed2On = 0;
        }
        else
        {
            GPIO_SetBits(GPIOB, GPIO_Pin_8);
            g_stLedCB.bISLed2On = 1;
        }
    }     
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
