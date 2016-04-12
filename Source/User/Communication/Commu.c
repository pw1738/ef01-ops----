/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Commu.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/25 20:18:41:442
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/25 20:18:41:442
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  COMMU_GLOBAL

/* includes-------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "main.h"
#include "Communication\Commu.h"
#include "Communication\Commu_Uart.h"
#include "Communication\Fifo.h"


    
/* Private typedef&macro&definde----------------------------------------------*/
#ifdef SERIAL_DEBUG
    #define __Commu_PROMPT__
    #define __Commu_ASSERT__
#endif

#define COMMU_FRAMEDATA_OFFSET_CMDID        0
#define COMMU_FRAMEDATA_OFFSET_FrameSequence       1
#define COMMU_FRAMEDATA_OFFSET_ADDR64       2
#define COMMU_CMD_ADDR_64     0// TBOX_CMD_ID_H        1
#define TBOX_CMD_ID_H        1
#define TBOX_CMD_ID_L        2
#define TBOX_CMD_SIZE        3
#define TBOX_CMD_PRIO		 4
#define TBOX_CMD_OPTION      11
#define TBOX_CMD_PARAM_START 11	
#define TBOX_CMD_ADDR_16   	 8
#define TBOX_UART_BASE_LEN 	 4
#define Commu_Message_OFFSET_TYPE   0
#define Commu_Message_OFFSET_COMPID  1
#define Commu_Message_OFFSET_MSGSEQ  2
#define Commu_Message_OFFSET_SIZE    3
#define Commu_Message_OFFSET_PRIORITY 4
#define Commu_Message_OFFSET_DATA  5

#define SZ_MessageBuff_Max 30

#define SZ_Frame_MAX 30

#define SZ_MSG_OUT_BUF_MAX 1024     /* Input buffer size */

#define SZ_Commu_IN_BUF_ONE_MAX 256     /* Input buffer size */
#define SZ_Commu_IN_BUF_TWO_MAX 512     /* Input buffer size */
#define SZ_Commu_IN_BUF_THREE_MAX 512     /* Input buffer size */
#define SZ_Commu_IN_BUF_FOUR_MAX 256     /* Input buffer size */

#define SZ_Commu_OUT_BUF_ONE_MAX 256     /* Input buffer size */
#define SZ_Commu_OUT_BUF_TWO_MAX 512     /* Input buffer size */
#define SZ_Commu_OUT_BUF_THREE_MAX 512     /* Input buffer size */
#define SZ_Commu_OUT_BUF_FOUR_MAX 256     /* Input buffer size */

#define Commu_SEND_FRAME_INTERVAL_MIN 5/**5*2ms**/
#define Commu_ENTER_TX_TIMEOUT_MAX 50/**50*2ms**/
#define Commu_REQ_SEND_TIMEOUT_MAX 0/**uart don't need synchronism mechanism,so set the value to 0.**/
#define Commu_TX_TIMEOUT_MAX 10/**10*2ms**/
#define Commu_ACK_TIMEOUT_MAX 100/**100*2ms=200ms**/
#define Commu_RX_TIMEOUT_MAX 50/**50*2ms**/

#define Commu_Message_PRIO_NUM 	4	  /*tbox only define 4 priority.*/

#define Commu_REQ_CONNECT_COUNTER_MAX 3
#define Commu_REQ_SEND_TIMEOUT_COUNTER_MAX 5
#define Commu_TX_TIMEOUT_COUNTER_MAX 5
#define Commu_ACK_TIMEOUT_COUNTER_MAX 5
#define Commu_RX_TIMEOUT_COUNTER_MAX 5
#define Commu_FRAME_LOST_COUNTER_MAX 5

typedef enum 
{
    EN_MSG_REQ_ACK_NEEDED,
    EN_MSG_REQ_ACK_NOT_NEEDED,
    EN_MSG_RESPONSE_ACK_NEEDED,
    EN_MSG_RESPONSE_ACK_NOT_NEEDED,
    EN_MSG_ACK_FRAME = 0xFF,
}EN_MSG_Type;


typedef enum
{
    EN_Commu_SENDING_NONE,
    EN_Commu_SENDING_ACK, 
    EN_Commu_SENDING_FRAME,
}EN_Commu_SENDING_Type;

typedef enum
{
    EN_Commu_ACK_WAIT_NONE,
    EN_Commu_ACK_WAITING, 
    EN_Commu_ACK_WAIT_TIMEOUT,
}EN_Commu_ACK_WAIT_STATE_Type;

typedef enum
{
    EN_Commu_ST_IDLE,
    EN_Commu_ST_START, /*Only for tx*/
    EN_Commu_ST_A5,
    EN_Commu_ST_5A,
    EN_Commu_ST_RC, /** RollingCode */
    EN_Commu_ST_LEN,
    EN_Commu_ST_LEN_1,
    EN_Commu_ST_LEN_2,
    EN_Commu_ST_FRAME_TYPE,
    EN_Commu_ST_FRAME_ID,
    EN_Commu_ST_ADDR_64,
    EN_Commu_ST_ADDR_16,
    EN_Commu_ST_TYPE,  /*indicate is ack frame or data frame.*/
    EN_Commu_ST_ACK_CS,
    EN_Commu_ST_DATA_PRE,
    EN_Commu_ST_DATA,
    EN_Commu_ST_DATA_CS,
    EN_Commu_ST_END/*Only for tx*/
}EN_Commu_PROCESS_STATUS_Type;

#pragma pack(push, 1)

typedef struct 
{
    BOOL bReqSendSyncByte;
    BOOL bReqSendAck;
    BOOL bReqSendFrame;
    BOOL bReqRestartCrq;
    uint8_t ucSendType;/*is sending ack or frame or nothing?*/
    uint8_t ucAckWaitState;
    uint8_t ucStatus;/** tx status **/
    uint8_t ucCs; /** cs  =ID^LENGTH^DATA1^DATA2^....^DATAn **/
    uint8_t ucAckID;/** id that just received and need to sent it's ack **/
    uint8_t ucFrameRC;/** Rolling code, value that will be sent in a new frame**/
    uint8_t ucFrameLen; /** data frame len,  not including the len byte **/ 
    uint8_t ucByteIndex;	
    uint8_t ucACKFrameRC;
    uint8_t bIsSendReqACK : 1;
    uint8_t bReserve1 : 7;
}ST_TX_Info_Type;

typedef struct 
{
    uint8_t ucStatus;/** rx status **/
    uint8_t ucCs;/** cs  =ID^LENGTH^DATA1^DATA2^....^DATAn **/
    uint8_t ucLastFrameRC;/** received successfully at last time **/
    uint8_t ucFrameRC;/** Frame Rolling Code, received currentlly **/
    uint8_t ucFrameLen;/** data frame len,  not including the len byte **/ 
    uint8_t ucByteIndex; 
    uint8_t ucMsgType;
}ST_RX_Info_Type;


typedef struct 
{
    BOOL bValid; /** if valid data? **/
    uint8_t ucLength;
    uint8_t aucBuff[SZ_Frame_MAX];   
}ST_Frame_Type;

typedef struct 
{
    uint16_t usTimerCounter;/** Timer = polling period*timer_counter **/
    uint16_t usTimeoutCounter;   
}ST_Commu_Timer_Type;

typedef struct{
    ST_Frame_Type  stOutFrame;
    ST_Frame_Type  stInFrame;
    
    ST_TX_Info_Type  volatile  stTxInfo;
    ST_RX_Info_Type  volatile  stRxInfo;
    
    volatile uint8_t ucTxTimeoutCounter;
    volatile uint8_t ucRxTimeoutCounter;
    
    volatile uint8_t ucRecFrameCounter;
    volatile uint8_t ucRecFrameHandledCounter;
    
    volatile uint8_t ucReqTxTimeoutCounter;
    
    volatile uint8_t ucSendFrameCounter;
    volatile uint8_t ucLostAckCounter;
    volatile uint8_t ucSendFrameHandledCounter;
    volatile uint8_t ucSendFrameIntervalCounter; /* 控制发送周期的计数器 */
        
    ST_Commu_Timer_Type stReqSendTimer;
    ST_Commu_Timer_Type stEnterTxTimer;
    ST_Commu_Timer_Type stTxTimer;
    ST_Commu_Timer_Type stAckTimer;
    ST_Commu_Timer_Type stRxTimer;
    ST_Fifo_Type stInFifo[Commu_Message_PRIO_NUM];
    ST_Fifo_Type stOutFifo[Commu_Message_PRIO_NUM];

    uint8_t aucInBuffOne[SZ_Commu_IN_BUF_ONE_MAX];
    uint8_t aucInBuffTwo[SZ_Commu_IN_BUF_TWO_MAX];
    uint8_t aucInBuffThree[SZ_Commu_IN_BUF_THREE_MAX];
    uint8_t aucInBuffFour[SZ_Commu_IN_BUF_FOUR_MAX];

    uint8_t aucOutBuffOne[SZ_Commu_OUT_BUF_ONE_MAX];
    uint8_t aucOutBuffTwo[SZ_Commu_OUT_BUF_TWO_MAX];
    uint8_t aucOutBuffThree[SZ_Commu_OUT_BUF_THREE_MAX];
    uint8_t aucOutBuffFour[SZ_Commu_OUT_BUF_FOUR_MAX];    
}ST_Commu_CB_Type;

/** Message包头 */
typedef struct 
{
    uint8_t ucType;
    uint8_t ucComponentID;
    uint8_t ucMessageSequence;
    uint8_t ucSize;
    uint8_t ucPriority;
}ST_Comm_MSGHeader_Type;

#define SZ_MSGHEADER_LEN  sizeof(ST_Comm_MSGHeader_Type)


#pragma pack(pop)



/* Private variables ---------------------------------------------------------*/
ST_Commu_CB_Type g_stCommuCB;
uint8_t g_aucMessageBuff[SZ_MessageBuff_Max] = {0};




/* Private functions ---------------------------------------------------------*/
/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/

static void __Commu_SendChar(uint8_t send_char)
{
    Commu_Uart_SendChar(send_char);
}


static void __Commu_RecvChar(uint8_t *recv_char )
{
    Commu_Uart_RecvChar(recv_char);
}


static void _Commu_CLR_Alarm(ST_Commu_Timer_Type *pTimer)
{
    pTimer->usTimerCounter = 0; 
    pTimer->usTimeoutCounter = 0;
}

static void _Commu_CLR_ReqSendAlarm(void)
{
    _Commu_CLR_Alarm(&g_stCommuCB.stReqSendTimer);
}

static void _Commu_CLR_EnterTxAlarm(void)
{
    _Commu_CLR_Alarm(&g_stCommuCB.stEnterTxTimer);
}

static void _Commu_CLR_TxAlarm(void)
{
    _Commu_CLR_Alarm(&g_stCommuCB.stTxTimer);
}

static void _Commu_CLR_AckAlarm(void)
{
    _Commu_CLR_Alarm(&g_stCommuCB.stAckTimer);
}

static void _Commu_CLR_RxAlarm(void)
{
    _Commu_CLR_Alarm(&g_stCommuCB.stRxTimer);
}

static void _Commu_CLR_ReqRestartCrq(void)
{
    g_stCommuCB.stTxInfo.bReqRestartCrq = FALSE;
}

static void _Commu_CLR_All_Send_Alarm(void)
{
    _Commu_CLR_ReqSendAlarm();
    _Commu_CLR_EnterTxAlarm();
    _Commu_CLR_TxAlarm();
}


static void _Commu_Set_TxAlarm(void)
{
    g_stCommuCB.stTxTimer.usTimerCounter = Commu_TX_TIMEOUT_MAX;
}

static void _Commu_Set_EnterTxAlarm(void)
{
    g_stCommuCB.stEnterTxTimer.usTimerCounter = Commu_ENTER_TX_TIMEOUT_MAX;
}

static void _Commu_Set_ReqSendAlarm(void)
{
    /**uart don't need synchronism mechanism,so set the value to 0.**/
    g_stCommuCB.stReqSendTimer.usTimerCounter = 0;//Commu_REQ_SEND_TIMEOUT_MAX;
}

static void _Commu_Set_ReqRestartCrq(void)
{
    g_stCommuCB.stTxInfo.bReqRestartCrq = TRUE;
}

static void _Commu_End_Send(void)
{
    _Commu_CLR_All_Send_Alarm();
}


static BOOL _Commu_Get_ReqRestartCrq(void)
{
    return (g_stCommuCB.stTxInfo.bReqRestartCrq);
}

static void _Commu_Crq_Retry(void)
{
    _Commu_End_Send();
    _Commu_Set_ReqRestartCrq();
}

static BOOL _Commu_IS_TX_Idle(void)
{
    BOOL ret_val = FALSE ;
    if(EN_Commu_ST_IDLE == g_stCommuCB.stTxInfo.ucStatus)
    {
        ret_val = TRUE ;
    }
    return ret_val;
}

static BOOL _Commu_Has_Frame_Rev(void)
{
    BOOL ret_val = FALSE ;
    if(g_stCommuCB.ucRecFrameCounter != g_stCommuCB.ucRecFrameHandledCounter)
    {
        ret_val = TRUE ;
    }
    return ret_val;
}

static BOOL _Commu_Has_Frame_Send(void)
{
    BOOL ret_val = FALSE ;
    if(g_stCommuCB.ucSendFrameCounter != g_stCommuCB.ucSendFrameHandledCounter)
    {
        ret_val = TRUE ;
    }
    return ret_val;
}

static BOOL _Commu_Send_Out_Frame(void)
{
    BOOL ret_val = FALSE;
    //uint8_t ucCs,iLoop;

    if(FALSE == g_stCommuCB.stOutFrame.bValid)/**g_stCommuCB.stOutFrame send completely **/
    {
        if(!Fifo_IS_Empty(&g_stCommuCB.stOutFifo[0]))
        {
            /**get frame to uart_tbox_out_frame**/
            ret_val = Fifo_Output_Frame(&g_stCommuCB.stOutFifo[0], g_stCommuCB.stOutFrame.aucBuff, &g_stCommuCB.stOutFrame.ucLength);
        }
        else if(!Fifo_IS_Empty(&g_stCommuCB.stOutFifo[1]))
        {
            /**get frame to uart_tbox_out_frame**/
            ret_val = Fifo_Output_Frame(&g_stCommuCB.stOutFifo[1], g_stCommuCB.stOutFrame.aucBuff, &g_stCommuCB.stOutFrame.ucLength);
        }
        else if(!Fifo_IS_Empty(&g_stCommuCB.stOutFifo[2]))
        {
            /**get frame to uart_tbox_out_frame**/
            ret_val = Fifo_Output_Frame(&g_stCommuCB.stOutFifo[2], g_stCommuCB.stOutFrame.aucBuff, &g_stCommuCB.stOutFrame.ucLength);
        }
        else if(!Fifo_IS_Empty(&g_stCommuCB.stOutFifo[3]))
        {
            /**get frame to uart_tbox_out_frame**/
            ret_val = Fifo_Output_Frame(&g_stCommuCB.stOutFifo[3], g_stCommuCB.stOutFrame.aucBuff, &g_stCommuCB.stOutFrame.ucLength);
        }

        if(TRUE == ret_val)
        {
            ++g_stCommuCB.stTxInfo.ucFrameRC;
            g_stCommuCB.stOutFrame.bValid = TRUE;
            ret_val = TRUE;

            /*
			ucCs = ++uart_tx_info.frame_id;  // fame_id ++  ??
			ucCs ^= uart_out_frame.length;//cs  =ID^LENGTH^DATA1^DATA2^....^DATAn    To caculate the checksum
			
			for(i=0; i<uart_out_frame.length ;i++)
			{
				ucCs^=uart_out_frame.buff[i];
			}
			uart_out_frame.buff[uart_out_frame.length] = ucCs;
			uart_out_frame.bValid = TRUE;
			ret_val = TRUE;            
			*/
        }
    }
    return ret_val;
}

static void _Commu_Start_Send(void)
{
    if(1)  //?0 == UART_TBOX_IS_TX_FLAG )   
    {
        __Commu_SendChar(0x00);/*not use oxff,because when receive frame,tbox start with 0xff*/

        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_START;
        g_stCommuCB.stTxInfo.bReqSendSyncByte = FALSE;
        g_stCommuCB.ucReqTxTimeoutCounter = 0;
        _Commu_CLR_ReqSendAlarm();
        _Commu_Set_EnterTxAlarm();
    }
}


static void _Commu_Trig_Tx(void)
{  
    if(_Commu_IS_TX_Idle())
    {
        _Commu_Start_Send();
        _Commu_Set_ReqSendAlarm();
        g_stCommuCB.stTxInfo.bReqSendSyncByte = TRUE;
    }
}

static BOOL _Commu_Read_Frame(uint8_t *frame, uint8_t *length)	
{
    if (!Fifo_IS_Empty(&g_stCommuCB.stInFifo[0]))
    {
        return (Fifo_Output_Frame(&g_stCommuCB.stInFifo[0], frame, length));
    }
    else if (!Fifo_IS_Empty(&g_stCommuCB.stInFifo[1]))
    {
        return (Fifo_Output_Frame(&g_stCommuCB.stInFifo[1], frame, length));
    }
    else if (!Fifo_IS_Empty(&g_stCommuCB.stInFifo[2]))
    {
        return (Fifo_Output_Frame(&g_stCommuCB.stInFifo[2], frame, length));
    }
    else if (!Fifo_IS_Empty(&g_stCommuCB.stInFifo[3]))
    {
        return (Fifo_Output_Frame(&g_stCommuCB.stInFifo[3], frame, length));
    }
    else
    {
        return FALSE;
    }
}

static BOOL _Commu_Send_frame(uint8_t *frame,uint8_t length)
{
    BOOL ret_val = FALSE;
    uint8_t ucTxFramePrio;    //The priority of the message   The priority is seted 4 different classes
    /*put data into fifo bace on the priority of the data frame.*/
    ucTxFramePrio = 1;//*(frame + TBOX_CMD_PRIO);   //   TBOX_CMD_PRIO		 		 4   frame[4]  To select the prio according to the frame[4]
    if(Fifo_Input_Frame(&g_stCommuCB.stOutFifo[ucTxFramePrio - 1], (uint8_t *)frame, length))  // send frame to a out_fifo
    {
        g_stCommuCB.ucSendFrameCounter++;
        ret_val = TRUE;
    }
    return ret_val;
}

void _Commu_Frame_mk(uint8_t *frame, uint8_t nArgs, uint8_t *pArg)
{
    uint8_t idx;
    for(idx = 0; idx < nArgs; idx++)
    {
        frame[idx] = pArg[idx];
    }
}

static void __Commu_Packet_Message(uint8_t messageType, uint8_t compID, uint8_t messageSeq, uint8_t messagePro, uint8_t *pucData, uint8_t dataLen)
{
    int iLoop;
    g_aucMessageBuff[Commu_Message_OFFSET_TYPE] = messageType;
    g_aucMessageBuff[Commu_Message_OFFSET_COMPID] = compID;
    g_aucMessageBuff[Commu_Message_OFFSET_MSGSEQ] = messageSeq;
    g_aucMessageBuff[Commu_Message_OFFSET_PRIORITY] = messagePro;
    g_aucMessageBuff[Commu_Message_OFFSET_SIZE] = dataLen+1;

    for(iLoop = 0; iLoop < dataLen; iLoop++)
    {
        g_aucMessageBuff[Commu_Message_OFFSET_DATA + iLoop] = pucData[iLoop];
    }
}

void __Commu_SprayerStateSwitchMessage_Rep_send(void)
{
    uint8_t ucResultCode = EN_Result_Positive;

    g_stCommuCB.stTxInfo.bIsSendReqACK = 1;
    __Commu_Packet_Message(0x03, 0x02, 0x01, 0x03, &ucResultCode, 1);

    Commu_Send(6, g_aucMessageBuff);
}

void __Commu_SprayerInforSetMessage_Rep_send(void)
{
    uint8_t ucResultCode = EN_Result_Positive;
    __Commu_Packet_Message(0x03, 0x02, 0x02, 0x03, &ucResultCode, 1);
    Commu_Send(6, g_aucMessageBuff);
}


static void __Commu_handle(uint8_t *frame)
{
    //ST_Comm_Message_Type stMsg;
    ST_Comm_MSGHeader_Type *pstMsgHeader = (ST_Comm_MSGHeader_Type *)frame;
    uint8_t *pucData = &frame[SZ_MSGHEADER_LEN];
    //_Commu_Message_Decode(frame,&stMsg);


    if(0x01 == pstMsgHeader->ucType)
    {
        if((0x02 == pstMsgHeader->ucComponentID) && (0x01 == pstMsgHeader->ucMessageSequence))
        {
            if(0x00 == pucData[0]) /*!< Stop spraying */
            {                g_stMainSystemCB.pstSprayer->stateSwitch(g_stMainSystemCB.pstSprayer, OFF);
            }
            else if(0x01 == pucData[0]) /*!< Start spraying */
            {
                g_stMainSystemCB.pstSprayer->stateSwitch(g_stMainSystemCB.pstSprayer, ON);
            }
            __Commu_SprayerStateSwitchMessage_Rep_send();
        }   
        else if((0x02 == pstMsgHeader->ucComponentID) && (0x02 == pstMsgHeader->ucMessageSequence))
        {
            g_stMainSystemCB.usFlowRateDefByTime = pucData[0] * 256 + pucData[1];
            if(g_stMainSystemCB.usFlowRateDefByTime > g_stMainSystemCB.usMaxFlowRateDefByTime)
            {
                g_stMainSystemCB.usMaxFlowRateDefByTime = g_stMainSystemCB.usFlowRateDefByTime;
            }
            g_stMainSystemCB.usFlowRateDefByArea = pucData[2] * 256 + pucData[3];
            g_stMainSystemCB.usDroplets = pucData[4] * 256 + pucData[5];
            g_stMainSystemCB.usAtomize = pucData[6] * 256 + pucData[7];
            if(g_stMainSystemCB.usAtomize > g_stMainSystemCB.usMaxAtomize)
            {
                g_stMainSystemCB.usMaxAtomize = g_stMainSystemCB.usAtomize;
            }            
            g_stMainSystemCB.pstSprayer->uSprayerChannelSwitchState.ucSprayerChannelSwitchState = pucData[8];
            __Commu_SprayerInforSetMessage_Rep_send();
            if(ON == g_stMainSystemCB.pstSprayer->ucIsSprayerOn)
            {
                g_stMainSystemCB.pstSprayer->stateSwitch(g_stMainSystemCB.pstSprayer, ON);
            }
        }
    }
    else if(0x03 == pstMsgHeader->ucType)
    {
        if((0x01 == pstMsgHeader->ucComponentID) && (0x01 == pstMsgHeader->ucMessageSequence))
        {
            g_stMainSystemCB.ucResultCode = pucData[0];
            g_stMainSystemCB.usAircraftAverageVelocity = pucData[1];
        }
    }
}

static void _Commu_Handle_Frame(uint8_t *frame)
{
    __Commu_handle(frame);
    g_stCommuCB.ucRecFrameHandledCounter++;
}

void _Commu_Send_Polling(void)
{
    BOOL bSend = FALSE;
    if(_Commu_IS_TX_Idle())
    {
        if(0 == g_stCommuCB.ucSendFrameIntervalCounter)
        {
            if(EN_Commu_ACK_WAIT_TIMEOUT == g_stCommuCB.stTxInfo.ucAckWaitState)/*if ack timeout, we need retry sending last frame*/
            {
                g_stCommuCB.stTxInfo.ucAckWaitState = EN_Commu_ACK_WAIT_NONE;
                g_stCommuCB.stTxInfo.bReqSendFrame = TRUE;
                bSend = TRUE;
            }
            
            if((EN_Commu_ACK_WAIT_NONE == g_stCommuCB.stTxInfo.ucAckWaitState) &&
               (_Commu_Has_Frame_Send()))/*if no ack to be waited, send a new frame*/
            {
                g_stCommuCB.stTxInfo.bReqSendFrame = TRUE;
                _Commu_Send_Out_Frame();  //  
                bSend = TRUE;
            }
            else
            {
            }

            if(bSend)
            {
                bSend = FALSE;
                _Commu_Trig_Tx();/*to trigger tx intr,when has frame/ack to be send*/
            }
        }
        
        if((FALSE == g_stCommuCB.stTxInfo.bReqSendAck)&&(FALSE == g_stCommuCB.stTxInfo.bReqSendFrame)) /*nothing is sending,and nothing need to send, we should check the crq */
        {
            _Commu_End_Send();
        } 
    }
}

void _Commu_Read_Polling(void)
{
    if(_Commu_Has_Frame_Rev())
    {
        uint8_t len;
        uint8_t frame[SZ_Frame_MAX];
        if(_Commu_Read_Frame(frame, &len))
        {
            _Commu_Handle_Frame(frame);
        }
    }
}

void _Commu_Polling(void)
{
    uint8_t ucLostFrameCounter = 0;
    if(_Commu_Get_ReqRestartCrq())   //  request restart
    {
        _Commu_CLR_ReqRestartCrq();
        _Commu_Start_Send();
    }

    if(g_stCommuCB.ucSendFrameIntervalCounter)   //   
    {
        g_stCommuCB.ucSendFrameIntervalCounter--;
    }

    if(g_stCommuCB.stAckTimer.usTimerCounter)   //
    {
        if(--g_stCommuCB.stAckTimer.usTimerCounter == 0)
        {   //req clock time out
            if(++g_stCommuCB.stAckTimer.usTimeoutCounter > Commu_ACK_TIMEOUT_COUNTER_MAX) /**when >5, how to do **/
            {
                _Commu_CLR_AckAlarm();
                g_stCommuCB.stTxInfo.ucAckWaitState = EN_Commu_ACK_WAIT_NONE;
                g_stCommuCB.stOutFrame.bValid = FALSE;/**ignore the current frame,**/
                g_stCommuCB.ucLostAckCounter++;
                g_stCommuCB.ucSendFrameHandledCounter = g_stCommuCB.stTxInfo.ucFrameRC;//uart_tbox_send_frame_handled_counter++;
            }
            else
            {
                g_stCommuCB.stTxInfo.ucAckWaitState = EN_Commu_ACK_WAIT_TIMEOUT;
            }
        }
    }

    if(g_stCommuCB.stReqSendTimer.usTimerCounter )
    {
        if(--g_stCommuCB.stReqSendTimer.usTimerCounter  == 0)
        {   //req clock time out
            if(++g_stCommuCB.stReqSendTimer.usTimeoutCounter > Commu_REQ_SEND_TIMEOUT_COUNTER_MAX) /**when >10, how to do ?**/
            {
                //uart_tbox_clr_req_send_alarm();
                _Commu_End_Send();/**stop comm**/
                g_stCommuCB.stOutFrame.bValid = FALSE;/**ignore the current frame,**/
                g_stCommuCB.ucReqTxTimeoutCounter++;
            }
            else
            {
                _Commu_Set_ReqSendAlarm();
                _Commu_Crq_Retry();
            }
        }
    }

    if(g_stCommuCB.stEnterTxTimer.usTimerCounter )
    {
        if(--g_stCommuCB.stEnterTxTimer.usTimerCounter  == 0)
        {   
            _Commu_CLR_EnterTxAlarm();
            _Commu_End_Send();/**stop comm**/                
            g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_IDLE;
            g_stCommuCB.stTxInfo.ucAckWaitState = EN_Commu_ACK_WAIT_NONE;
            g_stCommuCB.stOutFrame.bValid = FALSE;/**ignore the current frame,**/
        }
    }

    if(g_stCommuCB.stTxTimer.usTimerCounter )
    {
        if(--g_stCommuCB.stTxTimer.usTimerCounter  == 0)
        {
            if((g_stCommuCB.stTxInfo.bReqSendAck)||(g_stCommuCB.stTxInfo.bReqSendFrame))
            {
                //tx time out
                if(++g_stCommuCB.stTxTimer.usTimeoutCounter > Commu_TX_TIMEOUT_COUNTER_MAX)
                {
                    //uart_tbox_clr_tx_alarm();
                    _Commu_End_Send();/**stop comm**/                
                    g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_IDLE;
                    g_stCommuCB.stOutFrame.bValid = FALSE;/**ignore the current frame,**/
                    g_stCommuCB.ucSendFrameHandledCounter = g_stCommuCB.stTxInfo.ucFrameRC;
                    g_stCommuCB.ucTxTimeoutCounter++;
                }
                else
                {
                    _Commu_Crq_Retry();
                }
            }
            else
            {
                //uart_tbox_clr_tx_alarm();
                _Commu_End_Send();/**stop comm**/                
                g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_IDLE;
            }
        }
    }

    if(g_stCommuCB.stRxTimer.usTimerCounter )
    {
        if(--g_stCommuCB.stRxTimer.usTimerCounter  == 0)
        {//rx time out
            _Commu_CLR_RxAlarm();
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
            g_stCommuCB.ucRxTimeoutCounter++;
        }
    }

    ucLostFrameCounter = g_stCommuCB.ucLostAckCounter+g_stCommuCB.ucReqTxTimeoutCounter+g_stCommuCB.ucRxTimeoutCounter+g_stCommuCB.ucTxTimeoutCounter;
    if(ucLostFrameCounter > Commu_FRAME_LOST_COUNTER_MAX)
    {
        //reset system
        //power_get_navi_reqeust_reset_msg();
    }

}


static void  _Commu_CB_Init(ST_Commu_CB_Type *pstCB)
{
    /*!< init stRxInfo */
    pstCB->stRxInfo.ucStatus = 0;
    pstCB->stRxInfo.ucCs = 0;
    pstCB->stRxInfo.ucLastFrameRC = 0xff;
    pstCB->stRxInfo.ucFrameRC = 0;
    pstCB->stRxInfo.ucFrameLen = 0;
    pstCB->stRxInfo.ucByteIndex = 0;    


    /*!< init stTxInfo */
    pstCB->stTxInfo.bReqSendSyncByte = FALSE;
    pstCB->stTxInfo.bReqSendAck = FALSE;
    pstCB->stTxInfo.bReqSendFrame = FALSE;
    pstCB->stTxInfo.bReqRestartCrq = FALSE;
    pstCB->stTxInfo.ucSendType = EN_Commu_SENDING_NONE;
    pstCB->stTxInfo.ucAckWaitState = EN_Commu_ACK_WAIT_NONE;
    pstCB->stTxInfo.ucStatus = 0;
    pstCB->stTxInfo.ucCs = 0;
    pstCB->stTxInfo.ucAckID = 0;
    pstCB->stTxInfo.ucFrameRC = 0;
    pstCB->stTxInfo.ucFrameLen = 0;
    pstCB->stTxInfo.ucByteIndex = 0;
    pstCB->stTxInfo.bIsSendReqACK = 0;
    pstCB->stTxInfo.ucACKFrameRC = 0;

    /*!< init frame */
    pstCB->stInFrame.bValid = FALSE;
    pstCB->stInFrame.ucLength= 0;
    pstCB->stOutFrame.bValid = FALSE;
    pstCB->stOutFrame.ucLength = 0;    


    /*!< init fifo */
    Fifo_Init(&pstCB->stInFifo[0], pstCB->aucInBuffOne, SZ_Commu_IN_BUF_ONE_MAX);
    Fifo_Init(&pstCB->stInFifo[1], pstCB->aucInBuffTwo, SZ_Commu_IN_BUF_TWO_MAX);
    Fifo_Init(&pstCB->stInFifo[2], pstCB->aucInBuffThree, SZ_Commu_IN_BUF_THREE_MAX);
    Fifo_Init(&pstCB->stInFifo[3], pstCB->aucInBuffFour, SZ_Commu_IN_BUF_FOUR_MAX);
    Fifo_Init(&pstCB->stOutFifo[0], pstCB->aucOutBuffOne, SZ_Commu_OUT_BUF_ONE_MAX);
    Fifo_Init(&pstCB->stOutFifo[1], pstCB->aucOutBuffTwo, SZ_Commu_OUT_BUF_TWO_MAX);
    Fifo_Init(&pstCB->stOutFifo[2], pstCB->aucOutBuffThree, SZ_Commu_OUT_BUF_THREE_MAX);
    Fifo_Init(&pstCB->stOutFifo[3], pstCB->aucOutBuffFour, SZ_Commu_OUT_BUF_FOUR_MAX);

    /*!< init timer */
    _Commu_CLR_Alarm(&pstCB->stReqSendTimer);
    _Commu_CLR_Alarm(&pstCB->stEnterTxTimer);
    _Commu_CLR_Alarm(&pstCB->stTxTimer);
    _Commu_CLR_Alarm(&pstCB->stAckTimer);
    _Commu_CLR_Alarm(&pstCB->stRxTimer);   	
}


void Commu_Init(void)
{
    Commu_Uart_Init();
    _Commu_CB_Init(&g_stCommuCB);
}

void Commu_Proc(void *arg)
{
    _Commu_Send_Polling();   // send a frame to navi periodically if has frame to be send 
    _Commu_Read_Polling();   //
    _Commu_Polling();      
}

BOOL Commu_Send(uint8_t nArgs, uint8_t *pArg)
{ 
    BOOL ret_val = FALSE; 
    uint8_t frame[SZ_Frame_MAX];
    _Commu_Frame_mk(frame, nArgs, pArg);
    ret_val = _Commu_Send_frame(frame, nArgs);

    return ret_val;
}

void Commu_Send_HeartbeatPKT(void)
{
    uint8_t aucDataBuff[4] = {0};
    aucDataBuff[0] = g_stMainSystemCB.ucLiquidResidualCapacity;
    aucDataBuff[1] = g_stMainSystemCB.ucLiquidResidualCapacity;
    aucDataBuff[2] = 0;//spayer_status|(pump_status<1);
    aucDataBuff[3] = g_stMainSystemCB.pstSprayer->ucIsSprayerOn;
    __Commu_Packet_Message(0x01, 0x01, 0x01, 0x03, aucDataBuff, 4);

	Commu_Send(9, g_aucMessageBuff);
}

void Commu_ISR_TX_Proc(void)
{  
    _Commu_CLR_EnterTxAlarm();
    g_stCommuCB.ucTxTimeoutCounter = 0;
    _Commu_CLR_TxAlarm();

    switch(g_stCommuCB.stTxInfo.ucStatus)
    {
    case EN_Commu_ST_IDLE:/*do nothing,error state*/
        break;

    case EN_Commu_ST_START:
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_A5;
        g_stCommuCB.stTxInfo.ucSendType = EN_Commu_SENDING_FRAME; 
        __Commu_SendChar(0xFF);
        break;

			
    case EN_Commu_ST_A5:
        __Commu_SendChar(0xA5);
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_5A;
        break;
			
    case EN_Commu_ST_5A:
        __Commu_SendChar(0x5A);
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_RC;
         break;
			
    case EN_Commu_ST_RC:
        g_stCommuCB.stTxInfo.ucCs = g_stCommuCB.stTxInfo.ucFrameRC;
        /*
        if ( 1 == g_stCommuCB.stTxInfo.bIsSendReqACK )
        {
            __Commu_SendChar(g_stCommuCB.stTxInfo.ucACKFrameRC);
            g_stCommuCB.stTxInfo.ucACKFrameRC = 0;
            g_stCommuCB.stTxInfo.bIsSendReqACK = 0;
        }
        else
        {
        */
            __Commu_SendChar(g_stCommuCB.stTxInfo.ucFrameRC);
        //}
        
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_LEN;
        break ;
			
    case EN_Commu_ST_LEN:
        g_stCommuCB.stTxInfo.ucByteIndex = 0;
        g_stCommuCB.stTxInfo.ucFrameLen = g_stCommuCB.stOutFrame.ucLength;
        __Commu_SendChar(g_stCommuCB.stOutFrame.ucLength); //  start delimiter
        g_stCommuCB.stTxInfo.ucCs ^= g_stCommuCB.stOutFrame.ucLength;
        if(0 == g_stCommuCB.stOutFrame.ucLength)
        {
            g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_DATA_CS;
        }
        else
        {
            g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_DATA;
        }
        break ;
									
    case EN_Commu_ST_TYPE:	/*the first data is frame type: 0xff indicate ack frame.*/
		break;
					
    case EN_Commu_ST_DATA:
        __Commu_SendChar(g_stCommuCB.stOutFrame.aucBuff[g_stCommuCB.stTxInfo.ucByteIndex]);
        g_stCommuCB.stTxInfo.ucCs ^= g_stCommuCB.stOutFrame.aucBuff[g_stCommuCB.stTxInfo.ucByteIndex];
        g_stCommuCB.stTxInfo.ucByteIndex++;
        if(--g_stCommuCB.stTxInfo.ucFrameLen == 0 )
        {
           g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_DATA_CS;//EN_Commu_ST_IDLE;//  //EN_Commu_ST_DATA_CS;
        }
        break;
					
    case EN_Commu_ST_DATA_CS:
        __Commu_SendChar(g_stCommuCB.stTxInfo.ucCs);
        g_stCommuCB.stTxInfo.ucAckWaitState = EN_Commu_ACK_WAIT_NONE;
        g_stCommuCB.stOutFrame.bValid = FALSE;
        g_stCommuCB.ucSendFrameHandledCounter++;
        g_stCommuCB.stTxInfo.ucSendType = EN_Commu_SENDING_NONE;
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_END;
        break ;
					
    case EN_Commu_ST_ACK_CS:
        __Commu_SendChar(0x01^0xff^g_stCommuCB.stTxInfo.ucAckID) ;/*cs= RC^LEN^MSGTYPE.*/
        g_stCommuCB.stTxInfo.ucSendType = EN_Commu_SENDING_NONE;
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_END;
        break ;
        
    case EN_Commu_ST_END:
        _Commu_End_Send(); /** one of the two places to call uart_tbox_end_send() **/
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_IDLE;        
        break;
                    
    default:
        __Commu_SendChar(0xff);
        g_stCommuCB.stTxInfo.ucStatus = EN_Commu_ST_END;
        break ;
    }
		
    if(_Commu_IS_TX_Idle())
    {
        g_stCommuCB.ucSendFrameIntervalCounter = Commu_SEND_FRAME_INTERVAL_MIN;
    }
    else
    {
        if(g_stCommuCB.stTxInfo.ucStatus > EN_Commu_ST_START)
        {
            _Commu_Set_TxAlarm();
        }
    }
}

void Commu_ISR_RX_Proc(void)
{
    uint8_t data = 0 ;
    uint8_t ucRxFramePrio = 0 ;
    g_stCommuCB.ucRxTimeoutCounter = 0;
    _Commu_CLR_RxAlarm();
 
		
    __Commu_RecvChar(&data);

    switch(g_stCommuCB.stRxInfo.ucStatus)   // 0xff  0xa5   0x5a ( 0xa5  |   0xff ) 
    {
    case EN_Commu_ST_IDLE:
        if(0xff == data)   
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_A5;//EN_Commu_ST_A5;
        }
        break;

    case EN_Commu_ST_START:/* No this state */
        g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
        break;
			
    case EN_Commu_ST_A5:
        if(0xa5 == data)
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_5A;
        }
        else if(0xff == data)
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_A5;
        }
        else
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
        }
        break;
					
    case EN_Commu_ST_5A:
        if(0x5a == data)
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_RC;
        }
        else if(0xff == data)
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_A5;
        }
        else
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
        }
        break;
			
    case EN_Commu_ST_RC:
        g_stCommuCB.stRxInfo.ucFrameRC = data;   //  frame id
        g_stCommuCB.stRxInfo.ucCs = g_stCommuCB.stRxInfo.ucFrameRC; //init the cs value
        g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_LEN;
        break;
			
    case EN_Commu_ST_LEN:
        if(data <= SZ_Frame_MAX)  //  30
        {
            g_stCommuCB.stRxInfo.ucFrameLen = data;
            g_stCommuCB.stRxInfo.ucCs ^= g_stCommuCB.stRxInfo.ucFrameLen;   //  cs xor frame_len  is doing what???
            g_stCommuCB.stInFrame.ucLength = data;
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_TYPE;
        }
        else
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
        }
        break;
			
    case EN_Commu_ST_TYPE:	/*the first data indicate frame type:0xff is ack,others is data frame.*/
        if(EN_MSG_ACK_FRAME == data) /** ack frame **/
        {
            g_stCommuCB.stRxInfo.ucMsgType = data;
            g_stCommuCB.stRxInfo.ucCs ^= g_stCommuCB.stRxInfo.ucMsgType;
            --g_stCommuCB.stRxInfo.ucFrameLen;
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_ACK_CS;
        }
        else if(data <= EN_MSG_RESPONSE_ACK_NOT_NEEDED) /** data frame **/
        {
            g_stCommuCB.stRxInfo.ucByteIndex = 0;
            g_stCommuCB.stInFrame.aucBuff[g_stCommuCB.stRxInfo.ucByteIndex] = data;
            g_stCommuCB.stRxInfo.ucMsgType = data;
            g_stCommuCB.stRxInfo.ucCs ^= g_stCommuCB.stRxInfo.ucMsgType;
            --g_stCommuCB.stRxInfo.ucFrameLen;
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_DATA;    
            g_stCommuCB.stRxInfo.ucByteIndex++;
        }
        else
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
        }
        break;
					
    case EN_Commu_ST_DATA:
        g_stCommuCB.stInFrame.aucBuff[g_stCommuCB.stRxInfo.ucByteIndex++] = data;
        g_stCommuCB.stRxInfo.ucCs ^= data;
        if(g_stCommuCB.stRxInfo.ucFrameLen > 0)
        {
            if(--g_stCommuCB.stRxInfo.ucFrameLen == 0)
            {
                g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_DATA_CS;
            }
        }
        else
        {
            g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_DATA_CS;
        }
        break;
				
    case EN_Commu_ST_DATA_CS:
        if(data == g_stCommuCB.stRxInfo.ucCs)    /** a full frame received */
        {
            g_stCommuCB.stRxInfo.ucByteIndex = 0;   /** clear byte_index */

            ucRxFramePrio = g_stCommuCB.stInFrame.aucBuff[TBOX_CMD_PRIO];
             if(ucRxFramePrio <= Commu_Message_PRIO_NUM)
             {
                 
                if(TRUE == Fifo_Input_Frame(&g_stCommuCB.stInFifo[ucRxFramePrio-1], &(g_stCommuCB.stInFrame.aucBuff[0]), g_stCommuCB.stInFrame.ucLength)) 
                {/**if fifo full, will not send ack, so navi will send again. **/
                    g_stCommuCB.ucRecFrameCounter++;
                    g_stCommuCB.stRxInfo.ucLastFrameRC  = g_stCommuCB.stRxInfo.ucFrameRC;                    

                    if(EN_MSG_REQ_ACK_NEEDED == g_stCommuCB.stRxInfo.ucMsgType)
                    {
                        if(0 == g_stCommuCB.stTxInfo.ucACKFrameRC)
                        {
                            g_stCommuCB.stTxInfo.ucACKFrameRC = g_stCommuCB.stRxInfo.ucFrameRC;
                        }
                    }
                }
                else
                {
                         //fifo is full
                }
             }
        }
        else
        {             
        }
        g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
        break;
					
    case EN_Commu_ST_ACK_CS:
         if((g_stCommuCB.stRxInfo.ucFrameRC == g_stCommuCB.stTxInfo.ucFrameRC)&&(data == g_stCommuCB.stRxInfo.ucCs)) /** a new ack received **/
         {
             g_stCommuCB.ucLostAckCounter = 0;
             _Commu_CLR_AckAlarm(); /**kill the timer **/
             g_stCommuCB.stTxInfo.ucAckWaitState = EN_Commu_ACK_WAIT_NONE;/**/
             g_stCommuCB.stOutFrame.bValid = FALSE;/** uart_tbox_out_frame has been send successfully**/
             g_stCommuCB.ucSendFrameHandledCounter = g_stCommuCB.stTxInfo.ucFrameRC;
         }
         g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
         break ;
					 
    default:
        g_stCommuCB.stRxInfo.ucStatus = EN_Commu_ST_IDLE;
        break ;
    }
}


    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
