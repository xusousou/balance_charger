/**
  ******************************************************************************
  *	文件: error.c
  * 描述: 错误记录
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#include "error.h"

volatile uint32_t error_state = 0;

/****
    * @函数名     Get_Error_State 
    * @描述       获取错误状态     
    * @传入参数   无
    * @传出参数   无
    * @返回值     错误状态;
    */
uint32_t Get_Error_State(void) {
	return error_state;
}

void Set_Error_State(uint32_t error_bitmask) {
	error_state |= error_bitmask;
}

void Clear_Error_State(uint32_t error_bitmask) {
	error_state &= ~error_bitmask;
}
