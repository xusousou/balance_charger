/**
  ******************************************************************************
  * 文件: husb238_iic.h
  * 描述: 
  ******************************************************************************
  * @attention
  ******************************************************************************
**/
#ifndef __HUSB238_IIC_H
#define __HUSB238_IIC_H

#include "board.h"

#define HUSB238_IIC_SCL(n)   (n?gpio_bit_set(GPIOA,GPIO_PIN_11):gpio_bit_reset(GPIOA,GPIO_PIN_11))//n=1,输出高电平；n=0，输出低电平
#define HUSB238_IIC_SDA(n)   (n?gpio_bit_set(GPIOA,GPIO_PIN_12):gpio_bit_reset(GPIOA,GPIO_PIN_12))
#define HUSB238_READ_SDA     gpio_input_bit_get(GPIOA,GPIO_PIN_12)

void HUSB238_i2c_start(void);
void HUSB238_i2c_stop(void);
uint8_t HUSB238_i2c_wait_ack(void);
void HUSB238_i2c_ack(void);
void HUSB238_i2c_nack(void);
void HUSB238_i2c_send_byte(uint8_t byte);
uint8_t HUSB238_i2c_read_byte();

void HUSB238_i2c_init(void);

#endif
