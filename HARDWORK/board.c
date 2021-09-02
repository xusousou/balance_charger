#include "board.h"

void boardInit()
{
    SW_Init();
    i2c_init();
    balance_Init();

    rgbInit();
    delay_1ms(100);
    LED_Control(red);


    USART_Init();
    adc_init();
    delay_1ms(1000);
    LED_Control(green);

    BQ25703A_init();

    delay_1ms(1000);
    LED_Control(blue);

    delay_1ms(1000);
    LED_Control(none);
    delay_1ms(1000);
}


