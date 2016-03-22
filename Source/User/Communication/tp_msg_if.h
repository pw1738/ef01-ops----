/***********************************************************************
*   Copyright (c) 2015, Shenzhen YIFEIXING Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   :            										   *
************************************************************************
* Object        : 
* Module        : 
* Instance      : 
* Description   : 
*-----------------------------------------------------------------------
* Version: 
* Date: 
* Author: WangWei
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name      Changes and comments
* V1.0				20151215	WangWei
*=====================================================================*/
#ifndef _TP_MSG_IF_H_
#define _TP_MSG_IF_H_ 

#include <stdint.h>
#include "stm32f10x.h"


/**** Definition of types ****/ 

typedef union
{
    struct
    {
        uint8_t msgseq;
        uint8_t compid;
    }msgid;
    uint16_t msg_id;
} MSG_ID;

typedef struct 
{
    uint8_t type;
    MSG_ID msg_id;
    uint8_t size;
    uint8_t option;
    uint8_t *param;
	
    uint8_t *data1;
    uint8_t *data2;
    uint8_t *data3;
    uint8_t *data4;
    uint8_t *data5;
    uint8_t *data6;
    uint8_t *data7;
    uint8_t *data8;
    uint8_t *data9;
} UART_FRAME;


/**** Definition of macros ****/
enum TBOX_COMPONENT_ID
{
	TBOX_UART_DEV_MANAGE_SERVEICE = 0x01,
	TBOX_UART_3G_GPS_SERVICE,
	TBOX_UART_VEHICLE_ACCESS_SERVICE,
	TBOX_UART_CONFIG_SERVICE,
	TBOX_UART_DIAGNOSE_SERVICE,
	TBOX_UART_DEBUG_SERVICE,	
#ifdef TBOX_SW_UPGRADE_OPTION
	TBOX_UART_FIRMWARE_UPGRADE_SERVICE = 0x07,
#endif	/*TBOX_SW_UPGRADE_OPTION*/
};



/**** Definition of macros ****/


/**** Declaration of constants ****/


/**** Declaration of variables ****/


/**** Declaration of functions ****/
extern BOOL uart_msg_send(uint8_t nArgs,uint8_t *pArg);

#endif /*_TP_MSG_IF_H_*/
/****************************** END OF FILE ***************************/
