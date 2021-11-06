/**
  ******************************************************************************
  * 文件: control.h
  * 描述: 
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "board.h"

void balance_Init(void);
void Balance_Battery();
void Balancing_GPIO_Control(uint8_t cell_balancing_gpio_bitmask);

#endif
