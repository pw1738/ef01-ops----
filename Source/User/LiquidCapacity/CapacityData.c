/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : CapacityData.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/21 14:10:2:301
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/21 14:10:2:301
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  CAPACITYDATA_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdio.h>
#include "LiquidCapacity\CapacityData.h"


    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
CapacityData g_stCapacityData;

/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    
static void _CapacityData_Init(CapacityData* const me) 
{
    if(me != NULL)
    {
        me->level = 0;
    }
}

static void _CapacityData_Cleanup(CapacityData* const me) 
{
    if(me != NULL)
    {
        me->level = 0;
    }    
}

CapacityData* CapacityData_Create(void) 
{
    CapacityData* me = &g_stCapacityData;
    //CapacityData* me = (CapacityData *) malloc(sizeof(CapacityData));
    if(me != NULL) 
    {
        _CapacityData_Init(me);
    }
    return me;
}

void CapacityData_Destroy(CapacityData* const me)
{
    if(me != NULL) 
    {
        _CapacityData_Cleanup(me);
    }
    //free(me);
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
