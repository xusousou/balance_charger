/**
  ******************************************************************************
  *	文件: board.c
  * 描述: 初始化外设
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#include "board.h"

void boardInit()
{
    rgbInit();
    key_init();
    i2c_init();
    HUSB238_i2c_init();
    balance_Init();
    LED_Control(red);
    USART_Init();

    delay_1ms(1000);
    LED_Control(green);
    BQ25703A_init();

    delay_1ms(1000);
    LED_Control(blue);
    adc_init();

    delay_1ms(1000);
    LED_Control(none);

}


