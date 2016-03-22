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
* Author: wangwei
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name      Changes and comments
* V1.0				20151215	wangwei
*=====================================================================*/

/**** System include files ****/
#include <stdint.h>
#include "stm32f10x.h"
#include "main.h"
 
/*---Private include files--------------------------------------------*/
#include "Communication\msg.h"   
#include "Communication\tp_msg.h"   
#include "Communication\tp_msg_if.h"   
#include "Communication\uart_tp_if.h"
#include "Led\Led.h"

/*===VARIABLES========================================================*/

/*---Global-----------------------------------------------------------*/
int spayer_status ;
int pump_status ;

#define SZ_MessageBuff_Max 30
uint8_t g_aucMessageBuff[SZ_MessageBuff_Max] = {0};
#define MessageType_OFFSET 0
#define MessageCompID_OFFSET 1
#define MessageSeq_OFFSET 2
#define MessagePriority_OFFSET 4
#define MessageSize_OFFSET 3
#define MessageDATA_OFFSET 5



int debug1;
/*---Private----------------------------------------------------------*/

/***********************************************************************
*  Name         : tp_uart_msg_send
*  Description : send nav frame to output fifo      
*  Parameter  : 
*  Returns      : 
***********************************************************************/
BOOL uart_msg_send(uint8_t nArgs,uint8_t *pArg)
{ 
	BOOL ret_val = FALSE; 
	uint8_t frame[UART_FRAME_SIZE];
	uart_msg_frame_mk(frame, nArgs, pArg);
	ret_val = uart_send_frame(frame,nArgs);
	
	return ret_val;
}

/****************************************************************************************************
TP UART FRAME:

frame type   |         command    id            |   cmd size   | param1...n
1byte        |  compid(H:8bit)  msgseq(L:8bit)  |   1 byte     | n byte

*****************************************************************************************************/
void uart_msg_frame_mk(uint8_t *frame,uint8_t nArgs,uint8_t *pArg )
{
	uint8_t idx;
	for(idx=0;idx<nArgs;idx++)
	{
		frame[idx] = pArg[idx];
	}
}

/***********************************************************************
*  Name         : uart_msg_handle
*  Description : it is a callback function, only called by "uart_tbox_handle_frame" .     
*  Parameter  : 
*  Returns      : 
***********************************************************************/
void uart_msg_handle(uint8_t *frame)
{
    UART_FRAME  uart_frame;
    uart_msg_frame_decode(frame,&uart_frame);

    if(0x00 == uart_frame.type)
    {
        if((0x02 == uart_frame.msg_id.msgid.compid) && (0x01 == uart_frame.msg_id.msgid.msgseq))
        {
            if(0x00 == *(uart_frame.data1)) /*!< Stop spraying */
            {
                g_stMainSystemCB.pstSprayer->stateSwitch(g_stMainSystemCB.pstSprayer, OFF);
            }
            else if(0x01 == *(uart_frame.data1)) /*!< Start spraying */
            {
                g_stMainSystemCB.pstSprayer->stateSwitch(g_stMainSystemCB.pstSprayer, ON);
            }
            SprayerStateSwitchMessage_Rep_send();
        }   
        else if((0x02 == uart_frame.msg_id.msgid.compid) && (0x02 == uart_frame.msg_id.msgid.msgseq))
        {
            g_stMainSystemCB.usFlowRateDefByTime = (uint16_t)*(uart_frame.data1);
            if(g_stMainSystemCB.usFlowRateDefByTime > g_stMainSystemCB.usMaxFlowRateDefByTime)
            {
                g_stMainSystemCB.usMaxFlowRateDefByTime = g_stMainSystemCB.usFlowRateDefByTime;
            }
            g_stMainSystemCB.usFlowRateDefByArea = (uint16_t)*(uart_frame.data3);
            g_stMainSystemCB.usDroplets = (uint16_t)*(uart_frame.data5);
            g_stMainSystemCB.usAtomize = (uint16_t)*(uart_frame.data7);
            if(g_stMainSystemCB.usAtomize > g_stMainSystemCB.usMaxAtomize)
            {
                g_stMainSystemCB.usMaxAtomize = g_stMainSystemCB.usAtomize;
            }            
            g_stMainSystemCB.pstSprayer->uSprayerChannelSwitchState.ucSprayerChannelSwitchState = (uint8_t)*(uart_frame.data9);
            SprayerInforSetMessage_Rep_send();
        }
    }
    else if(0x03 == uart_frame.type)
    {
        if((0x01 == uart_frame.msg_id.msgid.compid) && (0x01 == uart_frame.msg_id.msgid.msgseq))
        {
            g_stMainSystemCB.ucResultCode = *(uart_frame.data1);
            g_stMainSystemCB.usAircraftAverageVelocity = *(uart_frame.data2);
        }
    }



    
    if(uart_frame.type == 0)
	{
		if(*(uart_frame.data1))
		{
			//sprayer_on();
			//switch_on();
			//pump_on();
			//led_toggle(LED2);
		}
		else{
			//sprayer_off();
			//switch_off();
			//pump_off();
			//led_ensti(LED2);
				};
		  //set_pump_rate(*(uart_frame.data2),*(uart_frame.data3));
		  //set_sprayer_rate(*(uart_frame.data4),*(uart_frame.data5));
		  //pwm_timer_config(*(uart_frame.data3),*(uart_frame.data5));
    }
	
	
	
// 	switch (uart_frame.msg_id.msgid.compid)
// 	{
// 		case TBOX_UART_DEV_MANAGE_SERVEICE:
// 			//tbox_uart_msg_device_manage_handler(&tbox_uart_frame);
// 			break;
// 		case TBOX_UART_VEHICLE_ACCESS_SERVICE:
// 			//tbox_uart_msg_config_manage_handler(&tbox_uart_frame);
// 			break;
// 		case TBOX_UART_3G_GPS_SERVICE:

// 			break;
// 		#ifdef TBOX_SW_UPGRADE_OPTION
// 		case TBOX_UART_FIRMWARE_UPGRADE_SERVICE:
// 			//tbox_uart_msg_firmware_upgrade_handler(&tbox_uart_frame);
// 			break;
// 		#endif
// 		
// 		default:
// 			break;
// 	}
}

/****************************************************************************************************
TBOX UART FRAME:

frame type   |         command    id            |   cmd size   | param1...n
1byte        |  compid(H:8bit)  msgseq(L:8bit)  |   1 byte     | n byte

*****************************************************************************************************/
void uart_msg_frame_decode(uint8_t *frame, UART_FRAME  *uart_frame)
{
	(*uart_frame).type = *(frame+TBOX_CMD_TYPE);
	(*uart_frame).msg_id.msg_id = (((uint16_t)*(frame+TBOX_CMD_ID_H))<<8) + (uint16_t)*(frame+TBOX_CMD_ID_L);
	(*uart_frame).size =*(frame+TBOX_CMD_SIZE);
	(*uart_frame).option = *(frame+TBOX_CMD_OPTION);
	(*uart_frame).param = (uint8_t*)(frame+TBOX_CMD_PARAM_START);
	(*uart_frame).data1 = (uint8_t*)(frame+TBOX_CMD_DATA_1);
	(*uart_frame).data2 = (uint8_t*)(frame+TBOX_CMD_DATA_2);
	(*uart_frame).data3 = (uint8_t*)(frame+TBOX_CMD_DATA_3);
	(*uart_frame).data4 = (uint8_t*)(frame+TBOX_CMD_DATA_4);
	(*uart_frame).data5 = (uint8_t*)(frame+TBOX_CMD_DATA_5);
}




static void _Packet_Message(uint8_t messageType, uint8_t compID, uint8_t messageSeq, uint8_t messagePro, uint8_t *pucData, uint8_t dataLen)
{
    int iLoop;
    g_aucMessageBuff[MessageType_OFFSET] = messageType;
    g_aucMessageBuff[MessageCompID_OFFSET] = compID;
    g_aucMessageBuff[MessageSeq_OFFSET] = messageSeq;
    g_aucMessageBuff[MessagePriority_OFFSET] = messagePro;
    g_aucMessageBuff[MessageSize_OFFSET] = dataLen+1;

    for(iLoop = 0; iLoop < dataLen; iLoop++)
    {
        g_aucMessageBuff[MessageDATA_OFFSET+iLoop] = pucData[iLoop];
    }
}

void SprayerStateSwitchMessage_Rep_send(void)
{
    uint8_t ucResultCode = EN_Result_Positive;
    _Packet_Message(0x02, 0x02, 0x01, 0x03, &ucResultCode, 1);
    uart_msg_send(6, g_aucMessageBuff);
}

void SprayerInforSetMessage_Rep_send(void)
{
    uint8_t ucResultCode = EN_Result_Positive;
    _Packet_Message(0x02, 0x02, 0x02, 0x03, &ucResultCode, 1);
    uart_msg_send(6, g_aucMessageBuff);
}

/***********************************************************************
*  Name         : heartbeat_send
*  Description : it is a callback function, only called by "uart_tbox_handle_frame" .     
*  Parameter  : 
*  Returns      : 
***********************************************************************/
void heartbeat_send(void)
{
    uint8_t aucDataBuff[4] = {0};
    aucDataBuff[0] = g_stMainSystemCB.ucLiquidResidualCapacity;
    aucDataBuff[1] = g_stMainSystemCB.ucLiquidResidualCapacity;
	aucDataBuff[2] = spayer_status|(pump_status<1);
    aucDataBuff[3] = g_stMainSystemCB.pstSprayer->ucIsSprayerOn;
    _Packet_Message(0x01, 0x01, 0x01, 0x03, aucDataBuff, 4);

    
    //heartbeat_msg[5] = g_stMainSystemCB.ucLiquidResidualCapacity;
	//heartbeat_msg[7] = spayer_status|(pump_status<1);
    //heartbeat_msg[8] = g_stMainSystemCB.pstSprayer->ucIsSprayerOn;
	uart_msg_send(9, g_aucMessageBuff);
}


/***********************************************************************
*  Name         : resp_msg_send
*  Description : it is a callback function, only called by "uart_tbox_handle_frame" .     
*  Parameter  : 
*  Returns      : 
***********************************************************************/
void resp_msg_send(void)
{
	uart_msg_send(8, response_msg);
}

/***********************************************************************
*  Name         : set_spayer_status
*  Description : it is a callback function, only called by "uart_tbox_handle_frame" .     
*  Parameter  : 
*  Returns      : 
***********************************************************************/
void set_spayer_status(int a)
{
	spayer_status = a ;
}

/***********************************************************************
*  Name         : set_pump_status
*  Description : it is a callback function, only called by "uart_tbox_handle_frame" .     
*  Parameter  : 
*  Returns      : 
***********************************************************************/
void set_pump_status(int a)
{
	pump_status = a ;
}

/**************** END OF FILE *****************************************/

