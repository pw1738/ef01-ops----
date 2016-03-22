/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Task_Led.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/16 10:47:13:56
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/16 10:47:13:56
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  TASK_LED_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "Task_Led.h"
#include "Led.h"
    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/


void Task_Led_Init(void)
{
    Led_Init();

    LED_Switch(EN_LED_LED1, EN_LED_Switch_Blink);
    LED_Switch(EN_LED_LED2, EN_LED_Switch_Blink);
}

void Task_Led_Proc(void *arg)
{
    LED_Loop_Proc();
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/