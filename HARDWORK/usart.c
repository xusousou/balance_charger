/**
  ******************************************************************************
  *	文件: usart.c
  * 描述: 串口
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#include "usart.h"

void USART_Init()
{
	rcu_periph_clock_enable(RCU_GPIOA); // 使能外设时钟
	rcu_periph_clock_enable(RCU_USART0);
	
	gpio_af_set            (GPIOA, GPIO_AF_1,     GPIO_PIN_9); // 设置GPIO的备用功能
	gpio_af_set            (GPIOA, GPIO_AF_1,     GPIO_PIN_10);
	gpio_mode_set          (GPIOA, GPIO_MODE_AF,  GPIO_PUPD_PULLUP,  GPIO_PIN_9); // 设置GPIO模式
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_9); // 设置GPIO输出模式和速度
	gpio_mode_set          (GPIOA, GPIO_MODE_AF,  GPIO_PUPD_PULLUP,  GPIO_PIN_10);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_10);
	
	usart_deinit                   (USART0);                        // 复位外设USARTx
	usart_word_length_set          (USART0, USART_WL_8BIT);         // 配置USART字长
	usart_stop_bit_set             (USART0, USART_STB_1BIT);        // 配置USART停止位
	usart_parity_config            (USART0, USART_PM_NONE);         // 配置USART奇偶校验
	usart_baudrate_set             (USART0, 115200U);               // 配置USART波特率
	usart_receive_config           (USART0, USART_RECEIVE_ENABLE);  // USART接收器配置
	usart_transmit_config          (USART0, USART_TRANSMIT_ENABLE); // USART发送器配置
	usart_enable                   (USART0);                        // 使能USART
	
}

