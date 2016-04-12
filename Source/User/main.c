/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : main.c
 Author      : ranwei
 Version     : 
 Date        : 2016/1/25 19:12:14:221
 Description : 
 Others      : 
*******************************************************************************/
    
#define  MAIN_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include "main_conf.h"
#include <stdio.h>
#include "serial_debug\serial_debug.h"
#include "OS.h"
#include "main.h"

    
/* Private typedef&macro&definde----------------------------------------------*/
#ifdef SERIAL_DEBUG
    #define __MAIN_PROMPT__
    #define __MAIN_ASSERT__
#endif


/* Private  -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/



    

static void _Main_Prompt(int line, const char *func, const char *format, ...)
#ifdef __MAIN_PROMPT__
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


static void _Main_Assert(int line, const char *func, const char *format, ...)
#ifdef __MAIN_ASSERT__
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


void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;   // sys_timer
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;//TIM1中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
    NVIC_Init(&NVIC_InitStructure);//中断初始化
            
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void MainSystemCB_Var_Init(void)
{
    g_stMainSystemCB.ucResultCode = EN_Result_Positive;
    g_stMainSystemCB.ucLiquidResidualCapacity = 0;
    g_stMainSystemCB.pstSprayer = &g_stDevSprayer;
    g_stMainSystemCB.usAircraftAverageVelocity = 0;
    g_stMainSystemCB.usFlowRateDefByTime = 0; /*!< 流量, mL/min */
    g_stMainSystemCB.usFlowRateDefByArea = 0; /*!< 流量, mL/m2 */
    g_stMainSystemCB.usDroplets = 0; /*!< 喷洒着药量 (droplets/cm2) */
    g_stMainSystemCB.usAtomize = 0; /*!< 喷洒雾化标准(μm) */    

    g_stMainSystemCB.pstLiquidCapacityServer = LiquidCapacity_Create();
}



/*******************************************************************************
** @Fn     :
**
** @Brief  : At this stage the microcontroller clock setting is already 
**           configured to 168 MHz, this is done through SystemInit() function 
**           which is called from startup file (startup_stm32f4xx.s) before to 
**           branch to application main. To reconfigure the default setting of 
**           SystemInit() function, refer to system_stm32f4xx.c file
**
** @Param  :
**    <input>
**            none.
**    </input>
**
**    <output>
**            none.
**    </output>
**
** @Return :
**
** @Note   :
*******************************************************************************/  
int main(void)
{        
    //NVIC_SetVectorTable(0x8000000, 0x40000);	
    NVIC_Configuration();
#ifdef SERIAL_DEBUG
    DebugComPort_Init();
    printf("------------------------------------------------------------------------------------------------");printf("\n");
    printf("                                                                                                ");printf("\n");
    printf("  ***    *****   *   *    ***     ***    *****      *    *   *   *   *            ***     ***   ");printf("\n");
    printf(" *   *     *     ** **   *   *   *   *   *         **    *   *   *   *           *   *   *   *  ");printf("\n");
    printf(" *         *     * * *       *       *   *        * *     * *     * *            *   *   *      ");printf("\n");
    printf("  ***      *     * * *     **      **    ****    *  *      *       *     *****   *   *    ***   ");printf("\n");
    printf("     *     *     *   *       *    *      *       *****    * *     * *            *   *       *  ");printf("\n");
    printf(" *   *     *     *   *   *   *   *       *          *    *   *   *   *           *   *   *   *  ");printf("\n");
    printf("  ***      *     *   *    ***    *****   *          *    *   *   *   *            ***     ***   ");printf("\n");
    printf("                                                                                                ");printf("\n");
    printf("                                                     %s", __SYSTEM_VERSION);printf("\n");
    printf("------------------------------------------------------------------------------------------------");printf("\n");
#endif
    _Main_Prompt(__LINE__, __FUNCTION__, "System start");
    MainSystemCB_Var_Init();
    
    OS_Init();     
    //g_stMainSystemCB.pstSprayer->stateSwitch(g_stMainSystemCB.pstSprayer, ON);

    for(;;)
    {
        OS_Loop();
    }
}




#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
