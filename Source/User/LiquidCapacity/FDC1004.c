/*******************************************************************************
        (c) COPYRIGHT 2010-2018 by Efficient Systems, Inc.    
                          All rights reserved.
    
       This software is confidential and proprietary to Efficient 
     Systems, Inc.  No part of this software may be reproduced,    
     stored, transmitted, disclosed or used in any form or by any means
     other than as expressly provided by the written license agreement    
     between Efficient Systems and its licensee.
 FileName    : FDC1004.c
 Author      : ranwei
 Version     : 
 Date        : 2016/3/18 13:48:46:629
 Description : 
 Others      : 

 History      :
  1.Date         -- 2016/3/18 13:48:46:629
    Author       -- ranwei
    Modification -- Created file

*******************************************************************************/
    
#define  FDC1004_GLOBAL

/* includes-------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "LiquidCapacity\FDC1004.h"



/* Private typedef&macro&definde----------------------------------------------*/
#define FDC1004_SDA1_SET_INPUT()			{GPIOB->CRL &= 0x4fffffff; GPIOB->CRL |= 0x40000000;}//PB7
#define FDC1004_SDA1_SET_OUTPUT()		{GPIOB->CRL &= 0x3fffffff; GPIOB->CRL |= 0x30000000;}

#define FDC1004_SCL1_SET_INPUT()			{GPIOB->CRL &= 0xf4ffffff; GPIOB->CRH |= 0x04000000;}// 
#define FDC1004_SCL1_SET_OUTPUT()		{GPIOB->CRL &= 0xf3ffffff; GPIOB->CRH |= 0x03000000;}// PB6

#define FDC1004_SDA1_SET_VAL()			{GPIOB->BSRR = GPIO_Pin_7;}//PB7   I2C2_SDA
#define FDC1004_SDA1_CLR_VAL()			{GPIOB->BRR = GPIO_Pin_7;}

#define FDC1004_SCL1_SET_VAL()			{GPIOB->BSRR = GPIO_Pin_6;}//PB6   I2C2_SCL
#define FDC1004_SCL1_CLR_VAL()			{GPIOB->BRR = GPIO_Pin_6;}


#define FDC1004_SDA1_GET_VAL()			(GPIOB->IDR & GPIO_Pin_7)
#define FDC1004_SCL1_GET_VAL()			(GPIOB->IDR & GPIO_Pin_6)

/*Bits 2y:2y+1 MODERy[1:0]: Port x configuration bits (y = 0..15)
00: Input (reset state)
01: General purpose output mode
10: Alternate function mode
11: Analog mode
*/

#define FDC1004_SDA2_SET_INPUT()			{GPIOB->CRH &= 0xf4ffffff; GPIOB->CRH |= 0x04000000;}//PB14
#define FDC1004_SDA2_SET_OUTPUT()		{GPIOB->CRH &= 0xf3ffffff; GPIOB->CRH |= 0x03000000;}

#define FDC1004_SCL2_SET_INPUT()			{GPIOB->CRH &= 0x4fffffff; GPIOB->CRH |= 0x40000000;}// 
#define FDC1004_SCL2_SET_OUTPUT()		{GPIOB->CRH &= 0x3fffffff; GPIOB->CRH |= 0x30000000;}// PB15

#define FDC1004_SDA2_SET_VAL()			{GPIOB->BSRR = GPIO_Pin_14;}//PB14   I2C2_SDA
#define FDC1004_SDA2_CLR_VAL()			{GPIOB->BRR = GPIO_Pin_14;}

#define FDC1004_SCL2_SET_VAL()			{GPIOB->BSRR = GPIO_Pin_15;}//PB15   I2C2_SCL
#define FDC1004_SCL2_CLR_VAL()			{GPIOB->BRR = GPIO_Pin_15;}

#define FDC1004_SDA2_GET_VAL()			(GPIOB->IDR & GPIO_Pin_14)
#define FDC1004_SCL2_GET_VAL()			(GPIOB->IDR & GPIO_Pin_15)

#define FDC1004_SDA3_SET_INPUT()			{GPIOB->CRH &= 0xfff4ffff; GPIOB->CRH |= 0x00040000;}//PB12
#define FDC1004_SDA3_SET_OUTPUT()		{GPIOB->CRH &= 0xfff3ffff; GPIOB->CRH |= 0x00030000;}

#define FDC1004_SCL3_SET_INPUT()			{GPIOB->CRH &= 0xff4fffff; GPIOB->CRH |= 0x00400000;}// 
#define FDC1004_SCL3_SET_OUTPUT()		{GPIOB->CRH &= 0xff3fffff; GPIOB->CRH |= 0x00300000;}// PB13

#define FDC1004_SDA3_SET_VAL()			{GPIOB->BSRR = GPIO_Pin_12;}//PB12   I2C2_SDA
#define FDC1004_SDA3_CLR_VAL()			{GPIOB->BRR = GPIO_Pin_12;}

#define FDC1004_SCL3_SET_VAL()			{GPIOB->BSRR = GPIO_Pin_13;}//PB13   I2C2_SCL
#define FDC1004_SCL3_CLR_VAL()			{GPIOB->BRR = GPIO_Pin_13;}

#define FDC1004_SDA3_GET_VAL()			(GPIOB->IDR & GPIO_Pin_12)
#define FDC1004_SCL3_GET_VAL()			(GPIOB->IDR & GPIO_Pin_13)

#define FDC1004_I2C_WRITE_ADDR 	0xA0 //0xc4
#define FDC1004_I2C_READ_ADDR 	0xA1 //0xc5

#define ACK             1
#define NOACK           0


/*!< FDC1004 register */
#define FDC_REG_MEAS1_MSB       0x00
#define FDC_REG_MEAS1_LSB       0x01
#define FDC_REG_MEAS2_MSB       0x02
#define FDC_REG_MEAS2_LSB       0x03
#define FDC_REG_MEAS3_MSB       0x04
#define FDC_REG_MEAS3_LSB       0x05
#define FDC_REG_MEAS4_MSB       0x06
#define FDC_REG_MEAS4_LSB       0x07
#define FDC_REG_CONF_MEAS1      0x08
#define FDC_REG_CONF_MEAS2      0x09
#define FDC_REG_CONF_MEAS3      0x0A
#define FDC_REG_CONF_MEAS4      0x0B
#define FDC_REG_FDC_CONF        0x0C
#define FDC_REG_OFFSET_CAL_CIN1 0x0D
#define FDC_REG_OFFSET_CAL_CIN2 0x0E
#define FDC_REG_OFFSET_CAL_CIN3 0x0F
#define FDC_REG_OFFSET_CAL_CIN4 0x10
#define FDC_REG_GAIN_CAL_CIN1   0x11
#define FDC_REG_GAIN_CAL_CIN2   0x12
#define FDC_REG_GAIN_CAL_CIN3   0x13
#define FDC_REG_GAIN_CAL_CIN4   0x14
#define FDC_REG_Manufacturer_ID 0xFE
#define FDC_REG_Device_ID       0xFF



/* Private variables ---------------------------------------------------------*/
uint8_t buf[21];
uint16_t g_usMeas1MSB = 0;
uint16_t g_usMeas1LSB = 0;
uint16_t g_usMeas2MSB = 0;
uint16_t g_usMeas2LSB = 0;
uint16_t g_usMeas3MSB = 0;
uint16_t g_usMeas3LSB = 0;
uint16_t g_usMeas4MSB = 0;
uint16_t g_usMeas4LSB = 0;
float    g_fClevel0 = 0;
float    g_fClevel = 0;
float    g_fCrl = 0;
float    g_fCre = 0;
float    g_fHrl = 1;
float    g_fLevel = 0;


/* Private functions ---------------------------------------------------------*/
static void _FDC1004_Lowlevel_Init(void);
static void _FDC1004_I2C_Start_All(void);
static void _FDC1004_I2C1_Start(void);
static void _FDC1004_I2C_Delay(uint32_t time);
static void _FDC1004_I2C1_Stop(void);
static void _FDC1004_I2C2_Start(void);
static void _FDC1004_I2C2_Stop(void);
static void _FDC1004_I2C3_Start(void);
static void _FDC1004_I2C3_Stop(void);
static uint8_t _FDC1004_I2C1_WriteByte(uint8_t reg, uint8_t MSB, uint8_t LSB);
static uint8_t _FDC1004_I2C1_SendByte(uint8_t dat);
static void _FDC1004_I2C1_WaitCLK(void);
static uint8_t _FDC1004_I2C1_ReadReg(uint8_t reg, uint8_t length, uint8_t* buf);
static uint8_t _FDC1004_I2C1_ReadByte(uint8_t ack);




/* External functions --------------------------------------------------------*/
/* External variables --------------------------------------------------------*/





void FDC1004_Init(void)
{
    _FDC1004_Lowlevel_Init();

    _FDC1004_I2C_Start_All();
    _FDC1004_I2C1_WriteByte(FDC_REG_CONF_MEAS1, 0x10, 0x00);  // set meas 1
    _FDC1004_I2C_Delay(50);
    _FDC1004_I2C1_WriteByte(FDC_REG_CONF_MEAS2, 0x11,0x00);  // set meas 2
    _FDC1004_I2C_Delay(50);
    _FDC1004_I2C1_WriteByte(FDC_REG_CONF_MEAS3, 0x12,0x00);  // set meas 3
    _FDC1004_I2C_Delay(50);
    _FDC1004_I2C1_WriteByte(FDC_REG_CONF_MEAS4, 0x13,0x00);  // set meas 4
    _FDC1004_I2C_Delay(50);
    _FDC1004_I2C1_WriteByte(FDC_REG_FDC_CONF, 0x05,0xF0);  // trigger measurement
    _FDC1004_I2C_Delay(50);

//Read FDC1004
    _FDC1004_I2C1_ReadReg(FDC_REG_FDC_CONF,2,&buf[0]);
    if(buf[1] & 0x08)
    {
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS1_MSB, 2, (uint8_t *)&g_usMeas1MSB);
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS1_LSB, 2, (uint8_t *)&g_usMeas1LSB);
    }
    else if(buf[1] & 0x04)
    {
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS2_MSB, 2, (uint8_t *)&g_usMeas2MSB);
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS2_LSB, 2, (uint8_t *)&g_usMeas2LSB);
    }
    else if(buf[1] &  0x02)
    {
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS3_MSB, 2, (uint8_t *)&g_usMeas3MSB);
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS3_LSB, 2, (uint8_t *)&g_usMeas3LSB);
    }
    else if(buf[1] &  0x01)
    {
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS4_MSB, 2, (uint8_t *)&g_usMeas4MSB);
        _FDC1004_I2C1_ReadReg(FDC_REG_MEAS3_LSB, 2, (uint8_t *)&g_usMeas4LSB);
    }
}



float FDC1004_GetLiquidLevel(void)
{
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS1_MSB, 2, (uint8_t *)&g_usMeas1MSB);
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS1_LSB, 2, (uint8_t *)&g_usMeas1LSB);
    g_fClevel = (g_usMeas1MSB>>11) + (float)(0x07FF & g_usMeas1MSB)/2048 + (float)((g_usMeas1LSB & 0xFF00)>>8)/1048576;
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS2_MSB, 2, (uint8_t *)&g_usMeas2MSB);
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS2_LSB, 2, (uint8_t *)&g_usMeas2LSB);
    g_fCrl = (g_usMeas2MSB >> 11) + (float)(0x07FF & g_usMeas2MSB)/2048 + (float)((g_usMeas2LSB & 0xFF00)>>8)/1048576;
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS3_MSB, 2, (uint8_t *)&g_usMeas3MSB);
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS3_LSB, 2, (uint8_t *)&g_usMeas3LSB);
    g_fCre = (g_usMeas3MSB >> 11) + (float)(0x07FF & g_usMeas3MSB)/2048 + (float)((g_usMeas3LSB & 0xFF00)>>8)/1048576;
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS4_MSB, 2, (uint8_t *)&g_usMeas4MSB);
    _FDC1004_I2C1_ReadReg(FDC_REG_MEAS4_LSB, 2, (uint8_t *)&g_usMeas4LSB);    

    g_fLevel = g_fHrl * ((g_fClevel - g_fClevel0)/(g_fCrl - g_fCre));

    return g_fLevel;
}

static void _FDC1004_Lowlevel_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*  Configuration  I2C   SDA IO Config - PB7   SCL IO Config - PB6  	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   // Initialize GPIO B
	
	/*  Configuration  I2C 	 SDA IO Config - PB14   SCL IO Config - PB15   */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15|GPIO_Pin_14 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   // Initialize GPIO B
	
	/*  Configuration  I2C   SDA IO Config - PB12   SCL IO Config - PB13 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);   // Initialize GPIO B    
}


static void _FDC1004_I2C_Start_All(void)
{
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Start();
    _FDC1004_I2C1_Stop();
	
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Start();
    _FDC1004_I2C2_Stop();
		
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Start();
    _FDC1004_I2C3_Stop();
}

static void _FDC1004_I2C1_Start(void)
{
    FDC1004_SDA1_SET_INPUT();
    FDC1004_SCL1_SET_INPUT();

    _FDC1004_I2C_Delay(10);

    FDC1004_SDA1_CLR_VAL();
    FDC1004_SDA1_SET_OUTPUT();

    _FDC1004_I2C_Delay(10);

    FDC1004_SCL1_CLR_VAL();
    FDC1004_SCL1_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);
}

static void _FDC1004_I2C_Delay(uint32_t time)
{ 
  	uint32_t TimingDelay = time;
  	while(TimingDelay != 0)
  	{
        TimingDelay--;
  	}
}

static void _FDC1004_I2C1_Stop(void)
{
    FDC1004_SCL1_CLR_VAL();
    FDC1004_SCL1_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SDA1_CLR_VAL();
    FDC1004_SDA1_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SCL1_SET_VAL();
    FDC1004_SCL1_SET_INPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SDA1_SET_VAL();
    FDC1004_SDA1_SET_INPUT();
    _FDC1004_I2C_Delay(10);
}

static void _FDC1004_I2C2_Start(void)
{
    FDC1004_SDA2_SET_INPUT();
    FDC1004_SCL2_SET_INPUT();

    _FDC1004_I2C_Delay(10);

    FDC1004_SDA2_CLR_VAL();
    FDC1004_SDA2_SET_OUTPUT();

    _FDC1004_I2C_Delay(10);

    FDC1004_SCL2_CLR_VAL();
    FDC1004_SCL2_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);
}

static void _FDC1004_I2C2_Stop(void)
{
    FDC1004_SCL2_CLR_VAL();
    FDC1004_SCL2_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SDA2_CLR_VAL();
    FDC1004_SDA2_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SCL2_SET_VAL();
    FDC1004_SCL2_SET_INPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SDA2_SET_VAL();
    FDC1004_SDA2_SET_INPUT();
    _FDC1004_I2C_Delay(10);
}

static void _FDC1004_I2C3_Start(void)
{
    FDC1004_SDA3_SET_INPUT();
    FDC1004_SCL3_SET_INPUT();

    _FDC1004_I2C_Delay(10);

    FDC1004_SDA3_CLR_VAL();
    FDC1004_SDA3_SET_OUTPUT();

    _FDC1004_I2C_Delay(10);

    FDC1004_SCL3_CLR_VAL();
    FDC1004_SCL3_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);
}

static void _FDC1004_I2C3_Stop(void)
{
    FDC1004_SCL3_CLR_VAL();
    FDC1004_SCL3_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SDA3_CLR_VAL();
    FDC1004_SDA3_SET_OUTPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SCL3_SET_VAL();
    FDC1004_SCL3_SET_INPUT();
    _FDC1004_I2C_Delay(10);

    FDC1004_SDA3_SET_VAL();
    FDC1004_SDA3_SET_INPUT();
    _FDC1004_I2C_Delay(10);
}

static uint8_t _FDC1004_I2C1_WriteByte(uint8_t reg, uint8_t MSB, uint8_t LSB)
{	
	_FDC1004_I2C1_Start();
	_FDC1004_I2C1_SendByte(FDC1004_I2C_WRITE_ADDR);
	_FDC1004_I2C1_SendByte(reg);	//first send LSByte.
	_FDC1004_I2C1_SendByte(MSB);
	_FDC1004_I2C1_SendByte(LSB);
	_FDC1004_I2C1_Stop();	

	return 1;
}

static uint8_t _FDC1004_I2C1_SendByte(uint8_t dat)
{
	uint8_t sendmask = 0x80;
    while (sendmask)
    {
        if (dat & sendmask)
        {
            FDC1004_SDA1_SET_INPUT();
        }
        else
        {
            FDC1004_SDA1_SET_OUTPUT();
        }
        _FDC1004_I2C_Delay(10);
        _FDC1004_I2C_Delay(10);
        FDC1004_SCL1_SET_INPUT();/* high */
        _FDC1004_I2C1_WaitCLK();
        _FDC1004_I2C_Delay(10);
        _FDC1004_I2C_Delay(10);
        sendmask >>= 1;
        FDC1004_SCL1_SET_OUTPUT();/* low */
        _FDC1004_I2C_Delay(10);
        _FDC1004_I2C_Delay(10);
    };

    FDC1004_SDA1_SET_INPUT();/* high */
    _FDC1004_I2C_Delay(10);
    _FDC1004_I2C_Delay(10);
    FDC1004_SCL1_SET_INPUT();/* high */
    _FDC1004_I2C1_WaitCLK();
    _FDC1004_I2C_Delay(10);
    _FDC1004_I2C_Delay(10);
    sendmask = FDC1004_SDA1_GET_VAL();
    FDC1004_SCL1_SET_OUTPUT();/* low */
    _FDC1004_I2C_Delay(10);
    _FDC1004_I2C_Delay(10);
    return sendmask;
	//return ( mxt_i2c_wait_ack() );
}

static void _FDC1004_I2C1_WaitCLK(void)
{
    uint8_t i;
    i = 0;
    while ((!FDC1004_SCL1_GET_VAL()) && (++i < 0xff) );
}

static uint8_t _FDC1004_I2C1_ReadReg(uint8_t reg, uint8_t length, uint8_t* buf)
{
	_FDC1004_I2C1_Start();
	_FDC1004_I2C1_SendByte(FDC1004_I2C_WRITE_ADDR);
	_FDC1004_I2C1_SendByte(reg);
	_FDC1004_I2C1_Stop();

	_FDC1004_I2C1_Start();
	_FDC1004_I2C1_SendByte(FDC1004_I2C_READ_ADDR);
	while(length)
	{
		if(length == 1)
		{
			*buf = _FDC1004_I2C1_ReadByte(NOACK);
		}
		else
		{
			*buf++ = _FDC1004_I2C1_ReadByte(ACK);
		}
		length--;
	}
	_FDC1004_I2C1_Stop();

	return 1;
}

static uint8_t _FDC1004_I2C1_ReadByte(uint8_t ack)
{
    uint8_t readmask = 0;
    uint8_t count=8;
    FDC1004_SDA1_SET_INPUT();
    while (count--)
    {
        FDC1004_SCL1_SET_OUTPUT();/* low */
        _FDC1004_I2C_Delay(10);
        _FDC1004_I2C_Delay(10);
        readmask <<= 1;
        FDC1004_SCL1_SET_INPUT();/* high */
        _FDC1004_I2C1_WaitCLK();
        _FDC1004_I2C_Delay(10);
        _FDC1004_I2C_Delay(10);
        if (FDC1004_SDA1_GET_VAL())
        {
            readmask+=1;
        }
    };

    if(FDC1004_SDA1_GET_VAL())
    {
        FDC1004_SDA1_SET_VAL();  
    }
    else
    {
        FDC1004_SDA1_CLR_VAL();
    }
    FDC1004_SDA1_SET_OUTPUT();

    FDC1004_SCL1_SET_OUTPUT();/* low */
    if (ack)
    {
        FDC1004_SDA1_CLR_VAL();
    }
    else
    {
        FDC1004_SDA1_SET_VAL();
    }
    _FDC1004_I2C_Delay(10);
    _FDC1004_I2C_Delay(10);
    FDC1004_SCL1_SET_INPUT();/* high */
    _FDC1004_I2C1_WaitCLK();
    _FDC1004_I2C_Delay(10);
    _FDC1004_I2C_Delay(10);
    FDC1004_SCL1_SET_OUTPUT();/* low */
    _FDC1004_I2C_Delay(10);
    _FDC1004_I2C_Delay(10);
    return readmask;
}

    
/**************** (C) COPYRIGHT 2010-2018 Efficient *****END OF FILE***********/
