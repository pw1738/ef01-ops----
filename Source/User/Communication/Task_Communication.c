/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Task_Communication.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/17 11:17:14:117
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/17 11:17:14:117
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  TASK_COMMUNICATION_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "Task_Communication.h"
#include "Communication\uart_tp_if.h"
    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/


void Task_Communication_Init(void)
{
    uart_create();
}

void Task_Communication_Proc(void *arg)
{
    uart_send_polling();   //   send a frame to navi periodically if has frame to be send 
    uart_read_polling();   //
    uart_polling();    
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
