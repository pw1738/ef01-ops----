/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : main.h
 Author      : ranwei
 Version     : 
 Date        : 2016/3/17 14:16:40:54
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/17 14:16:40:54
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

#ifdef  MAIN_GLOBAL
#define MAIN_EXT
#else
#define MAIN_EXT extern
#endif /* MAIN_GLOBAL */

/*============================================================================*/
/*                                  @INCLUDES                                 */
/*============================================================================*/
#include "Sprayer\Sprayer.h"
#include "LiquidCapacity\LiquidCapacity.h"


/*============================================================================*/
/*                              @MACROS & @TYPEDEFS                           */
/*============================================================================*/
#define FlowRate_M2_UNIT  20  /*!< 流量, mL/m2,  */
#define FarmOperator_LENGTH_MAX 3 /*!< 喷幅, m  */

#pragma pack(push, 1)

typedef enum{
    EN_Result_Positive = 0x00,
    EN_Result_BadReq = 0x10,
    EN_Result_PRECONDITION_NOT_FULFILLED = 0x21,
    EN_Result_BUSY = 0x22,
    EN_Result_HW_Failure = 0x23,
    EN_Result_Other = 0x2F,
}EN_ResultCode_Type;

typedef struct{
    uint8_t ucLiquidResidualCapacity;/*!< 剩余药量, percentage */
    Sprayer *pstSprayer;  
    uint8_t ucResultCode;
    uint16_t usAircraftAverageVelocity; /*!< 飞机平均速率，mm/s,0~0xFFFF */
    uint16_t usFlowRateDefByTime; /*!< 流量, mL/min */
    uint16_t usMaxFlowRateDefByTime; /*!< 最大流量, mL/min */
    uint16_t usFlowRateDefByArea  ; /*!< 流量, mL/m2,  */
    uint16_t usDroplets; /*!< 喷洒着药量 (droplets/cm2) */
    uint16_t usAtomize; /*!< 喷洒雾化标准(μm) */
    uint16_t usMaxAtomize; /*!< 最大喷洒雾化标准(μm) */

    LiquidCapacity *pstLiquidCapacityServer;
}ST_MainSystem_CB_Type;
#pragma pack(pop)



/*============================================================================*/
/*                               @GLOBAL VIRIABLES                            */
/*============================================================================*/
MAIN_EXT ST_MainSystem_CB_Type g_stMainSystemCB;



/*============================================================================*/
/*                                    @FUNCS                                  */
/*============================================================================*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __MAIN_H__ */
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
