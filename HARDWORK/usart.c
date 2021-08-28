#include "usart.h"

#define USART_RECV_BUF_SIZE 32// 串口接收缓存长度
#define USART_SEND_BUF_SIZE 32// 串口发送缓存长度

static uint8_t USART_Rx_Buf[USART_RECV_BUF_SIZE];// 用于保存 RX 接收到的数据
static uint16_t USART_Rx_Len = 0;                // 用于保存 RX 接收到的数据的个数
static uint16_t USART_Rx_Count = 0;              // 用于保存 RX 接收数据时已经接收到的数据个数
static uint8_t USART_Tx_Buf[USART_SEND_BUF_SIZE];// 用于保存 Tx 要发送的数据
static uint16_t USART_Tx_Len = 0;                // 用于保存 Tx 要发送的数据的个数
static uint16_t USART_Tx_Count = 0;              // 用于保存 Tx 要发送的数据时已经发送的数据个数

/**
 * @brief 初始化串口
 */
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

/**
 * @brief 使用串口发送数据
 */
//void USART_Send()
//{
////	usart_interrupt_enable(USART0, USART_INT_TBE);
//}

///**
// * @brief 串口 0 中断服务函数
// */
//void USART0_IRQHandler(void)
//{
//	// 如果满足以下条件，说明检测到“读数据缓冲区非空中断标志”被置位
//	if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE))
//	{ 
//		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_RBNE);// 清除标志位
//		
//		uint8_t ch = (uint8_t)usart_data_receive(USART0);// 从 RX 数据寄存器中读取数据
//		
//		// 如果满足以下条件，说明缓存区还有空间去存储数据
//		if(USART_Rx_Count < USART_RECV_BUF_SIZE)
//		{
//			USART_Rx_Buf[USART_Rx_Count++] = ch;
//		}
//	}
//	// 如果满足以下条件，说明检测到“IDLE线检测中断标志”被置位
//	if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_IDLE))
//	{
//		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_IDLE);
//		
//		// 如果满足以下条件，说明已经接收到了一帧数据
//		if(USART_Rx_Count > 0)
//		{		
//			usart_receive_config(USART0, USART_RECEIVE_DISABLE);// 关闭串口接收功能，准备处理数据

//			USART_Rx_Len   = USART_Rx_Count;
//			USART_Rx_Count = 0;	

//			SEGGER_RTT_printf(0,"Receive %d bytes!\n", USART_Rx_Len);	
//		}
//	}
//	// 如果满足以下条件，说明检测到“发送缓冲区空中断标志”被置位
//	if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_TBE))
//	{
//		usart_interrupt_flag_clear(USART0, USART_INT_FLAG_TBE);

//		// 如果满足以下条件，说明缓存中有数据需要发送
//		if(USART_Tx_Len > 0)
//		{
//			// 如果满足以下条件，说明缓存中还有数据没发送完
//			if(USART_Tx_Count < USART_Tx_Len)
//			{
//				usart_data_transmit(USART0, USART_Tx_Buf[USART_Tx_Count++]);
//			}
//			// 反之，说明缓存中的数据都已经发送完毕
//			else
//			{
//				usart_interrupt_disable(USART0, USART_INT_TBE);

//				USART_Tx_Len = 0;
//				USART_Tx_Count = 0;
//			}
//		}
//		// 反之，说明缓存中没有数据需要发送
//		else
//		{
//			usart_interrupt_disable(USART0, USART_INT_TBE);
//		}
//	}
//}

