/*
*********************************************************************************************************
*
*	模块名称 : MODEBUS 通信模块 (从站）
*	文件名称 : modbus_slave.h
*	版    本 : V1.5
*	说    明 : 头文件
*
*	Copyright (C), 2020-2030, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __MODBUY_SLAVE_H
#define __MODBUY_SLAVE_H
#include "main.h"

#define SADDR485	1
#define SBAUD485	UART3_BAUD

/* 01H 读强制单线圈 */
/* 05H 写强制单线圈 */
#define REG_D01		0x0101
#define REG_D02		0x0102
#define REG_D03		0x0103
#define REG_D04		0x0104
#define REG_DXX 	REG_D04

/* 02H 读取输入状态 */
#define REG_T01		0x0201
#define REG_T02		0x0202
#define REG_T03		0x0203
#define REG_TXX		REG_T03

/* 03H 读保持寄存器 */
/* 06H 写保持寄存器 */
/* 10H 写多个保存寄存器 */
#define SLAVE_REG_P01		0x0301
#define SLAVE_REG_P02		0x0302

/* 04H 读取输入寄存器(模拟信号) */
#define REG_A01		0x0401
#define REG_AXX		REG_A01


/* RTU 应答代码 */
#define RSP_OK				0		/* 成功 */
#define RSP_ERR_CMD			0x01	/* 不支持的功能码 */
#define RSP_ERR_REG_ADDR	0x02	/* 寄存器地址错误 */
#define RSP_ERR_VALUE		0x03	/* 数据值域错误 */
#define RSP_ERR_WRITE		0x04	/* 写入失败 */

#define S_RX_BUF_SIZE		30
#define S_TX_BUF_SIZE		128

typedef struct
{
	uint8_t RxBuf[S_RX_BUF_SIZE];
	uint8_t RxCount;
	uint8_t RxStatus;
	uint8_t RxNewFlag;

	uint8_t RspCode;

	uint8_t TxBuf[S_TX_BUF_SIZE];
	uint8_t TxCount;
}MODS_T;

typedef struct
{
	/* 03H 06H 读写保持寄存器 */
	uint16_t P01;
	uint16_t P02;

	/* 04H 读取模拟量寄存器 */
	uint16_t A01;

	/* 01H 05H 读写单个强制线圈 */
	uint16_t D01;
	uint16_t D02;
	uint16_t D03;
	uint16_t D04;

}VAR_T;

void MODS_Poll(void);

extern MODS_T g_tModS;
extern VAR_T g_tVar;
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
