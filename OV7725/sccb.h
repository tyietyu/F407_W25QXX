#ifndef __SCCB_H
#define __SCCB_H
#include "main.h"
#include "gpio.h"

#define SCCB_READ_SDA    HAL_GPIO_ReadPin(GPIOC, OV_SDA_Pin)
#define SCCB_ID          0X42 // OV7670µÄID

///////////////////////////////////////////
void SCCB_Init(void);
void SCCB_Start(void);
void SCCB_Stop(void);
void SCCB_No_Ack(void);
uint8_t SCCB_WR_Byte(uint8_t dat);
uint8_t SCCB_RD_Byte(void);
uint8_t SCCB_WR_Reg(uint8_t reg, uint8_t data);
uint8_t SCCB_RD_Reg(uint8_t reg);
#endif
