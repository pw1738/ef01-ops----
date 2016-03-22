/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : OS_CLASS.H
 Author      : ranwei
 Version     : 
 Date        : 2016/1/21 20:12:31:704
 Description : 
 Others      : 
*******************************************************************************/
#ifndef __OS_CLASS_H__
#define __OS_CLASS_H__

#define OS_PASS_1

#endif /* __OS_CLASS_H__ */


#ifdef OS_PASS_4
    #undef  OS_PASS_4
    #undef  BEGIN_OS
    #undef  END_OS
    #undef  OS_TASK
    #define BEGIN_OS    Task g_astOSTaskArray[OS_TASK_MAX] = {
    #define END_OS       };
    #define OS_TASK(taskID, periodicFunc, initFunc, mode, period, periodBackup, arg)   {periodicFunc, initFunc, mode, period, periodBackup, arg},
#endif

#ifdef OS_PASS_3
    #undef  OS_PASS_3
    #define OS_PASS_4
    #undef  BEGIN_OS
    #undef  END_OS
    #undef  OS_TASK
    #define BEGIN_OS    
    #define END_OS      
    #define OS_TASK(taskID, periodicFunc, initFunc, mode, period, periodBackup, arg)   extern void initFunc(void);
#endif

#ifdef OS_PASS_2
    #undef  OS_PASS_2
    #define OS_PASS_3
    #undef  BEGIN_OS
    #undef  END_OS
    #undef  OS_TASK
    #define BEGIN_OS    
    #define END_OS      
    #define OS_TASK(taskID, periodicFunc, initFunc, mode, period, periodBackup, arg)   extern void periodicFunc(void *);
#endif

#ifdef OS_PASS_1
    #undef OS_PASS_1
    #define OS_PASS_2
    #define BEGIN_OS        typedef enum  {
    #define END_OS          OS_TASK_MAX }EN_OS_Task_ID_Type;
    #define OS_TASK(taskID, periodicFunc, initFunc, mode, period, periodBackup, arg)   taskID,
#endif

/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
