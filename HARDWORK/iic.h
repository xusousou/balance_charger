/**
  ******************************************************************************
  * 文件: iic.h
  * 描述: BQ25703A->iic
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#ifndef __IIC_H
#define __IIC_H
#include "board.h"

#define IIC_SCL(n)   (n?gpio_bit_set(GPIOB,GPIO_PIN_6):gpio_bit_reset(GPIOB,GPIO_PIN_6))//n=1,输出高电平；n=0，输出低电平
#define IIC_SDA(n)   (n?gpio_bit_set(GPIOB,GPIO_PIN_7):gpio_bit_reset(GPIOB,GPIO_PIN_7))
#define READ_SDA     gpio_input_bit_get(GPIOB,GPIO_PIN_7)


void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_wait_ack(void);
void i2c_ack(void);
void i2c_nack(void);
void i2c_send_byte(uint8_t byte);
uint8_t i2c_read_byte();

void i2c_init(void);


//IIC所有操作函数


#endif
