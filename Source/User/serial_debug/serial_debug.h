/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by USC Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to USC 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between USC Systems and its licensee.
    
 FileName    : serial_debug.h
 Author      : ranwei    
 Version     : 
 Date        : 2015/3/25 10:16:23
 Description : 
 Others      : 
*******************************************************************************/
#ifndef __SERIAL_DEBUG_H__
#define __SERIAL_DEBUG_H__
    
#ifdef __cplusplus
    extern "C"{
#endif
    
#ifdef  SERIAL_DEBUG_GLOBAL
#define SERIAL_DEBUG_EXT
#else
#define SERIAL_DEBUG_EXT extern
#endif
    
/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include <stdarg.h>
#include "main_conf.h"



/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/

/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/
#ifdef SERIAL_DEBUG
    void DebugComPort_Init(void);
#endif    
void __Prompt__(const char *file, int line, const char *func, const char *format, ...);
void __Assert__(const char *file, int line, const char *func, const char *format, ...);

    
#ifdef __cplusplus
    }
#endif
    
#endif 




/******************* (C) COPYRIGHT 2010-2018 USC *****END OF FILE**************/
