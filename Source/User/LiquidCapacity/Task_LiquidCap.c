/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Task_LiquidCap.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/18 11:14:17:189
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/18 11:14:17:189
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  TASK_LIQUIDCAPACITY_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdio.h>
#include "main.h"
#include "LiquidCapacity\FDC1004.h"    

		
/* Private typedef&macro&definde----------------------------------------------*/



/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

void Task_LiquidCap_Init(void)
{
    FDC1004_Init();
}

void Task_LiquidCap_Proc(void *arg)
{
    g_stMainSystemCB.pstLiquidCapacityServer->ModuleProc(g_stMainSystemCB.pstLiquidCapacityServer, NULL);
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
