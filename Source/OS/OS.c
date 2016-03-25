/*******************************************************************************
  (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
  All rights reserved.

  This software is confidential and proprietary to Efficient 
  Systems, Inc.  No part of this software may be reproduced,    
  stored, transmitted, disclosed or used in any form or by any means
  other than as expressly provided by the written license agreement    
  between Efficient Systems and its licensee.
FileName    : OS.c
Author      : ranwei
Version     : 
Date        : 2016/1/21 16:4:42:801
Description : 
Others      : 
 *******************************************************************************/

#define  OS_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"

/* define a EN_OS_Task_ID_Type */
#include "OS_CLASS.h"
#include "OS_Conf.h"

/* extern a periodicFunc list */
#include "OS_CLASS.h"
#include "OS_Conf.h"

/* extern a initFunc list */
#include "OS_CLASS.h"
#include "OS_Conf.h"

/* define a g_astOSTaskArray array */
#include "OS_CLASS.h"
#include "OS_Conf.h"

#include "OS.h"



/* Private macro&definde------------------------------------------------------*/
#define PERIOD_IDLE 0

/* Private typedef -----------------------------------------------------------*/
typedef struct{
    uint32_t uiOSTimeCounter;
    uint32_t uiOSTimeCounterBackup;
    uint32_t uiLastOSTimeCounter;
}ST_OS_CB_Type;


/* Private variables ---------------------------------------------------------*/
ST_OS_CB_Type g_stOSCB = {0};



/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

static void __OS_Clear_TaskStruct(Task *pTask)
{
    pTask->initFunc = NULL;
    pTask->periodicFunc = NULL;
    pTask->mode = EN_TASK_Periodic;
    pTask->period = PERIOD_IDLE;
    pTask->periodBackup = PERIOD_IDLE;    
    pTask->arg = NULL;
}

/*******************************************************************************
 Prototype    : OS_Init
 Description  : os init function
 Input        : 
 Output       : 
 Return Value : 
 Calls        : 
 Called By    : 
 
 History      :
  1.Date         -- 2016/1/21 17:14:12:696
    Author       -- ranwei
    Modification -- Created function

*******************************************************************************/
void OS_Init(void)
{
    int iLoop;
    /* 
       Setup SysTick Timer for 1 msec interrupts.
       ------------------------------------------
       1. The SysTick_Config() function is a CMSIS function which configure:
          - The SysTick Reload register with value passed as function parameter.
          - Configure the SysTick IRQ priority to the lowest value (0x0F).
          - Reset the SysTick Counter register.
          - Configure the SysTick Counter clock source to be Core Clock Source (HCLK).
          - Enable the SysTick Interrupt.
          - Start the SysTick Counter.

       2. You can change the SysTick Clock source to be HCLK_Div8 by calling the
          SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8) just after the
          SysTick_Config() function call. The SysTick_CLKSourceConfig() is defined
          inside the misc.c file.

       3. You can change the SysTick IRQ priority by calling the
          NVIC_SetPriority(SysTick_IRQn,...) just after the SysTick_Config() function 
          call. The NVIC_SetPriority() is defined inside the core_cm4.h file.

       4. To adjust the SysTick time base, use the following formula:

          Reload Value = SysTick Counter Clock (Hz) x  Desired Time base (s)

          - Reload Value is the parameter to be passed for SysTick_Config() function
          - Reload Value should not exceed 0xFFFFFF
       */    
    if (SysTick_Config(SystemCoreClock / OS_PERIOD))
    { 
        /* Capture error */ 
        while (1);
    }    

    for(iLoop = 0; iLoop < OS_TASK_MAX; iLoop++)
    {
        if(g_astOSTaskArray[iLoop].initFunc)
        {
            g_astOSTaskArray[iLoop].initFunc();
        }
    }    
}


void OS_Loop(void)
{
    int iLoop;

    g_stOSCB.uiOSTimeCounterBackup = g_stOSCB.uiOSTimeCounter;
    if(g_stOSCB.uiOSTimeCounterBackup > g_stOSCB.uiLastOSTimeCounter)
    {
        for(iLoop = 0; iLoop < OS_TASK_MAX; iLoop++)
        {
            if(g_astOSTaskArray[iLoop].period != 0)
            {
                if(g_astOSTaskArray[iLoop].period >= g_stOSCB.uiOSTimeCounterBackup - g_stOSCB.uiLastOSTimeCounter)
                {
                    g_astOSTaskArray[iLoop].period -= g_stOSCB.uiOSTimeCounterBackup - g_stOSCB.uiLastOSTimeCounter;
                }
                else
                {
                    g_astOSTaskArray[iLoop].period = 0;
                }
            }

            if(0 == g_astOSTaskArray[iLoop].period)
            {
                if(EN_TASK_OneShot == g_astOSTaskArray[iLoop].mode)
                {
                    if(g_astOSTaskArray[iLoop].periodicFunc)
                    {
                        g_astOSTaskArray[iLoop].periodicFunc(g_astOSTaskArray[iLoop].arg);
                    }
                    __OS_Clear_TaskStruct(&g_astOSTaskArray[iLoop]);
                }
                else
                {
                    if(g_astOSTaskArray[iLoop].periodicFunc)
                    {
                        g_astOSTaskArray[iLoop].periodicFunc(g_astOSTaskArray[iLoop].arg);
                    }
                    g_astOSTaskArray[iLoop].period = g_astOSTaskArray[iLoop].periodBackup;
                }
            }
        } 
        g_stOSCB.uiLastOSTimeCounter = g_stOSCB.uiOSTimeCounter;
    }
}

/*******************************************************************************
 Prototype    : OS_ISR_handler
 Description  : 
 Input        : 
 Output       : 
 Return Value : 
 Calls        : 
 Called By    : 
 
 History      :
  1.Date         -- 2016/1/21 17:14:57:452
    Author       -- ranwei
    Modification -- Created function

*******************************************************************************/
void OS_ISR_handler(void)
{
    g_stOSCB.uiOSTimeCounter++;
}


/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
