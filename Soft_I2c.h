#ifndef __I2C_H_
#define __I2C_H_

#include "stm8l15x.h"

typedef void (*write_sda_h)(void);
typedef void (*write_sda_l)(void);
typedef void (*write_scl_h)(void);
typedef void (*write_scl_l)(void);
typedef void (*set_sda_pp)(void);
typedef void (*set_sda_in)(void);
typedef int (*read_sda)(void);
typedef void (*delayus)(int);
typedef struct SOFT_I2C1
{		
	write_sda_h  write_sda_h_static;  
	write_sda_l  write_sda_l_static;  
	write_scl_h  write_scl_h_static;  
	write_scl_l  write_scl_l_static;  
	read_sda  read_sda_static;  
	uint8_t i2c_rate;
	delayus  delayus_static;
	set_sda_pp  set_sda_pp_static;
	set_sda_in  set_sda_in_static;
}SOFT_I2C_STR;

void soft_I2C_init(SOFT_I2C_STR* base,
	write_sda_h  w_sda_h,//sda输出高电平
	write_sda_l  w_sda_l,//sda输出低电平
	write_scl_h  w_scl_h,//scl输出高电平
	write_scl_l  w_scl_l,//scl输出低电平
	read_sda  r_sda,//读取sda电平高低，高电平返回1，低电平返回0
	set_sda_pp  set_sda_pp_,//设置sda为输出模式
	set_sda_in  set_sda_in_,//设置scl为输入模式
	delayus  delayus_,//模拟延时
	uint8_t count//延时单位，用来控制i2c通信速率
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


