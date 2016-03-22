/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Task_Sprayer.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/18 9:11:3:12
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/18 9:11:3:12
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  TASK_SPRAYER_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "Sprayer\Task_Sprayer.h"
#include "main.h"
    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    
void Task_Sprayer_Init(void)
{
    g_stMainSystemCB.pstSprayer->init(g_stMainSystemCB.pstSprayer);
}

void Task_Sprayer_Proc(void *arg)
{
    g_stMainSystemCB.pstSprayer->ucFlowLevel = g_stMainSystemCB.usFlowRateDefByArea / FlowRate_M2_UNIT * 20;
    g_stMainSystemCB.pstSprayer->ucAtomizeLevel = g_stMainSystemCB.usMaxAtomize * 100 / g_stMainSystemCB.usAtomize;
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
