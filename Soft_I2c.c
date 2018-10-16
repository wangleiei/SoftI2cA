
#include "Soft_I2c.h"

static void I2C_start(SOFT_I2C_STR*base);
static void I2C_stop(SOFT_I2C_STR*base);
static void I2C_Nack(SOFT_I2C_STR*base);
static void I2C_ack(SOFT_I2C_STR*base);

void soft_I2C_init(SOFT_I2C_STR* base,
	write_sda_h  w_sda_h,
	write_sda_l  w_sda_l,
	write_scl_h  w_scl_h,
	write_scl_l  w_scl_l,
	read_sda  r_sda,
	set_sda_pp  set_sda_pp_,
	set_sda_in  set_sda_in_,
	delayus  delayus_,
	uint8_t count
	)
{
 	base->write_sda_h_static = w_sda_h;
 	base->write_sda_l_static = w_sda_l;
 	base->write_scl_h_static = w_scl_h;
 	base->write_scl_l_static = w_scl_l;
 	base->read_sda_static = r_sda;
 	base->delayus_static = delayus_;
 	base->i2c_rate = count;
 	base->set_sda_pp_static = set_sda_pp_;
 	base->set_sda_in_static = set_sda_in_;
}
static void I2C_start(SOFT_I2C_STR*base)
{	
	
	base->write_scl_l_static();
	base->delayus_static(base->i2c_rate);

	base->write_sda_h_static();	
	base->delayus_static(base->i2c_rate);

	base->write_scl_h_static();
	base->delayus_static(base->i2c_rate);

	base->write_sda_l_static();	
	base->delayus_static(base->i2c_rate);

	base->write_scl_l_static();	
	base->delayus_static(base->i2c_rate);
}
static void I2C_stop(SOFT_I2C_STR*base)
{
	base->write_scl_l_static();	
	base->delayus_static(base->i2c_rate);

	base->write_sda_l_static();	
	base->delayus_static(base->i2c_rate);

	base->write_scl_h_static();
	base->delayus_static(base->i2c_rate);

	base->write_sda_h_static();	
	base->delayus_static(base->i2c_rate);
}
static void I2C_Nack(SOFT_I2C_STR*base)
{
	base->delayus_static(base->i2c_rate);
	base->write_sda_h_static();
	base->delayus_static(base->i2c_rate);

	base->write_scl_h_static();
	base->delayus_static(base->i2c_rate);

	base->write_scl_l_static();
	base->delayus_static(base->i2c_rate);
}
static uint8_t I2C_SendAddr(SOFT_I2C_STR*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t SubaNum);

static void I2C_ack(SOFT_I2C_STR*base)
{

	base->delayus_static(base->i2c_rate);
	base->write_sda_l_static();
	
	base->delayus_static(base->i2c_rate);
	base->write_scl_h_static();
	
	base->delayus_static(base->i2c_rate);
	base->write_scl_l_static();

	base->delayus_static(base->i2c_rate);
}
// 返回0 发送成功，返回1发送失败
uint8_t I2C_SendByte(SOFT_I2C_STR*base,uint8_t SendByte)
{
	uint8_t status;
	uint16_t count;
	
	for(count=8; count>0;count--)
	{
		if((SendByte&0x80) == 0x80)
		{
			base->write_sda_h_static();
		}	
		else
		{
			base->write_sda_l_static();
		}	
		
		// base->delayus_static(base->i2c_rate);
		SendByte <<= 1;		
	
		base->write_scl_h_static();
		base->delayus_static(base->i2c_rate);

		base->write_scl_l_static();	
		base->delayus_static(base->i2c_rate);
	}
	base->set_sda_in_static();
	
	base->delayus_static(5);	
	
	base->write_scl_h_static();
	
	status = 0;	
	
	count = 100;	
	while(base->read_sda_static() == 1)
	{
		count --;
		if(count == 0)
		{
			status = 1;	
			break;
		}
	}
	
	base->write_scl_l_static();	
	
	base->delayus_static(base->i2c_rate);
		
	base->set_sda_pp_static();
	
	base->delayus_static(base->i2c_rate);		
	
	return(status);
}
uint8_t I2C_ReceiveByte(SOFT_I2C_STR*base)
{

	
	uint8_t count;	
	uint8_t Rdata = 0;
	
	base->set_sda_in_static();

	base->delayus_static(20);

	for(count=8; count>0; count--)
	{
		Rdata <<= 1;

		base->write_scl_h_static();
		base->delayus_static(base->i2c_rate);

		if(base->read_sda_static() !=0)
		{
			Rdata |= 0x01;
		}
		
		base->write_scl_l_static();	
		base->delayus_static(base->i2c_rate);				
	}
	base->set_sda_pp_static();

	return(Rdata);
}
static uint8_t  I2C_SendAddr(SOFT_I2C_STR*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t SubaNum)
{
	I2CAddr<<=1;
	if(SubaNum==SUBA1X)		
	{
		I2C_SendByte(base,I2CAddr | ((RegAddr>>8)<<1));
	}
	else
	{
		I2C_SendByte(base,I2CAddr);	
		I2C_SendByte(base,(uint8_t)(RegAddr>>8));			
	}

	I2C_SendByte(base,(uint8_t)RegAddr);				
	
	return 0;
}

uint8_t I2C_WriteNByte(SOFT_I2C_STR*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum)
{
	uint8_t status;

	I2C_start(base);

	status = I2C_SendAddr(base,I2CAddr,RegAddr,SubaNum);

	while(length != 0x00)
	{
		status = I2C_SendByte(base,*pBuffer);
		(uint8_t*)pBuffer ++;
		RegAddr++;
		if ((RegAddr % 16) == 0)
		{
			I2C_stop(base);	
			base->delayus_static(100);
			I2C_start(base);
			status = I2C_SendAddr(base,I2CAddr,RegAddr,SubaNum);
		}
		length --;
		base->delayus_static(base->i2c_rate);
	}
	I2C_stop(base);	

	return(status);
}
uint8_t I2C_ReadNByte(SOFT_I2C_STR*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum)
{	
	uint8_t status;

	I2C_start(base);

	status = I2C_SendAddr(base,I2CAddr,RegAddr,SubaNum);

	I2C_start(base);							
	
	I2CAddr<<=1;
	if(SubaNum==SUBA1X)
	{
		status = I2C_SendByte(base,(I2CAddr|0x01) + ((RegAddr>>8)<<1));		
	}
	else
	{
		status = I2C_SendByte(base,I2CAddr|0x01);			
	}

	while(length != 0x00)
	{
		*(uint8_t*)pBuffer = I2C_ReceiveByte(base);
		if(length != 1)	
		{
			I2C_ack(base);	
		}

		(uint8_t*)pBuffer ++;
		length --;	
	}
	I2C_Nack(base);
	I2C_stop(base);
	base->delayus_static(base->i2c_rate);
	return(status);
}


