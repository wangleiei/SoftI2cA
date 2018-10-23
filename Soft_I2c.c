
#include "Soft_I2c.h"

static void I2C_start(SoftI2cA*base);
static void I2C_stop(SoftI2cA*base);
static void I2C_Nack(SoftI2cA*base);
static void I2C_ack(SoftI2cA*base);

void soft_I2C_init(SoftI2cA* base,
	VOIDFUNC  w_sda_h,//sda输出高电平
	VOIDFUNC  w_sda_l,//sda输出低电平
	VOIDFUNC  w_scl_h,//scl输出高电平
	VOIDFUNC  w_scl_l,//scl输出低电平
	INTFPTR  r_sda,//读取sda电平高低，高电平返回1，低电平返回0
	VOIDFUNC  set_sda_pp_,//设置sda为输出模式
	VOIDFUNC  set_sda_in_,//设置scl为输入模式
	VOIDFUNC  set_scl_pp_,//设置scl为输入模式
	FUNPTRINT  delayus_,//模拟延时
	uint32_t count//延时单位，用来控制i2c通信速率
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
 	base->set_scl_pp_static = set_scl_pp_;

	base->set_sda_pp_static();
	base->set_scl_pp_static();
}
static void I2C_start(SoftI2cA*base)
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
static void I2C_stop(SoftI2cA*base)
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
static void I2C_Nack(SoftI2cA*base)
{
	base->delayus_static(base->i2c_rate);
	base->write_sda_h_static();
	base->delayus_static(base->i2c_rate);

	base->write_scl_h_static();
	base->delayus_static(base->i2c_rate);

	base->write_scl_l_static();
	base->delayus_static(base->i2c_rate);
}
static uint8_t I2C_SendAddr(SoftI2cA*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t SubaNum);

static void I2C_ack(SoftI2cA*base)
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
uint8_t I2C_SendByte(SoftI2cA*base,uint8_t SendByte)
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
		
		base->delayus_static(base->i2c_rate);//scl上升沿读取电平，
		SendByte <<= 1;		
	
		base->write_scl_h_static();
		base->delayus_static(base->i2c_rate);

		base->write_scl_l_static();	
		// base->delayus_static(5);
	}
	base->set_sda_in_static();
	
	base->delayus_static(5);	
	
	base->write_scl_h_static();

	base->delayus_static(base->i2c_rate);	
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
uint8_t I2C_ReceiveByte(SoftI2cA*base)
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
static uint8_t  I2C_SendAddr(SoftI2cA*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t SubaNum)
{
	I2CAddr<<=1;
	I2CAddr&=0xFE;
	I2C_SendByte(base,I2CAddr);

	if(SubaNum != 1){
		I2C_SendByte(base,(uint8_t)(RegAddr>>8));			
	}

	I2C_SendByte(base,(uint8_t)RegAddr);				
	
	return 0;
}

uint8_t I2C_WriteNByte(SoftI2cA*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum)
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
uint8_t I2C_ReadNByte(SoftI2cA*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum)
{	

	uint8_t status;

	I2C_start(base);

	status = I2C_SendAddr(base,I2CAddr,RegAddr,SubaNum);

	I2C_start(base);							
	
	I2CAddr<<=1;
	status = I2C_SendByte(base,I2CAddr|0x01);

	while(length != 0x00){
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


