/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Task_Commu.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/8 19:48:46:757
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/8 19:48:46:757
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  TASK_COMMU_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "Communication\Commu.h"
    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    
void Task_Commu_Proc(void *arg)
{
    Commu_Proc(arg);
}

void Task_Commu_Init(void)
{
    Commu_Init();
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
