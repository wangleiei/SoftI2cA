#ifndef __I2C_H_
#define __I2C_H_

#include "stm32f722xx.h"

typedef void (*VOIDFUNC)(void);
typedef int (*INTFPTR)(void);
typedef void (*FUNPTRINT)(int);
typedef struct SOFT_I2C1
{		
	VOIDFUNC  write_sda_h_static;  
	VOIDFUNC  write_sda_l_static;  
	VOIDFUNC  write_scl_h_static;  
	VOIDFUNC  write_scl_l_static;  
	INTFPTR  read_sda_static;  
	uint8_t i2c_rate;
	FUNPTRINT  delayus_static;
	VOIDFUNC  set_sda_pp_static;
	VOIDFUNC  set_sda_in_static;
	VOIDFUNC  set_scl_pp_static;
}SOFT_I2C_STR;

void soft_I2C_init(SOFT_I2C_STR* base,
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
	);
// 返回0 发送成功，返回1发送失败
uint8_t I2C_SendByte(SOFT_I2C_STR*base,uint8_t SendByte);
uint8_t I2C_ReceiveByte(SOFT_I2C_STR*base);
// SubaNum 是寄存器地址长度，分为双字节地址，单字节地址
uint8_t I2C_WriteNByte(SOFT_I2C_STR*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum);
// SubaNum 是寄存器地址长度，分为双字节地址，单字节地址
uint8_t I2C_ReadNByte(SOFT_I2C_STR*base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum);
#define SUBA1B 	1		
#define SUBA2B 	2		
#define SUBA1X 	3		

#endif


