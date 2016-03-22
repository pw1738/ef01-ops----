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
#ifndef __UART_TP_H_
#define __UART_TP_H_

#include "Communication\FIFO.H"

/**** Definition of constants ****/


/**** Definition of types ****/
#define ERR_OK  0xff 

#define TBOX_UART_MSG_PRIO_NUM 	4	  /*tbox only define 4 priority.*/

#define TBOX_UART_SEND_FRAME_INTERVAL_MIN 5/**5*2ms**/
#define TBOX_UART_ENTER_TX_TIMEOUT_MAX 50/**50*2ms**/
#define TBOX_UART_REQ_SEND_TIMEOUT_MAX 0/**uart don't need synchronism mechanism,so set the value to 0.**/
#define TBOX_UART_TX_TIMEOUT_MAX 10/**10*2ms**/
#define TBOX_UART_ACK_TIMEOUT_MAX 100/**100*2ms=200ms**/
#define TBOX_UART_RX_TIMEOUT_MAX 50/**50*2ms**/

#define TBOX_REQ_CONNECT_COUNTER_MAX 3
#define TBOX_REQ_SEND_TIMEOUT_COUNTER_MAX 5
#define TBOX_TX_TIMEOUT_COUNTER_MAX 5
#define TBOX_ACK_TIMEOUT_COUNTER_MAX 5
#define TBOX_RX_TIMEOUT_COUNTER_MAX 5
#define TBOX_FRAME_LOST_COUNTER_MAX 5

/**** Declaration of constants ****/

#define UART_TBOX_INP_BUF_ONE_SIZE 256     /* Input buffer size */
#define UART_TBOX_INP_BUF_TWO_SIZE 512     /* Input buffer size */
#define UART_TBOX_INP_BUF_THREE_SIZE 512     /* Input buffer size */
#define UART_TBOX_INP_BUF_FOUR_SIZE 256     /* Input buffer size */

#define UART_TBOX_OUT_BUF_ONE_SIZE 256     /* Input buffer size */
#define UART_TBOX_OUT_BUF_TWO_SIZE 512     /* Input buffer size */
#define UART_TBOX_OUT_BUF_THREE_SIZE 512     /* Input buffer size */
#define UART_TBOX_OUT_BUF_FOUR_SIZE 256     /* Input buffer size */
//enum
//{
//    TP_ST_IDLE,
//    TP_ST_START, /*Only for tx*/
 //   TP_ST_A5,
//    TP_ST_5A,
//    TP_ST_LEN,
//    TP_ST_DATA,
//    TP_ST_DATA_CS,
 //   TP_ST_END/*Only for tx*/
//} UART_PROCESS_STATUS;

typedef struct 
{
    FIFO_TYPE fifo_info;
     uint8_t  *pBuff;
} UART_FIFO_TYPE;

typedef struct 
{
    BOOL req_send_sync_byte;
    BOOL req_send_ack;
    BOOL req_send_frame;
    BOOL req_restart_crq;
    uint8_t send_type;/*is sending ack or frame or nothing?*/
    uint8_t ack_wait_state;
    uint8_t status;/** tx status **/
    uint8_t cs; /** cs  =ID^LENGTH^DATA1^DATA2^....^DATAn **/
    uint8_t ack_id;/** id that just received and need to sent it's ack **/
    uint8_t frame_id;/** id value that will be sent in a new frame**/
    uint8_t frame_len; /** data frame len,  not including the len byte **/ 
    uint8_t byte_index;	
} UART_TX_INFO_TYPE;

typedef struct 
{
	uint8_t status;/** rx status **/
	uint8_t cs;/** cs  =ID^LENGTH^DATA1^DATA2^....^DATAn **/
	uint8_t last_frame_id;/** received successfully at last time **/
	uint8_t frame_id;/** received currentlly **/
	uint8_t frame_len;/** data frame len,  not including the len byte **/ 
	uint8_t byte_index; 
	uint8_t frame_type;
} UART_RX_INFO_TYPE;

typedef struct 
{
    uint16_t timer_counter;/** Timer = polling period*timer_counter **/
    uint16_t timeout_counter;   
} UART_TIMER_TYPE;

/**** Declaration of constants ****/
#define UART_OUT_BUF_SIZE 1024     /* Input buffer size */

/**** Declaration of variables ****/


/**** Declaration of functions ****/
static void uart_init_var(void);
static BOOL uart_send_out_frame(void);
static BOOL uart_has_frame_send(void);
static BOOL uart_is_tx_idle(void);
static BOOL uart_read_frame(uint8_t *frame, uint8_t *length);
static BOOL uart_has_frame_rev(void);
static BOOL uart_req_restart_crq(void);
static void uart_clr_restart_crq(void);
static void uart_handle_frame(uint8_t *frame);
static void uart_crq_retry(void);
static void uart_trig_tx(void);
static void uart_set_restart_crq(void);
static void uart_start_send(void);
static void uart_end_send(void);

static void uart_send_ack(uint8_t frame_id);

static void uart_set_tx_alarm(void);
static void uart_clr_tx_alarm(void);

//static void uart_set_rx_alarm(void);
static void uart_clr_rx_alarm(void);

static void uart_set_ack_alarm(void);
static void uart_clr_ack_alarm(void);

static void uart_set_enter_tx_alarm(void);
static void uart_clr_enter_tx_alarm(void);

static void uart_set_req_send_alarm(void);
static void uart_clr_req_send_alarm(void);

static void uart_clr_all_send_alarm(void);


int uart_is_tx_start(void);
#endif /*__UART_TP_H_*/

/****************************** END OF FILE ***************************/
