#include "key.h"

char KEY1,KEY1_Flag = 0;

void key_init(void)
{
    rcu_periph_clock_enable(WAKEUP_KEY_GPIO_CLK);
    gpio_mode_set(WAKEUP_KEY_GPIO_PORT,GPIO_MODE_INPUT,GPIO_PUPD_PULLUP,WAKEUP_KEY_PIN);
}

uint8_t KEY_Scan(void)
{
    KEY1 = gpio_input_bit_get(WAKEUP_KEY_GPIO_PORT,WAKEUP_KEY_PIN);
    static uint8_t flag_key=1;

    if(flag_key && KEY1==0)
    {
        flag_key=0;
        KEY1_Flag = ~KEY1_Flag;
        return 1;	
    }
    else if(1==KEY1)			
    {
        flag_key=1;
    }
return 0;
}
