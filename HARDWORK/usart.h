/**
  ******************************************************************************
  * 文件: usart.h
  * 描述: 
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#ifndef __USART_H
#define __USART_H
#include "board.h"

void USART_Init(void);

//void Usart_SendDate(unsigned char data);
//void Uart_Sendstrings(char *pt) ;
void package_frame(const void *data);
#endif
