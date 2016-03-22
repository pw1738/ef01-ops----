/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : LiquidCapacity.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/21 14:4:10:385
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/21 14:4:10:385
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  LIQUIDCAPACITY_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdio.h>
#include "LiquidCapacity\LiquidCapacity.h"
#include "LiquidCapacity\FDC1004.h"

    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
LiquidCapacity g_stLiquidCapacity;


/* Private functions ---------------------------------------------------------*/
/* Operations */
static void _LiquidCapacity_DumpList(LiquidCapacity* const me);
static void _LiquidCapacity_NewData(LiquidCapacity* const me, float level);
static void _LiquidCapacity_Notify(LiquidCapacity* const me);
static void _LiquidCapacity_Subscribe(LiquidCapacity* const me, void * instancePtr, CapacityData_Acceptor_Func_Type Accept);
static void _LiquidCapacity_UnSubscribe(LiquidCapacity* const me, CapacityData_Acceptor_Func_Type Accept);
static CapacityData* _LiquidCapacity_GetItsCapData(const LiquidCapacity* const me);
static void _LiquidCapacity_SetItsCapData(LiquidCapacity* const me, CapacityData* pstCapData);
static CapacityNotifyHandle* _LiquidCapacity_GetItsCapNH(const LiquidCapacity* const me, void *pInstance);
static void _LiquidCapacity_AddItsCapNH(LiquidCapacity* const me, CapacityNotifyHandle * pHandle);
static void _LiquidCapacity_RemoveItsCapNH(LiquidCapacity* const me, CapacityNotifyHandle * pHandle);
static void _LiquidCapacity_ClearItsCapNH(LiquidCapacity* const me);
static void _LiquidCapacity_ModuleProc(LiquidCapacity* const me, void *pArg);



/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/



static void _LiquidCapacity_Init(LiquidCapacity* const me)
{
    int iLoop;
    me->itsCapData = NULL;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop) 
    {
    	me->itsCapNH[iLoop] = NULL;
    }
    me->ModuleProc = _LiquidCapacity_ModuleProc;
    me->Subscribe = _LiquidCapacity_Subscribe;
    me->UnSubscribe = _LiquidCapacity_UnSubscribe;
}


static void _LiquidCapacity_Cleanup(LiquidCapacity* const me)
{
    int iLoop;
    me->itsCapData = NULL;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop) 
    {
    	me->itsCapNH[iLoop] = NULL;
    }
    me->ModuleProc = NULL;
    me->Subscribe = NULL;
    me->UnSubscribe = NULL;    
}

static void _LiquidCapacity_ModuleProc(LiquidCapacity* const me, void *pArg)
{
    _LiquidCapacity_NewData(me, FDC1004_GetLiquidLevel());
}

static void _LiquidCapacity_DumpList(LiquidCapacity* const me)
{
    int iLoop;

    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    {
        if (me->itsCapNH[iLoop] != NULL) 
        {
            if (me->itsCapNH[iLoop]->Accept != NULL) 
            {
            }
        }
    }
}

static void _LiquidCapacity_NewData(LiquidCapacity* const me, float level)
{
    if(NULL == me->itsCapData) 
    {
    	me->itsCapData = CapacityData_Create();
    }
    
    if(me->itsCapData->level != level)
    {
        me->itsCapData->level = level;
        _LiquidCapacity_Notify(me);
    }
}

static void _LiquidCapacity_Notify(LiquidCapacity* const me)
{
    int iLoop;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    {    
        if(me->itsCapNH[iLoop] != NULL)
        {
            if(me->itsCapNH[iLoop]->Accept != NULL)
            {
                me->itsCapNH[iLoop]->Accept(me->itsCapNH[iLoop]->pInstance, me->itsCapData); 
            }
        }
    }
}
static void _LiquidCapacity_Subscribe(LiquidCapacity* const me, void * pInstance, CapacityData_Acceptor_Func_Type Accept)
{
    CapacityNotifyHandle* pstCapNHand;   
    
    pstCapNHand = CapacityNotifyHandle_Create();
    pstCapNHand->pInstance = pInstance;
    pstCapNHand->Accept = Accept;
    _LiquidCapacity_AddItsCapNH(me, pstCapNHand); 
}

static void _LiquidCapacity_UnSubscribe(LiquidCapacity* const me, CapacityData_Acceptor_Func_Type Accept)
{
    int iLoop;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    {    
    	if (me->itsCapNH[iLoop]) 
    	{
    		if (me->itsCapNH[iLoop]->Accept == Accept) 
            {
    			CapacityNotifyHandle_Destroy(me->itsCapNH[iLoop]);
    			me->itsCapNH[iLoop] = NULL;
    		}
    	}
    }
}

static CapacityData* _LiquidCapacity_GetItsCapData(const LiquidCapacity* const me)
{
    return (CapacityData *)me->itsCapData;
}    


static void _LiquidCapacity_SetItsCapData(LiquidCapacity* const me, CapacityData* pstCapData)
{
    me->itsCapData = pstCapData;
}


static CapacityNotifyHandle* _LiquidCapacity_GetItsCapNH(const LiquidCapacity* const me, void *pInstance)
{
    CapacityNotifyHandle *pHandle = NULL;
    int iLoop;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    {    
    	if (me->itsCapNH[iLoop]) 
    	{
    		if (pInstance == me->itsCapNH[iLoop]->pInstance) 
            {
    			pHandle = me->itsCapNH[iLoop];
                break;
    		}
    	}
    }    

    return pHandle;
}


static void _LiquidCapacity_AddItsCapNH(LiquidCapacity* const me, CapacityNotifyHandle * pHandle)
{
    int iLoop;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    { 
    	if (NULL == me->itsCapNH[iLoop]) 
        {
    		me->itsCapNH[iLoop] = pHandle;
    		break;
    	}
    }
}


static void _LiquidCapacity_RemoveItsCapNH(LiquidCapacity* const me, CapacityNotifyHandle * pHandle)
{
    int iLoop;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    { 
    	if (pHandle == me->itsCapNH[iLoop]) 
        {
    		me->itsCapNH[iLoop] = NULL;
    		break;
    	}
    }    
}



static void _LiquidCapacity_ClearItsCapNH(LiquidCapacity* const me)
{
    int iLoop;
    
    for(iLoop = 0; iLoop < SZ_CapacityNotifyHandle_NUM_Max; ++iLoop)  
    { 
        me->itsCapNH[iLoop] = NULL;
    }     
}

LiquidCapacity * LiquidCapacity_Create(void) 
{
    LiquidCapacity* me = &g_stLiquidCapacity;
    if(me != NULL) 
    {
        _LiquidCapacity_Init(me);
    }
    return me;
}


void LiquidCapacity_Destroy(LiquidCapacity* const me) 
{
    if(me != NULL) 
    {
        _LiquidCapacity_Cleanup(me);
    }
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
