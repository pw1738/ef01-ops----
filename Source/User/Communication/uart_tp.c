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

/**** System include files ****/
#include "stm32f10x.h"
/*---Public include files---------------------------------------------*/
#include "Communication\FIFO_IF.H"

/*---Private include files--------------------------------------------*/
#include "Communication\uart_tp.h"
#include "Communication\uart_tp_if.h"
#include "Communication\uart.h"
#include "Communication\uart_if.h"
#include "Communication\tp_msg.h"

/*===VARIABLES========================================================*/

/*---Global-----------------------------------------------------------*/
// enum
// {
//    TP_ST_IDLE,
//    TP_ST_START, /*Only for tx*/
//    TP_ST_A5,
//    TP_ST_5A,
//    TP_ST_LEN,
//    TP_ST_DATA,
//    TP_ST_DATA_CS,
//    TP_ST_END/*Only for tx*/
// } UART_PROCESS_STATUS;

enum
{
    TBOX_ST_IDLE,
    TBOX_ST_START, /*Only for tx*/
    TBOX_ST_A5,
    TBOX_ST_5A,
    TBOX_ST_ID,
		TBOX_ST_LEN,
    TBOX_ST_LEN_1,
		TBOX_ST_LEN_2,
		TBOX_ST_FRAME_TYPE,
		TBOX_ST_FRAME_ID,
		TBOX_ST_ADDR_64,
		TBOX_ST_ADDR_16,
    TBOX_ST_TYPE,  /*indicate is ack frame or data frame.*/
    TBOX_ST_ACK_CS,
		TBOX_ST_DATA_PRE,
    TBOX_ST_DATA,
    TBOX_ST_DATA_CS,
    TBOX_ST_END/*Only for tx*/
} UART_TBOX_PROCESS_STATUS;


enum 
{
    TBOX_REQ_ACK_NEEDED,
    TBOX_REQ_ACK_NOT_NEEDED,
    TBOX_RESPONSE_ACK_NEEDED,
    TBOX_RESPONSE_ACK_NOT_NEEDED,
    
    TBOX_FRAME_TYPE_MAX,
    TBOX_ACK_FRAME = 0xFF,
}MSG_TYPE;

enum
{
    TBOX_IS_SENDING_NONE,
    TBOX_IS_SENDING_ACK, 
    TBOX_IS_SENDING_FRAME,
} UART_SEND_TYPE;

enum
{
    TBOX_ACK_WAIT_NONE,
    TBOX_ACK_WAITING, 
    TBOX_ACK_WAIT_TIMEOUT,
} UART_TBOX_ACK_WAIT_STATE;

/*---Private----------------------------------------------------------*/
UART_FRAME_TYPE  uart_out_frame;
UART_FRAME_TYPE  uart_in_frame;

UART_TX_INFO_TYPE  volatile  uart_tx_info;
UART_RX_INFO_TYPE  volatile  uart_rx_info;

volatile uint8_t uart_tx_timeout_counter;
volatile uint8_t uart_rx_timeout_counter;

volatile uint8_t uart_rec_frame_counter;
volatile uint8_t uart_rec_frame_handled_counter;

volatile uint8_t uart_req_tx_timeout_counter;

volatile uint8_t  uart_send_frame_counter;
volatile uint8_t  uart_lost_ack_counter;
volatile uint8_t  uart_out_buff[UART_OUT_BUF_SIZE];
volatile uint8_t  uart_send_frame_handled_counter;
volatile uint8_t  uart_read_frame_counter;
volatile uint8_t  uart_read_frame_handled_counter;
volatile uint8_t  uart_send_frame_interval_counter;

// Addr
uint8_t Addr_64[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF } ;
uint8_t Addr_16[2] = { 0xFF, 0xFE } ;
uint8_t addr_16_len = 0 ;
uint8_t addr_64_len = 0 ;

uint8_t uart_in_buff_one[UART_TBOX_INP_BUF_ONE_SIZE];
uint8_t uart_in_buff_two[UART_TBOX_INP_BUF_TWO_SIZE];
uint8_t uart_in_buff_three[UART_TBOX_INP_BUF_THREE_SIZE];
uint8_t uart_in_buff_four[UART_TBOX_INP_BUF_FOUR_SIZE];

uint8_t uart_out_buff_one[UART_TBOX_OUT_BUF_ONE_SIZE];
uint8_t uart_out_buff_two[UART_TBOX_OUT_BUF_TWO_SIZE];
uint8_t uart_out_buff_three[UART_TBOX_OUT_BUF_THREE_SIZE];
uint8_t uart_out_buff_four[UART_TBOX_OUT_BUF_FOUR_SIZE];

UART_TIMER_TYPE uart_req_send_timer,uart_enter_tx_timer,uart_tx_timer,uart_ack_timer,uart_rx_timer;
UART_FIFO_TYPE uart_in_fifo[TBOX_UART_MSG_PRIO_NUM],uart_out_fifo[TBOX_UART_MSG_PRIO_NUM];

/***********************************************************************
*  Name         : uart_init_struct_var
*  Description : init the struct var  
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void  uart_init_var(void)
{
    //init uart_rx_info
    uart_rx_info.status = 0;
    uart_rx_info.cs = 0;
    uart_rx_info.last_frame_id = 0xff;
    uart_rx_info.frame_id = 0;
    uart_rx_info.frame_len = 0;
    uart_rx_info.byte_index = 0;

    //init uart_rx_info
    //init uart_tbox_tx_info
    uart_tx_info.req_send_sync_byte = FALSE;
    uart_tx_info.req_send_ack = FALSE;
    uart_tx_info.req_send_frame = FALSE;
    uart_tx_info.req_restart_crq = FALSE;
    uart_tx_info.send_type = TBOX_IS_SENDING_NONE;
    uart_tx_info.ack_wait_state = TBOX_ACK_WAIT_NONE;
    uart_tx_info.status= 0;
    uart_tx_info.cs= 0;
    uart_tx_info.ack_id= 0;
    uart_tx_info.frame_id= 0;
    uart_tx_info.frame_len= 0;
    uart_tx_info.byte_index= 0;

    //init frame
    uart_in_frame.bValid = FALSE;
    uart_in_frame.length= 0;
    uart_out_frame.bValid = FALSE;
    uart_out_frame.length = 0;

    //init fifo	

    uart_in_fifo[0].pBuff = uart_in_buff_one;
    uart_in_fifo[1].pBuff = uart_in_buff_two;
    uart_in_fifo[2].pBuff = uart_in_buff_three;
    uart_in_fifo[3].pBuff = uart_in_buff_four;

    uart_out_fifo[0].pBuff = uart_out_buff_one;
    uart_out_fifo[1].pBuff = uart_out_buff_two;
    uart_out_fifo[2].pBuff = uart_out_buff_three;
    uart_out_fifo[3].pBuff = uart_out_buff_four;

    fifo_init(&uart_in_fifo[0].fifo_info, uart_in_fifo[0].pBuff , UART_TBOX_INP_BUF_ONE_SIZE);
    fifo_init(&uart_in_fifo[1].fifo_info, uart_in_fifo[1].pBuff , UART_TBOX_INP_BUF_TWO_SIZE);
    fifo_init(&uart_in_fifo[2].fifo_info, uart_in_fifo[2].pBuff , UART_TBOX_INP_BUF_THREE_SIZE);
    fifo_init(&uart_in_fifo[3].fifo_info, uart_in_fifo[3].pBuff , UART_TBOX_INP_BUF_FOUR_SIZE);

    fifo_init(&uart_out_fifo[0].fifo_info, uart_out_fifo[0].pBuff , UART_TBOX_OUT_BUF_ONE_SIZE);
    fifo_init(&uart_out_fifo[1].fifo_info, uart_out_fifo[1].pBuff , UART_TBOX_OUT_BUF_TWO_SIZE);
    fifo_init(&uart_out_fifo[2].fifo_info, uart_out_fifo[2].pBuff , UART_TBOX_OUT_BUF_THREE_SIZE);
    fifo_init(&uart_out_fifo[3].fifo_info, uart_out_fifo[3].pBuff , UART_TBOX_OUT_BUF_FOUR_SIZE);


    //init timer
    uart_clr_req_send_alarm();
    uart_clr_enter_tx_alarm();
    uart_clr_tx_alarm();
    uart_clr_ack_alarm();
    uart_clr_rx_alarm();
		
}

/*===FUNCTIONS========================================================*/
/***********************************************************************
*  Name        : uart_down
*  Description : kill eng uart comm.
*  Parameter   : void
*  Returns     : void
***********************************************************************/
void uart_down(void)
{
 //Disable & kill ComPort here
	UART_Disable(ENABLE);
	
}

/***********************************************************************
*  Name         : uart_create
*  Description : creat spi navi  
*  Parameter  : None
*  Returns      : None
***********************************************************************/
void uart_create(void)
{
	USART_Config();
	uart_init_var();
}


/***********************************************************************
*  Name         : uart_send_frame
*  Description : send a frame to uart_out_fifo[TBOX_UART_MSG_PRIO_NUM]      
*  Parameter  : uint8_t *frame,uint8_t length
*  Returns      : BOOL
***********************************************************************/
BOOL uart_send_frame(uint8_t *frame,uint8_t length)
{
    BOOL ret_val = FALSE;
		uint8_t tx_frame_prio;    //The priority of the message   The priority is seted 4 different classes
	/*put data into fifo bace on the priority of the data frame.*/
		tx_frame_prio = 1;  //*(frame + TBOX_CMD_PRIO);   //   TBOX_CMD_PRIO		 		 4   frame[4]  To select the prio according to the frame[4]
	if(tx_frame_prio <= TBOX_UART_MSG_PRIO_NUM)  	// frame[4]  < TBOX_UART_MSG_PRIO_NUM 	4
		{
	    if(fifo_input_frame(&uart_out_fifo[tx_frame_prio - 1].fifo_info,(uint8_t *)frame,length))  // send frame to a out_fifo
	    {
	        uart_send_frame_counter++;
	        ret_val = TRUE;
	    }
		}
    return ret_val;
}

/***********************************************************************
*  Name         : uart_send_polling
*  Description : send a frame to navi periodically if has frame to be send.       
*  Parameter  : None
*  Returns      : None
***********************************************************************/
void uart_send_polling(void)
{
		BOOL bSend = FALSE;
    if(uart_is_tx_idle())
    {
			if(0 == uart_send_frame_interval_counter)
			{
				if(TBOX_ACK_WAIT_TIMEOUT == uart_tx_info.ack_wait_state)/*if ack timeout, we need retry sending last frame*/
				   {
            uart_tx_info.ack_wait_state = TBOX_ACK_WAIT_NONE;
            uart_tx_info.req_send_frame = TRUE;
            bSend = TRUE;
				  }
				 if((TBOX_ACK_WAIT_NONE == uart_tx_info.ack_wait_state)&&(uart_has_frame_send()))/*if no ack to be waited, send a new frame*/
					{
                uart_tx_info.req_send_frame = TRUE;
                uart_send_out_frame();  //  
                bSend = TRUE;
					}
				else
					{
                //
					}
				if(bSend)
					{
                bSend = FALSE;
                uart_trig_tx();/*to trigger tx intr,when has frame/ack to be send*/
					};
			};
        if((FALSE == uart_tx_info.req_send_ack)&&(FALSE == uart_tx_info.req_send_frame)) /*nothing is sending,and nothing need to send, we should check the crq */
        {
            //if(FALSE == IO_UP2NP_SPI_IRQ_GetVal())/**/
                uart_end_send();
        } 
    }
}

/***********************************************************************
*  Name         : uart_polling
*  Description : It's called by timer interrupt periodically as a timer.  
*  Parameter  : None
*  Returns      : None
***********************************************************************/
void uart_polling(void)
{
    uint8_t lost_frame_counter = 0;
    if(uart_req_restart_crq())   //  request restart
    {
        uart_clr_restart_crq();
        uart_start_send();
    }

    if(uart_send_frame_interval_counter)   //   
    {
        uart_send_frame_interval_counter--;
    }

    if(uart_ack_timer.timer_counter )   //
    {
        if(--uart_ack_timer.timer_counter == 0)
        {   //req clock time out
            if(++uart_ack_timer.timeout_counter > TBOX_ACK_TIMEOUT_COUNTER_MAX) /**when >5, how to do **/
            {
                uart_clr_ack_alarm();
                uart_tx_info.ack_wait_state = TBOX_ACK_WAIT_NONE;
                uart_out_frame.bValid = FALSE;/**ignore the current frame,**/
                uart_lost_ack_counter++;
                uart_send_frame_handled_counter = uart_tx_info.frame_id;//uart_tbox_send_frame_handled_counter++;
            }
            else
            {
                uart_tx_info.ack_wait_state = TBOX_ACK_WAIT_TIMEOUT;
            }
        }
    }

    if(uart_req_send_timer.timer_counter )
    {
        if(--uart_req_send_timer.timer_counter  == 0)
        {   //req clock time out
            if(++uart_req_send_timer.timeout_counter > TBOX_REQ_SEND_TIMEOUT_COUNTER_MAX) /**when >10, how to do ?**/
            {
                //uart_tbox_clr_req_send_alarm();
                uart_end_send();/**stop comm**/
                uart_out_frame.bValid = FALSE;/**ignore the current frame,**/
                //uart_send_frame_handled_counter = uart_tx_info.frame_id;  // changed
                uart_req_tx_timeout_counter++;
            }
            else
            {
                uart_set_req_send_alarm();
                uart_crq_retry();
            }
        }
    }

    if(uart_enter_tx_timer.timer_counter )
    {
        if(--uart_enter_tx_timer.timer_counter  == 0)
        {   
            uart_clr_enter_tx_alarm();
            uart_end_send();/**stop comm**/                
            uart_tx_info.status = TBOX_ST_IDLE;
            uart_tx_info.ack_wait_state = TBOX_ACK_WAIT_NONE;
            uart_out_frame.bValid = FALSE;/**ignore the current frame,**/
        }
    }
    
    if(uart_tx_timer.timer_counter )
    {
        if(--uart_tx_timer.timer_counter  == 0)
        {
            if((uart_tx_info.req_send_ack)||(uart_tx_info.req_send_frame))
            {
                //tx time out
                if(++uart_tx_timer.timeout_counter > TBOX_TX_TIMEOUT_COUNTER_MAX)
                {
                    //uart_tbox_clr_tx_alarm();
                    uart_end_send();/**stop comm**/                
                    uart_tx_info.status = TBOX_ST_IDLE;
                    uart_out_frame.bValid = FALSE;/**ignore the current frame,**/
                    //uart_send_frame_handled_counter = ;//uart_tx_info.frame_id;   //changed
                    uart_tx_timeout_counter++;
                }
                else
                {
                    uart_crq_retry();
                }
            }
            else
            {
                    //uart_tbox_clr_tx_alarm();
                    uart_end_send();/**stop comm**/                
                    uart_tx_info.status = TBOX_ST_IDLE;
            }
        }
    }

    if(uart_rx_timer.timer_counter )
    {
        if(--uart_rx_timer.timer_counter  == 0)
        {//rx time out
            uart_clr_rx_alarm();
            uart_rx_info.status = TBOX_ST_IDLE;
            uart_rx_timeout_counter++;
        }
    }
	
    lost_frame_counter = uart_lost_ack_counter+uart_req_tx_timeout_counter+uart_rx_timeout_counter+uart_tx_timeout_counter;
    if(lost_frame_counter > TBOX_FRAME_LOST_COUNTER_MAX)
    {
        //reset system
        //power_get_navi_reqeust_reset_msg();
    }
}

/***********************************************************************
*  Name :  uart_is_tx_start
*  Description  :  
*  Parameters  :  None
*  Returns  :  None
************************************************************************/
// int uart_is_tx_start(void)
// {
// 	return (TP_ST_START == uart_tx_info.status);
// }

/***********************************************************************
*  Name         : uart_is_tx_idle
*  Description : is tx idle ?
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static BOOL uart_is_tx_idle(void)
{
	BOOL ret_val =FALSE ;
	if( uart_tx_info.status == TBOX_ST_IDLE)
	  ret_val = TRUE ;
	return ret_val;
}

/***********************************************************************
*  Name         : uart_has_frame_send
*  Description : to confirm if has frame to be send. 
*  Parameter  : None
*  Returns      : BOOL
***********************************************************************/
static BOOL uart_has_frame_send(void)
{
	BOOL ret_val =FALSE ;
	if( uart_send_frame_counter != uart_send_frame_handled_counter)
	  ret_val = TRUE ;
	return ret_val;
}

/***********************************************************************
*  Name         : uart_tp_start_send
*  Description : 
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_start_send(void)
{
	if( 1 )  //©0 == UART_TBOX_IS_TX_FLAG )   
    {
      uart_tx_info.status = TBOX_ST_START;
      uart_tx_info.req_send_sync_byte = FALSE;
      uart_req_tx_timeout_counter = 0;
      uart_clr_req_send_alarm();
      uart_set_enter_tx_alarm();
			
			UART_SendChar(0x00);/*not use oxff,because when receive frame,tbox start with 0xff*/
    }
}

/***********************************************************************
*  Name         : uart_tx_proc
*  Description : it's called by the interrupt handler, and put char out of the buffer   
*  Parameter  : None
*  Returns      : None
***********************************************************************/
void uart_tx_proc(void)
{  
		uart_clr_enter_tx_alarm();
    uart_tx_timeout_counter = 0;
    uart_clr_tx_alarm();

    switch(uart_tx_info.status)
    {
        case TBOX_ST_IDLE:/*do nothing,error state*/
            break;

        case TBOX_ST_START:

        case TBOX_ST_END:
            if(uart_tx_info.req_send_ack)
            {
                uart_tx_info.req_send_ack = FALSE;
                uart_tx_info.send_type = TBOX_IS_SENDING_ACK;
                UART_SendChar(0xff);
                uart_tx_info.status = TBOX_ST_A5;
            }
            else if((TRUE == uart_tx_info.req_send_frame)&&(TRUE == uart_out_frame.bValid))
            {
                uart_tx_info.req_send_frame = FALSE;
                uart_tx_info.send_type = TBOX_IS_SENDING_FRAME;         
                UART_SendChar(0xff);
								//UART_SendChar(0x00);
                uart_tx_info.status = TBOX_ST_A5;
            }
            else 
            {/** it has nothing to send **/
                uart_end_send(); /** one of the two places to call uart_tbox_end_send() **/
                uart_tx_info.status = TBOX_ST_IDLE;
            }
						break;
				
        case TBOX_ST_A5:
            UART_SendChar(0xa5);
            uart_tx_info.status = TBOX_ST_5A;
            break;
				
        case TBOX_ST_5A:
            UART_SendChar(0x5a);
            uart_tx_info.status = TBOX_ST_ID;
             break;
				
        case TBOX_ST_ID:
            if(TBOX_IS_SENDING_ACK == uart_tx_info.send_type) /*send ack*/ 
            {
                UART_SendChar( uart_tx_info.ack_id );
            }
            else/* send a data frame */
            {
							  UART_SendChar(uart_tx_info.frame_id);
            }	
            uart_tx_info.status = TBOX_ST_LEN;
            break ;
				
        case TBOX_ST_LEN:
            if(TBOX_IS_SENDING_ACK == uart_tx_info.send_type) /*send ack*/ 
            {
                UART_SendChar(0x01);/*ack len=0x01*/
                uart_tx_info.status = TBOX_ST_TYPE;
            }
            else/* send a data frame */
            {
                uart_tx_info.byte_index = 0;
                uart_tx_info.frame_len = uart_out_frame.length;
								UART_SendChar(uart_out_frame.length); //  start delimiter
								//--uart_tx_info.frame_len;
                //UART_SendChar(uart_out_frame.length);
                if(0==uart_out_frame.length)
                {
                    uart_tx_info.status = TBOX_ST_DATA_CS;
                }
                else
                {
                    uart_tx_info.status = TBOX_ST_DATA;
                }
            }
						break ;
										
				case TBOX_ST_TYPE:	/*the first data is frame type: 0xff indicate ack frame.*/
						if(TBOX_IS_SENDING_ACK == uart_tx_info.send_type) /*send ack*/ 
						{
								UART_SendChar(0xff);/*ack msgtype=0xff*/
								uart_tx_info.status = TBOX_ST_ACK_CS;
						}
						else
						{
								uart_tx_info.status = TBOX_ST_END;
						}
						break;
						
        case TBOX_ST_DATA:
						UART_SendChar(uart_out_frame.buff[uart_tx_info.byte_index++]);
            if(--uart_tx_info.frame_len == 0 )
            {
               uart_tx_info.status = TBOX_ST_DATA_CS;//TBOX_ST_IDLE;//  //TBOX_ST_DATA_CS;
            }

						break;
						
        case TBOX_ST_DATA_CS:
            UART_SendChar(uart_out_frame.buff[uart_out_frame.length]); /*uart_tbox_out_frame.length byte is cs*/
			/*if sended frame type is TBOX_REQ_ACK_NEEDED,need to wait ack,otherwise don't have to wait ack.*/
						if( (TBOX_REQ_ACK_NEEDED == uart_out_frame.buff[0])
									||(TBOX_RESPONSE_ACK_NEEDED == uart_out_frame.buff[0]) )
						{
				/*set the bit prevent from send a new frame when sent a frame without receiving an ack back*/
							uart_tx_info.ack_wait_state = TBOX_ACK_WAITING; 
            	uart_set_ack_alarm();/* timing */ 
						}
						else
						{
								uart_lost_ack_counter = 0;
                uart_clr_ack_alarm(); /**kill the timer **/
                uart_tx_info.ack_wait_state = TBOX_ACK_WAIT_NONE;/**/
                uart_out_frame.bValid = FALSE;/** uart_tbox_out_frame regard as been send successfully**/
                uart_send_frame_handled_counter++; //= uart_tx_info.frame_id;
						}
            uart_tx_info.send_type = TBOX_IS_SENDING_NONE;
            uart_tx_info.status = TBOX_ST_END;
            break ;
						
        case TBOX_ST_ACK_CS:
            UART_SendChar(0x01^0xff^uart_tx_info.ack_id) ;/*cs= RC^LEN^MSGTYPE.*/
            uart_tx_info.send_type = TBOX_IS_SENDING_NONE;
            uart_tx_info.status = TBOX_ST_END;
            break ;
						
        default:
            UART_SendChar(0xff);
            uart_tx_info.status = TBOX_ST_END;
						break ;
    }
		
    if(uart_is_tx_idle())
    {
        uart_send_frame_interval_counter = TBOX_UART_SEND_FRAME_INTERVAL_MIN;
    }
    else
    {
        if(uart_tx_info.status > TBOX_ST_START)
        {
            uart_set_tx_alarm();
        }
    }
}


/***********************************************************************
*  Name         : uart_send_out_frame
*  Description : get frame from output fifo,and request sendding      
*  Parameter  : None
*  Returns      : BOOL
***********************************************************************/
static BOOL uart_send_out_frame(void)
{
	BOOL ret_val = FALSE;
	uint8_t cs,i;

	if(FALSE == uart_out_frame.bValid)/**uart_out_frame send completely **/
	{
		if( !fifo_is_empty(&uart_out_fifo[0].fifo_info) )
		{
			/**get frame to uart_tbox_out_frame**/
			ret_val = fifo_output_frame(&uart_out_fifo[0].fifo_info, uart_out_frame.buff,&uart_out_frame.length);
		}
		else if( !fifo_is_empty(&uart_out_fifo[1].fifo_info) )
		{
			/**get frame to uart_tbox_out_frame**/
			ret_val = fifo_output_frame(&uart_out_fifo[1].fifo_info, uart_out_frame.buff,&uart_out_frame.length);
		}
		else if( !fifo_is_empty(&uart_out_fifo[2].fifo_info) )
		{
			/**get frame to uart_tbox_out_frame**/
			ret_val = fifo_output_frame(&uart_out_fifo[2].fifo_info, uart_out_frame.buff,&uart_out_frame.length);
		}
		else if( !fifo_is_empty(&uart_out_fifo[3].fifo_info) )
		{
			/**get frame to uart_tbox_out_frame**/
			ret_val = fifo_output_frame(&uart_out_fifo[3].fifo_info, uart_out_frame.buff,&uart_out_frame.length);
		}
		
		if( TRUE == ret_val )
		{
			cs = ++uart_tx_info.frame_id;  // fame_id ++  ??
			cs ^= uart_out_frame.length;//cs  =ID^LENGTH^DATA1^DATA2^....^DATAn    To caculate the checksum
			for(i=0; i<uart_out_frame.length ;i++)
			{
				cs^=uart_out_frame.buff[i];
			}
			uart_out_frame.buff[uart_out_frame.length] = cs;
			uart_out_frame.bValid = TRUE;
			ret_val = TRUE;
		}
	}
	return ret_val;
}

/**********************************************************************
*    Name	:	uart_read_to_frame
*    Desciption	:	Read RX to the Frame
*    Parameter : None
*    Returns  : None
***********************************************************************/
void uart_read_to_frame(void)
{
	uint8_t data = 0 ;
	uart_read_frame_counter++ ;
	UART_RecvChar( &data );
}

/***********************************************************************
*  Name         : uart_rx_proc
*  Description : it's called by the interrupt handler,and put char into buffer when valid.      
*  Parameter  : None
*  Returns      : None

* Data Frame : ff a5 5a aa @framelen  @frametype  
***********************************************************************/
void uart_rx_proc(void)
{
		//contact_bind();
    uint8_t data = 0 ;
		uint8_t rx_frame_prio = 0 ;
    uart_rx_timeout_counter = 0 ;
    uart_clr_rx_alarm();

	#if 0	/*not used.*/
	if(TRUE == uart_tx_info.req_send_sync_byte)/** trig to start sending a new frame **/
    {
        if(0 == UART_TBOX_IS_TX_FLAG())
        {
            UART_TBOX_SendChar(0x00);/*not use oxff,because when receive frame,tbox start with 0xff*/

            uart_tx_info.status = TBOX_ST_START;
            uart_tx_info.req_send_sync_byte = FALSE;
            uart_tbox_req_tx_timeout_counter = 0;
            uart_tbox_clr_req_send_alarm();
            uart_tbox_set_enter_tx_alarm();
        }
    }
	#endif
   
    UART_RecvChar(&data) ;
//         if(ERR_OK != data)   
//      {
//          uart_rx_info.status = TBOX_ST_IDLE;
//         return;
//      }

    switch(uart_rx_info.status)   // 0xff  0xa5   0x5a ( 0xa5  |   0xff ) 
    {
        case TBOX_ST_IDLE:
            if(0xff == data)   
            {
								uart_rx_info.status = TBOX_ST_A5;//TBOX_ST_A5;
            }
            break;

        case TBOX_ST_START:/*No this state*/
            uart_rx_info.status = TBOX_ST_IDLE;
            break;
				
        case TBOX_ST_A5:
            if(0xa5 == data)
            {
								uart_rx_info.status = TBOX_ST_5A;
            }
            else if(0xff == data)
            {
								uart_rx_info.status = TBOX_ST_A5;
            }
            else
            {
								uart_rx_info.status = TBOX_ST_IDLE;
            }
            break;
						
        case TBOX_ST_5A:
            if(0x5a == data)
            {
                uart_rx_info.status = TBOX_ST_ID;
            }
            else if(0xff == data)
            {
                uart_rx_info.status = TBOX_ST_A5;
            }
            else
            {
                uart_rx_info.status = TBOX_ST_IDLE;
            }
            break;
				
        case TBOX_ST_ID:
            uart_rx_info.frame_id = data;   //  frame id
            uart_rx_info.cs = uart_rx_info.frame_id; //init the cs value
            uart_rx_info.status = TBOX_ST_LEN;
            break;
				
        case TBOX_ST_LEN:
						if(data <= UART_FRAME_SIZE)  //  30
						{
							uart_rx_info.frame_len = data;
							uart_rx_info.cs ^= uart_rx_info.frame_len;   //  cs xor frame_len  is doing what???
							uart_in_frame.length = data;
							uart_rx_info.status = TBOX_ST_TYPE;
						}
						else
            {
              uart_rx_info.status = TBOX_ST_IDLE;
            }
            break;
				
				case TBOX_ST_TYPE:	/*the first data indicate frame type:0xff is ack,others is data frame.*/
						if(0xff == data) /** ack frame **/
            {
                uart_rx_info.frame_type = data;
                uart_rx_info.cs ^= uart_rx_info.frame_type;
								--uart_rx_info.frame_len;
                uart_rx_info.status = TBOX_ST_ACK_CS;
            }
            else if(data < TBOX_FRAME_TYPE_MAX) /** data frame **/
            {
								uart_rx_info.byte_index = 0;
								uart_in_frame.buff[uart_rx_info.byte_index++] = data;
								uart_rx_info.frame_type = data;
                uart_rx_info.cs ^= uart_rx_info.frame_type;
								--uart_rx_info.frame_len;
                uart_rx_info.status = TBOX_ST_DATA;       
            }
            else
            {
                uart_rx_info.status =TBOX_ST_IDLE;
            }
						break;
						
        case TBOX_ST_DATA:
								uart_in_frame.buff[uart_rx_info.byte_index++] = data;
								uart_rx_info.cs ^= data;
	        if(uart_rx_info.frame_len > 0)
	        {
	            if(--uart_rx_info.frame_len == 0)
	            {
	                uart_rx_info.status = TBOX_ST_DATA_CS;
	            }
	        }
	        else
	        {
	            uart_rx_info.status = TBOX_ST_DATA_CS;
	        }
            break;
					
        case TBOX_ST_DATA_CS:
            if(data == uart_rx_info.cs )    /** a full frame received **/
            {
									uart_rx_info.byte_index = 0;   /**clear byte_index **/
										if(uart_rx_info.last_frame_id == uart_rx_info.frame_id)   /** is the last frame **/
										{						/**not put to fifo, only send ack again**/
																	/*received frame need ack.*/
												if( (TBOX_RESPONSE_ACK_NEEDED == uart_rx_info.frame_type)
														||(TBOX_REQ_ACK_NEEDED == uart_rx_info.frame_type) )    //   send   resend  request
												{
													uart_send_ack(uart_rx_info.frame_id);
												}
										}
										else
										{
													rx_frame_prio = uart_in_frame.buff[TBOX_CMD_PRIO];
												if(rx_frame_prio <= TBOX_UART_MSG_PRIO_NUM)
												{
															if(TRUE == fifo_input_frame(&uart_in_fifo[rx_frame_prio-1].fifo_info,&(uart_in_frame.buff[0]),uart_in_frame.length)) 
																{/**if fifo full, will not send ack, so navi will send again. **/
																		uart_rec_frame_counter++;
																		uart_rx_info.last_frame_id  = uart_rx_info.frame_id;
																	/*received frame need ack.*/
																		if( (TBOX_RESPONSE_ACK_NEEDED == uart_rx_info.frame_type)
																					||(TBOX_REQ_ACK_NEEDED == uart_rx_info.frame_type) )							
																		{
																			uart_send_ack(uart_rx_info.frame_id);
																		}
																}
															else
																{
																		//fifo is full
																}
												}
										}
            }
            uart_rx_info.status = TBOX_ST_IDLE;
            break;
						
        case TBOX_ST_ACK_CS:
             if((uart_rx_info.frame_id == uart_tx_info.frame_id)&&(data == uart_rx_info.cs)) /** a new ack received **/
             {
                 uart_lost_ack_counter = 0;
                 uart_clr_ack_alarm(); /**kill the timer **/
                 uart_tx_info.ack_wait_state = TBOX_ACK_WAIT_NONE;/**/
                 uart_out_frame.bValid = FALSE;/** uart_tbox_out_frame has been send successfully**/
                 uart_send_frame_handled_counter = uart_tx_info.frame_id;
             }
             uart_rx_info.status = TBOX_ST_IDLE;
             break ;
						 
        default:
            uart_rx_info.status = TBOX_ST_IDLE;
            break ;
		}
}
/***********************************************************************
*  Name         : uart_read_frame
*  Description : get frame from output fifo,and request sendding      
*  Parameter  : uint8_t *frame, uint8_t *length
*  Returns      : BOOL
***********************************************************************/
static BOOL uart_read_frame(uint8_t *frame, uint8_t *length)	
{
	if (!fifo_is_empty(&uart_in_fifo[0].fifo_info))
	{
		return (fifo_output_frame(&uart_in_fifo[0].fifo_info, frame,length));
	}
	else if (!fifo_is_empty(&uart_in_fifo[1].fifo_info))
	{
		return (fifo_output_frame(&uart_in_fifo[1].fifo_info, frame,length));
	}
	else if (!fifo_is_empty(&uart_in_fifo[2].fifo_info))
	{
		return (fifo_output_frame(&uart_in_fifo[2].fifo_info, frame,length));
	}
	else if (!fifo_is_empty(&uart_in_fifo[3].fifo_info))
	{
		return (fifo_output_frame(&uart_in_fifo[3].fifo_info, frame,length));
	}
	else
	{
		return FALSE;
	}
}
/***********************************************************************
*  Name         : uart_read_polling
*  Description : read from uart_tbox_in_fifo periodically.      
*  Parameter  : None
*  Returns      : None
***********************************************************************/
void uart_read_polling(void)
{
    if(uart_has_frame_rev())
    {
        uint8_t len;
        uint8_t frame[UART_FRAME_SIZE];
			if(uart_read_frame(frame,&len))
			{
	        uart_handle_frame(frame);
	    }
    }
}
/***********************************************************************
*  Name         : uart_handle_frame
*  Description : get frame from output fifo,and request sendding      
*  Parameter  : uint8_t *frame
*  Returns      : None
***********************************************************************/
static void uart_handle_frame(uint8_t *frame)
{
    uart_msg_handle(frame);
    uart_rec_frame_handled_counter++;
}

/***********************************************************************
*  Name         : uart_has_frame_rev
*  Description : to confirm if has frame received.
*  Parameter  : None
*  Returns      : BOOL
***********************************************************************/
static BOOL uart_has_frame_rev(void)
{
	BOOL ret_val =FALSE ;
	if( uart_rec_frame_counter != uart_rec_frame_handled_counter)
	  ret_val = TRUE ;
	return ret_val;
}

/***********************************************************************
*  Name         : uart_trig_tx
*  Description : to trigger the tx interrupt.
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_trig_tx(void)
{
    if(uart_is_tx_idle())
    {
        uart_start_send();
        uart_set_req_send_alarm();
        uart_tx_info.req_send_sync_byte = TRUE;
    }
}
/***********************************************************************
*  Name         : uart_set_tx_alarm
*  Description : start tx alarm   
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_set_tx_alarm(void)
{
    uart_tx_timer.timer_counter = TBOX_UART_TX_TIMEOUT_MAX;
}
/***********************************************************************
*  Name         : uart_clr_tx_alarm
*  Description : clear tx alarm  
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_clr_tx_alarm(void)
{
    uart_tx_timer.timer_counter = 0; 
    uart_tx_timer.timeout_counter = 0;
}

/***********************************************************************
*  Name         : uart_clr_rx_alarm
*  Description : clr rx alarm  
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_clr_rx_alarm(void)
{
    uart_rx_timer.timer_counter = 0;
    uart_rx_timer.timeout_counter = 0;
}

/***********************************************************************
*  Name         : uart_tbox_send_ack
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void  uart_send_ack(uint8_t frame_id)
{
    uart_tx_info.ack_id = frame_id;
    uart_tx_info.req_send_ack = TRUE;
    uart_trig_tx();
}

/***********************************************************************
*  Name         : uart_set_ack_alarm
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_set_ack_alarm(void)
{
     uart_ack_timer.timer_counter = TBOX_UART_ACK_TIMEOUT_MAX;
}
/***********************************************************************
*  Name         : uart_clr_ack_alarm
*  Description : 
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_clr_ack_alarm(void)
{
    uart_ack_timer.timer_counter = 0;
    uart_ack_timer.timeout_counter = 0;
}
/***********************************************************************
*  Name         : uart_set_enter_tx_alarm
*  Description :
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_set_enter_tx_alarm(void)
{
     uart_enter_tx_timer.timer_counter = TBOX_UART_ENTER_TX_TIMEOUT_MAX;
}
/***********************************************************************
*  Name         : uart_clr_enter_tx_alarm
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_clr_enter_tx_alarm(void)
{
    uart_enter_tx_timer.timer_counter = 0;
    uart_enter_tx_timer.timeout_counter = 0;
}
/***********************************************************************
*  Name         : uart_set_req_send_alarm
*  Description : 
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_set_req_send_alarm(void)
{
/**uart don't need synchronism mechanism,so set the value to 0.**/
	uart_req_send_timer.timer_counter = 0;//TBOX_UART_REQ_SEND_TIMEOUT_MAX;
}
/***********************************************************************
*  Name         : uart_clr_req_send_alarm
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_clr_req_send_alarm(void)
{
    uart_req_send_timer.timer_counter = 0; 
    uart_req_send_timer.timeout_counter = 0;
}
/***********************************************************************
*  Name         : uart_clr_all_send_alarm
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_clr_all_send_alarm(void)
{
    uart_clr_req_send_alarm();
    uart_clr_enter_tx_alarm();
    uart_clr_tx_alarm();
}
/***********************************************************************
*  Name         : uart_req_restart_crq
*  Description :    
*  Parameter  : None
*  Returns      : BOOL
***********************************************************************/
static BOOL uart_req_restart_crq(void)
{
     return (uart_tx_info.req_restart_crq);
}
/***********************************************************************
*  Name         : uart_crq_retry
*  Description : retry to set the crq to navi     
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_crq_retry(void)
{
    uart_end_send();
    uart_set_restart_crq();
}
/***********************************************************************
*  Name         : uart_set_restart_crq
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_set_restart_crq(void)
{
     uart_tx_info.req_restart_crq = TRUE;
}

/***********************************************************************
*  Name         : uart_clr_restart_crq
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_clr_restart_crq(void)
{
     uart_tx_info.req_restart_crq = FALSE;
}
/***********************************************************************
*  Name         : uart_end_send
*  Description :    
*  Parameter  : None
*  Returns      : None
***********************************************************************/
static void uart_end_send(void)
{
    uart_clr_all_send_alarm();
}
/**************** END OF FILE *****************************************/
