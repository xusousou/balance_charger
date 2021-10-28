#include "board.h"

void boardInit()
{
//    SW_Init();
    key_init();
    i2c_init();
    balance_Init();
    rgbInit();

    delay_1ms(10);
    LED_Control(red);
    USART_Init();

 
    delay_1ms(1000);
    LED_Control(blue);
    BQ25703A_init();

    delay_1ms(1000);
    LED_Control(green);
    adc_init();

    delay_1ms(500);
    LED_Control(none);
    delay_1ms(1000);
}


