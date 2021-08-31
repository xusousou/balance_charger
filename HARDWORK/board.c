#include "board.h"

void boardInit()
{
    SW_Init();
    i2c_init();
    balance_Init();

    rgbInit();
    delay_1ms(100);
    LED_Control(0x00ff00);


    USART_Init();
    adc_init();
    delay_1ms(1000);
    LED_Control(0xff0000);

    BQ25703A_init();

    delay_1ms(1000);
    LED_Control(0x0000ff);

    delay_1ms(1000);
    LED_Control(0x000000);
    delay_1ms(1000);
}


