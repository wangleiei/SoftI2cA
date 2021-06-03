#ifndef __I2C_H_
#define __I2C_H_

#include <stdint.h>
// 如果手册上写的是bit7-bit0-->0100-001rw，则该协议需要使用地址是0X21
typedef void (*VOIDFUNC)(void);
typedef int (*INTFPTR)(void);
typedef void (*FUNPTRINT)(int);
typedef enum {ADDR_8_BITS = 1,ADDR_16_BITS = 2}REGADDRLEN;

typedef struct SOFT_I2C1
{		
	VOIDFUNC  write_sda_h_static;  //sda输出高电平
	VOIDFUNC  write_sda_l_static;  //sda输出低电平
	VOIDFUNC  write_scl_h_static;  //scl输出高电平
	VOIDFUNC  write_scl_l_static;  //scl输出低电平
	INTFPTR   read_sda_static;  //读取sda电平高低，高电平返回1，低电平返回0
	VOIDFUNC  set_sda_pp_static;//设置sda为输出模式
	VOIDFUNC  set_sda_in_static;//设置scl为输入模式
	VOIDFUNC  set_scl_pp_static;//设置scl为输入模式
	FUNPTRINT  delayus_static;//模拟延时
	uint32_t i2c_rate;//延时单位，用来控制i2c通信速率
	uint16_t  dev_addr;//设备地址
	REGADDRLEN reg_addrlen;  //寄存器的地址长度，有些设备的寄存器比较多，地址可能多大16位  
}SoftI2cA ;
/**********************************************************************************************************
*	函 数 名: void Softi2cA_Init(SoftI2cA * base)
*	功能说明: 初始换i2c通信协议
*	传    参: base:结构体指针，该结构体已经填充操作函数，延时值，
*	返 回 值: 无
*   说    明: 
历程：
	SoftI2cA i2c_dev;
	i2c_dev.write_sda_h_static = p2_sda_h;//sda输出高电平
	i2c_dev.write_sda_l_static = p2_sda_l;//sda输出低电平
	i2c_dev.write_scl_h_static = p2_scl_h;//scl输出高电平
	i2c_dev.write_scl_l_static = p2_scl_l;//scl输出低电平
	i2c_dev.read_sda_static = (INTFPTR)p2_read_sda;//读取sda电平高低，高电平返回1，低电平返回0
	i2c_dev.set_sda_pp_static = p2_sda_pp;//设置sda为输出模式
	i2c_dev.set_sda_in_static = p2_sda_in;//设置scl为输入模式
	i2c_dev.set_scl_pp_static = p2_scl_pp;//设置scl为输入模式
	i2c_dev.delayus_static = block_delayus;//模拟延时
	i2c_dev.i2c_rate = 10;//延时单位，用来控制i2c通信速率
	i2c_dev.dev_addr = 0x21;//设备地址
	i2c_dev.reg_addrlen = ADDR_8_BITS;//寄存器地址长度
	Softi2cA_Init(&i2c_dev);
*********************************************************************************************************/
void Softi2cA_Init(SoftI2cA * base);

/**********************************************************************************************************
*	函 数 名: uint8_t I2C_SendByte(SoftI2cA * base,uint8_t SendByte);
*	功能说明: i2c发送数据
*	传    参: base:结构体指针
			  SendByte：发送的数据，一个字节
*	返 回 值: 返回0 发送成功，返回1发送失败
*   说    明: 
*********************************************************************************************************/
uint8_t I2C_SendByte(SoftI2cA * base,uint8_t SendByte);
/**********************************************************************************************************
*	函 数 名: uint8_t I2C_ReceiveByte(SoftI2cA * base);
*	功能说明: 读取i2c的数据
*	传    参: base:结构体指针
*	返 回 值: 返回的数据，长度是1字节
*   说    明: 
*********************************************************************************************************/
uint8_t I2C_ReceiveByte(SoftI2cA * base);
/**********************************************************************************************************
*	函 数 名: uint8_t I2C_WriteNByte(SoftI2cA * base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum);
*	功能说明: 
*	传    参: base:结构体指针
			  RegAddr：要操作的寄存器地址
			  pBuffer：发送数据缓冲区
			  length：发送数据长度
*	返 回 值: 0：成功，1失败
*   说    明: 
*********************************************************************************************************/
uint8_t I2C_WriteNByte(SoftI2cA * base,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length);
/**********************************************************************************************************
*	函 数 名: uint8_t I2C_ReadNByte(SoftI2cA * base,uint16_t I2CAddr,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length,uint8_t SubaNum);
*	功能说明: 
*	传    参: base:结构体指针
			RegAddr：要操作的寄存器地址
			pBuffer：接收数据缓冲区
			length：接收数据长度
*	返 回 值: 0：成功，1失败
*   说    明: 
*********************************************************************************************************/
uint8_t I2C_ReadNByte(SoftI2cA * base,uint16_t RegAddr,uint8_t *pBuffer,uint16_t length);
		

// 这个是专门为了一些不遵守标准IIC的烂东西做的接口 比如AHT10的读温度数据,传入设备地址，就能得到数据
uint8_t AhtIicRead(SoftI2cA*base,uint8_t *pBuffer,uint16_t length);

// 这个是专门为了一些不遵守标准IIC的烂东西做的接口 比如HTU21D的读温度数据(非主机保持模式)
// 是两个一套的
void Htu21dIicRead_write_devaddr(SoftI2cA*base,uint16_t RegAddr);
void Htu21dIicRead_read_devaddr(SoftI2cA*base,uint8_t *pBuffer,uint16_t length);
#endif


