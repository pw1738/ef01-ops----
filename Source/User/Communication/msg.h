/***********************************************************************
*   Copyright (c) 2015, Shenzhen YIFEIXING Co., Ltd.				   *
*				  All Rights Reserved.          					   *
*   Department : RN R&D SW1      									   *
*   AUTHOR	   :            										   *
************************************************************************
* Object        : 
* Module        : 
* Instance      : 
* Description   : This file is about the instructions of remotecontroller
									These instructions are only adapt to the Xbee wireless module
*-----------------------------------------------------------------------
* Version: V1.0
* Date: 
* Author: wangwei
***********************************************************************/
/*-History--------------------------------------------------------------
* Version       Date    Name      Changes and comments
* V1.0				20151215	wangwei
*=====================================================================*/
#ifndef _MSG_H
#define _MSG_H
#include "stm32f10x.h"

//								 			 Message Type  |Component ID    MessageSequence|     message Size    Message Priority     Data1  Data2  Data3  Data4                                                                                                    
u8 heartbeat_msg[9] = {0x01, 0x01, 0x01, 0x05, 0x03, 0x00, 0x00, 0x00, 0x00};


//																																																							
u8 response_msg[6] =  {0x02, 0x02, 0x01, 0x02, 0x03, 0x00,};


//								 			  Message Type  |Component ID    MessageSequence|     message Size    Message Priority    Data1  Data2 Data3// status of spray	
//																		 |        Message ID             |																																																				
// u8 heartbeat_msg[10] = 			{  	0x01, 		    0x01, 0x01,        				0x10,         0x00,        0x00,            0x00,     0x03,0x66, 0x00       } ;


u8 key_error[10] = 					{ 	0xff , 			0xff, 					0xff, 							0xff, 						0xff, 					0xff,  0xff, 	0xff,  0xff, 0xff } ;//

#endif

