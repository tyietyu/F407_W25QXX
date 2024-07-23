#ifndef _OV7725_H
#define _OV7725_H
#include "main.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////
//ALIENTEK MiniSTM32������
//OV7725 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2017/11/1
//�汾��V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
 
#define OV7725_MID				0X7FA2    
#define OV7725_PID				0X7721

#define OV7725_WRST_LOW		HAL_GPIO_WritePin(GPIOA,WRST_Pin,GPIO_PIN_RESET)		//дָ�븴λ
#define OV7725_WRST_HIGH	HAL_GPIO_WritePin(GPIOA,WRST_Pin,GPIO_PIN_SET)
#define OV7725_RCK_LOW		HAL_GPIO_WritePin(GPIOA,PCLK_Pin,GPIO_PIN_RESET)		//������ʱ��
#define OV7725_RCK_HIGH		HAL_GPIO_WritePin(GPIOA,PCLK_Pin,GPIO_PIN_SET)
#define OV7725_RRST_LOW		HAL_GPIO_WritePin(GPIOA,RRST_Pin,GPIO_PIN_RESET) 		//��ָ�븴λ
#define OV7725_RRST_HIGH	HAL_GPIO_WritePin(GPIOA,RRST_Pin,GPIO_PIN_SET)
#define OV7725_CS_LOW		HAL_GPIO_WritePin(GPIOA,OE_Pin,GPIO_PIN_RESET)   		//Ƭѡ�ź�(OE)
#define OV7725_CS_HIGH		HAL_GPIO_WritePin(GPIOA,OE_Pin,GPIO_PIN_SET)
#define OV7725_WREN_LOW		HAL_GPIO_WritePin(GPIOA,WEN_Pin,GPIO_PIN_RESET) 		//д��FIFOʹ��
#define OV7725_WREN_HIGH	HAL_GPIO_WritePin(GPIOA,WEN_Pin,GPIO_PIN_SET)

#define OV7725_VSYNC  	HAL_GPIO_ReadPin(GPIOA,VSYNC_Pin)		//ͬ���źż��IO															  					 
#define OV7725_DATA   GPIOB->IDR&0x00FF //��������˿�
/////////////////////////////////////////									
	    				 
uint8_t   OV7725_Init(void);		  	   		 
void OV7725_Light_Mode(uint8_t mode);
void OV7725_Color_Saturation(int8_t sat);
void OV7725_Brightness(int8_t bright);
void OV7725_Contrast(int8_t contrast);
void OV7725_Special_Effects(uint8_t eft);
void OV7725_Window_Set(uint16_t width,uint16_t height,uint8_t mode);


#endif





















