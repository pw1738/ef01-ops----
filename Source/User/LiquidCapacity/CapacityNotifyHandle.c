/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : CapacityNotifyHandle.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/21 14:56:37:924
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/21 14:56:37:924
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  CAPACITYNOTIFYHANDLE_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdio.h>
#include "LiquidCapacity\CapacityNotifyHandle.h"
    
/* Private typedef&macro&definde----------------------------------------------*/
typedef struct{
    CapacityNotifyHandle astCapacityNotifyHandleArray[SZ_CapacityNotifyHandle_NUM_Max];
    uint8_t aucHandleUsedFlagArray[SZ_CapacityNotifyHandle_NUM_Max];
}ST_CapacityNotifyHandle_CB_Type;

/* Private variables ---------------------------------------------------------*/
ST_CapacityNotifyHandle_CB_Type g_stCapacityNotifyHandleCB = {0};



/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    

static void _CapacityNotifyHandle_Init(CapacityNotifyHandle* const me) 
{
    if(me != NULL) 
    {
        me->Accept = NULL;
        me->pInstance = NULL;
    }    
}

static void _CapacityNotifyHandle_Cleanup(CapacityNotifyHandle* const me) 
{
    if(me != NULL) 
    {
        me->Accept = NULL;
        me->pInstance = NULL;
    }    
}

CapacityNotifyHandle* CapacityNotifyHandle_Create(void) 
{
    CapacityNotifyHandle* me = NULL;
    uint32_t iLoop;

    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    {
        if(0 == g_stCapacityNotifyHandleCB.aucHandleUsedFlagArray[iLoop])
        {
            me = &g_stCapacityNotifyHandleCB.astCapacityNotifyHandleArray[iLoop];
            g_stCapacityNotifyHandleCB.aucHandleUsedFlagArray[iLoop] = 1;
            break;
        }
    }
    
    if(me != NULL) 
    {
        _CapacityNotifyHandle_Init(me);
    }
    return me;
}

void CapacityNotifyHandle_Destroy(CapacityNotifyHandle* const me)
{
    uint32_t iLoop;
    
    if(me != NULL) 
    {
        _CapacityNotifyHandle_Cleanup(me);
    }

    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    {
        if(me == &g_stCapacityNotifyHandleCB.astCapacityNotifyHandleArray[iLoop])
        {
            g_stCapacityNotifyHandleCB.aucHandleUsedFlagArray[iLoop] = 0;
            break;
        }
    }    
    //free(me);
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
