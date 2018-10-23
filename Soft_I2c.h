#ifndef __I2C_H_
#define __I2C_H_

#include <stdint.h>

typedef void (*VOIDFUNC)(void);
typedef int (*INTFPTR)(void);
typedef void (*FUNPTRINT)(int);
typedef struct SOFT_I2C1
{		
	VOIDFUNC  write_sda_h_static;  //sda输出高电平
	VOIDFUNC  write_sda_l_static;  //sda输出低电平
	VOIDFUNC  write_scl_h_static;  //scl输出高电平
	VOIDFUNC  write_scl_l_static;  //scl输出低电平
	INTFPTR  read_sda_static;  //读取sda电平高低，高电平返回1，低电平返回0
	VOIDFUNC  set_sda_pp_static;//设置sda为输出模式
	VOIDFUNC  set_sda_in_static;//设置scl为输入模式
	VOIDFUNC  set_scl_pp_static;//设置scl为输入模式
	FUNPTRINT  delayus_static;//模拟延时
	uint8_t i2c_rate;//延时单位，用来控制i2c通信速率
}SoftI2cA ;

void Softi2cA_Init(SoftI2cA * base);
// 返回0 发送成功，返回1发送失败
uint8_t I2C_SendByte(SoftI2cA *base,uint8_t SendByte);
uint8_t I2C_ReceiveByte(SoftI2cA *base);
// SubaNum 是寄存器地址长度，分为双字节地址，单字节地址
uint8_t I2C_WriteNByte(SoftI2cA *base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum);
// SubaNum 是寄存器地址长度，分为双字节地址，单字节地址
uint8_t I2C_ReadNByte(SoftI2cA *base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum);
#define SUBA1B 	1		
#define SUBA2B 	2		
#define SUBA1X 	3		

#endif


