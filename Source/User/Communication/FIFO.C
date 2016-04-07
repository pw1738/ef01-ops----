/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : Fifo.c
 Author      : rw
 Version     : 
 Date        : 2016/3/8 22:33:59:33
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/8 22:33:59:33
    Author       -- rw
    Modification -- Created file

*******************************************************************************/
    
#define  FIFO_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "serial_debug\serial_debug.h"
#include "stm32f10x.h"
#include "Fifo.h"
    
/* Private typedef&macro&definde----------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static BOOL _Fifo_Input_Byte( ST_Fifo_Type *this ,unsigned char byte);
static BOOL _Fifo_Output_Byte(ST_Fifo_Type *this, uint8_t *pdata);
static uint16_t _Fifo_Current_Length(ST_Fifo_Type *this);
static uint16_t _Fifo_Have_Enough_Space(ST_Fifo_Type *this ,uint8_t nBytes);


/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/
    

void Fifo_Init(ST_Fifo_Type *this, uint8_t *pBuf, uint16_t nSize)
{
    this->pucFifoBuffer = pBuf;
    this->usFifoReadPtr = 0x00;
    this->usFifoWritePtr = 0x00;
    this->usFifoSize = nSize;
}


BOOL Fifo_Input_Frame(ST_Fifo_Type *this, uint8_t *pFrame ,uint8_t length)
{
  	uint8_t iLoop;
  	uint8_t *pTemp = pFrame;

    if(!_Fifo_Have_Enough_Space(this, length))
    {
        return FALSE;
    }

    if(!_Fifo_Input_Byte(this, length))
    {
        return FALSE;
    }

    for(iLoop = 0x00; iLoop < length; iLoop++) //input (1 +nbyte) byte to FIFO
    {    
    	if(!_Fifo_Input_Byte(this, *pTemp++))
    	{
    	    return FALSE;
    	}
    }
    return TRUE;
}


BOOL Fifo_Output_Frame(ST_Fifo_Type *this, uint8_t *pFrame, uint8_t *pLength)
{
  	uint8_t iLoop;
  	uint8_t tempVal;
    uint16_t tempLength;
//    uint8_t tempFrameLength;
    uint8_t *pTemp = pFrame;

    tempLength = _Fifo_Current_Length(this);  

    /*frist byte is the length of current frame*/
    /*if(_Fifo_Output_Byte(this, &tempFrameLength))
    {
        *pTemp++ = tempFrameLength;
    }
    else
    {
        return FALSE;
    }*/

    if(!_Fifo_Output_Byte(this, pLength))
    {
        return FALSE;
    }
        
    if(*pLength > tempLength || *pLength == 0x00)
    {
        this->usFifoReadPtr = this->usFifoWritePtr;
        return FALSE;
    }
    else
    {
        for (iLoop = 0x00; iLoop < *pLength; iLoop++)
        {
        	if (_Fifo_Output_Byte(this, &tempVal))
        	{
        		*pTemp++ = tempVal;
        	}
    		else
    		{
    			return FALSE;
    		}
        }        
    }

	return TRUE;
}


BOOL Fifo_IS_Empty(ST_Fifo_Type *this)
{
    if(this->usFifoReadPtr == this->usFifoWritePtr)
    {
        return TRUE;
    }
	else
	{
		return FALSE;
	}	
}


BOOL Fifo_IS_Full(ST_Fifo_Type *this)
{
	if (this->usFifoReadPtr > 0x00)
	{
		if (this->usFifoWritePtr == (this->usFifoReadPtr - 0x01))
		{
			return TRUE;
		}
	}
	else if (this->usFifoWritePtr == (this->usFifoSize - 0x01))
	{
		return TRUE;
	}
	
	return FALSE;
}


static BOOL _Fifo_Input_Byte(ST_Fifo_Type *this, unsigned char byte)
{
    if(!Fifo_IS_Full(this))
    {
        if(this->usFifoWritePtr < this->usFifoSize)
	    {
            this->pucFifoBuffer[this->usFifoWritePtr] = byte;
            this->usFifoWritePtr++;

            if(this->usFifoWritePtr >= this->usFifoSize)
            {
                this->usFifoWritePtr = 0x00;
            }
            return TRUE;
	    }
    }
    return FALSE;
}


static BOOL _Fifo_Output_Byte(ST_Fifo_Type *this, uint8_t *pdata)
{
    if (!Fifo_IS_Empty(this))
    {
        if (this->usFifoReadPtr < this->usFifoSize)
        {
	    	*pdata = this->pucFifoBuffer[this->usFifoReadPtr];
	    	this->usFifoReadPtr++;
            if(this->usFifoReadPtr >= this->usFifoSize)
            {
                this->usFifoReadPtr = 0x00;
            }
		    return TRUE;
		}
	}
	return FALSE;
}


static uint16_t _Fifo_Current_Length(ST_Fifo_Type *this)
{
  	uint16_t retVal;

    if(this->usFifoWritePtr > this->usFifoReadPtr)
    {
        retVal = this->usFifoWritePtr - this->usFifoReadPtr;
    }
    else
    {
        if (this->usFifoReadPtr == this->usFifoWritePtr)
        {
            retVal = 0x00; 
        }
        else
        {
            retVal = this->usFifoSize - (this->usFifoReadPtr - this->usFifoWritePtr);   
        }
    }
	return (retVal);
}


static uint16_t _Fifo_Have_Enough_Space(ST_Fifo_Type *this , uint8_t nBytes)
{
    /*this buffer have one byte canot be used.
      we should input one length byte + n data byte to FIFO buffer*/
    if((this->usFifoSize - _Fifo_Current_Length(this) - 0x01) >=  (nBytes + 0x01))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
