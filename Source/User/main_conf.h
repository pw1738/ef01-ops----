/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by USC Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to USC 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between USC Systems and its licensee.
    
FileName    : main_conf.h
Author      : ranwei    
Version     : 
Date        : 2015/3/26 15:27:50
Description : 
Others      : 
*******************************************************************************/

#ifndef __MAIN_CONF_H__
#define __MAIN_CONF_H__

#ifdef __cplusplus
extern "C"{
#endif

#ifdef  MAIN_CONF_GLOBAL
#define MAIN_CONF_EXT
#else
#define MAIN_CONF_EXT extern
#endif

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include "stm32f10x.h"



/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
/*
** @group : System Version
** @{
*/
#define __CONCAT_VERSION(main, sub1, sub2, rc) main##"."##sub1##"."##sub2##"-"##rc
#define CONCAT_VERSION(main, sub1, sub2, rc) __CONCAT_VERSION(main, sub1, sub2, rc)
#define __SYSTEM_VERSION_MAIN   "0" /* main version */                                  
#define __SYSTEM_VERSION_SUB1   "0" /* sub1 version */
#define __SYSTEM_VERSION_SUB2   "0" /* sub2 version */
#define __SYSTEM_VERSION_RC     "R02.20160318" /*!  release candidate */ 
#define __SYSTEM_VERSION        CONCAT_VERSION(__SYSTEM_VERSION_MAIN, __SYSTEM_VERSION_SUB1, __SYSTEM_VERSION_SUB2, __SYSTEM_VERSION_RC)
/*
** @}
*/
//#define USE_DHCP       /* enable DHCP, if disabled static address is used*/

/* retarget the printf to com */
//#define SERIAL_DEBUG 





/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/

/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/


#ifdef __cplusplus
}
#endif

#endif 
/******************* (C) COPYRIGHT 2010-2018 USC *****END OF FILE**************/
