




/**** System include files ****/
#include <stdint.h>
#include "stm32f10x.h"
/*---Public include files---------------------------------------------*/

/*---Private include files--------------------------------------------*/
#include "Communication\FIFO_IF.H" 
#include "Communication\FIFO.H"   

/*===VARIABLES========================================================*/

/*---Global-----------------------------------------------------------*/

/*---Private----------------------------------------------------------*/
 
/*===FUNCTIONS========================================================*/

/*---Global-----------------------------------------------------------*/
 
/*******************************************************************************
**  FUNCTION      : fifo_init                                         
**  DESCRIPTION   : This function initialize FIFO
**  PARAMETERS    : this
**                  pBuf
**                  nSize  
**  RETURN        : void                                                          
*******************************************************************************/
void fifo_init(  FIFO_TYPE *this, uint8_t *pBuf, u16 nSize)
{
    this->mpFIFOBuffer = pBuf;
    this->mFIFOHead = 0x00;
	this->mFIFOTail = 0x00;
	this->mFIFOSize = nSize;
}

/*******************************************************************************
**  FUNCTION      : fifo_input_frame                                         
**  DESCRIPTION   : This function input a frame to fifo buffer
**  PARAMETERS    : this
**                  pFrame
**                  nBytes  
**  RETURN        : void                                                          
*******************************************************************************/
BOOL fifo_input_frame( FIFO_TYPE *this , uint8_t *pFrame , uint8_t length)
{
  	uint8_t i;
  	uint8_t *pTemp = pFrame;

    if(!fifo_have_enough_space(this, length))
    {
        return FALSE;
    }

    if(!fifo_input_byte(this, length))
    {
        return FALSE;
    }

    for (i = 0x00; i < length; i++) //input (1 +nbyte) byte to FIFO
    {    
    	if (!fifo_input_byte(this, *pTemp++))
    	{
    	    return FALSE;
    	}
    }
    return TRUE;
}

/*******************************************************************************
**  FUNCTION      : fifo_output_frame                                         
**  DESCRIPTION   : This function output a frame from FIFO buffer
**  PARAMETERS    : this
**                  pFrame
**                  nBytes  
**  RETURN        : void                                                          
*******************************************************************************/
BOOL fifo_output_frame( FIFO_TYPE *this,  uint8_t *pFrame,  uint8_t *pLength)
{
  	uint8_t i;
  	uint8_t tempVal;
    u16 tempLength;
//    uint8_t tempFrameLength;
    uint8_t *pTemp = pFrame;

    tempLength = fifo_current_length(this);  

    /*frist byte is the length of current frame*/
    /*if(fifo_output_byte(this, &tempFrameLength))
    {
        *pTemp++ = tempFrameLength;
    }
    else
    {
        return FALSE;
    }*/

    if(!fifo_output_byte(this, pLength))
    {
        return FALSE;
    }
        
        
    if(*pLength > tempLength || *pLength == 0x00)
    {
       this->mFIFOHead = this->mFIFOTail;
       return FALSE;
    }
    else
    {
        for (i = 0x00; i < *pLength; i++)
        {
        	if (fifo_output_byte(this, &tempVal))
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


/*---Private----------------------------------------------------------*/

/*******************************************************************************
**  FUNCTION      : fifo_is_empty                                         
**  DESCRIPTION   : This function check whether the FIFO is empty.
**  PARAMETERS    : this:
**  RETURN        : BOOL                                                          
*******************************************************************************/
BOOL fifo_is_empty(FIFO_TYPE *this  )
{
    if(this->mFIFOHead == this->mFIFOTail)
    {
        return TRUE;
    }
	else
	{
		return FALSE;
	}	
}

/*******************************************************************************
**  FUNCTION      : fifo_is_full                                         
**  DESCRIPTION   : This function check whether the FIFO is full.
**  PARAMETERS    : this
**  RETURN        : BOOL                                                          
*******************************************************************************/
static BOOL fifo_is_full(FIFO_TYPE *this  )
{
	if (this->mFIFOHead > 0x00)
	{
		if (this->mFIFOTail == (this->mFIFOHead - 0x01))
		{
			return TRUE;
		}
	}
	else if (this->mFIFOTail == (this->mFIFOSize - 0x01))
	{
		return TRUE;
	}
	
	return FALSE;
}

/*******************************************************************************
**  FUNCTION      : fifo_input_byte                                         
**  DESCRIPTION   : This function input a byte into FIFO.
**  PARAMETERS    : this
**                  byte
**  RETURN        : BOOL
*******************************************************************************/
static BOOL fifo_input_byte( FIFO_TYPE *this , unsigned char byte )
{
    if (!fifo_is_full(this))
    {
        if (this->mFIFOTail < this->mFIFOSize)
	    {
            this->mpFIFOBuffer[this->mFIFOTail] = byte;
            this->mFIFOTail++;

            if(this->mFIFOTail >= this->mFIFOSize)
            {
                this->mFIFOTail = 0x00;
            }
            return TRUE;
	    }
    }
    return FALSE;
}

/*******************************************************************************
**  FUNCTION      : fifo_input_byte                                         
**  DESCRIPTION   : This function input a byte into FIFO.
**  PARAMETERS    : this
**                  byte
**  RETURN        : BOOL
*******************************************************************************/
static BOOL fifo_output_byte(FIFO_TYPE *this, uint8_t *pdata)
{
    if (!fifo_is_empty(this))
    {
		if (this->mFIFOHead < this->mFIFOSize)
	  {
	    	*pdata = this->mpFIFOBuffer[this->mFIFOHead];
	    	this->mFIFOHead++;
            if(this->mFIFOHead >= this->mFIFOSize)
            {
						this->mFIFOHead = 0x00;
            }
		    return TRUE;
		}
	}
	return FALSE;
}

/*******************************************************************************
**  FUNCTION      : fifo_current_length                                         
**  DESCRIPTION   : This function return the current length of FIFO buffer
**  PARAMETERS    : this
**  RETURN        : u16
*******************************************************************************/
static u16 fifo_current_length(FIFO_TYPE *this  )
{
  	u16 retVal;

    if(this->mFIFOTail > this->mFIFOHead)
    {
        retVal = this->mFIFOTail - this->mFIFOHead;
    }
    else
    {
        if (this->mFIFOHead == this->mFIFOTail)
        {
            retVal = 0x00; 
        }
        else
        {
            retVal = this->mFIFOSize - (this->mFIFOHead - this->mFIFOTail);   
        }
    }
	return (retVal);
}

/*******************************************************************************
**  FUNCTION      : fifo_have_enough_space                                        
**  DESCRIPTION   : This function judge fifo buffer have enough space to
                    the current length of FIFO buffer
**  PARAMETERS    : this
**  RETURN        : u16
*******************************************************************************/
static u16 fifo_have_enough_space(FIFO_TYPE *this , uint8_t nBytes)
{
    /*this buffer have one byte canot be used.
      we should input one length byte + n data byte to FIFO buffer*/
    if((this->mFIFOSize - fifo_current_length(this) - 0x01) >=  (nBytes + 0x01))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/**************** END OF FILE *****************************************/

