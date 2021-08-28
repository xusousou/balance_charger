#include "control.h"



void balance_Init()
{

    rcu_periph_clock_enable(CELL_1S_DIS_EN_CLK);
    rcu_periph_clock_enable(CELL_2S_DIS_EN_CLK);
    rcu_periph_clock_enable(CELL_3S_DIS_EN_CLK);
    rcu_periph_clock_enable(CELL_4S_DIS_EN_CLK);

    gpio_deinit(CELL_1S_DIS_EN_PORT);
    gpio_deinit(CELL_2S_DIS_EN_PORT);
    gpio_deinit(CELL_3S_DIS_EN_PORT);  
    gpio_deinit(CELL_4S_DIS_EN_PORT);

    gpio_mode_set(CELL_1S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_1S_DIS_EN_PIN);
    gpio_mode_set(CELL_2S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_2S_DIS_EN_PIN);
    gpio_mode_set(CELL_3S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_3S_DIS_EN_PIN);
    gpio_mode_set(CELL_4S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_4S_DIS_EN_PIN);

    gpio_output_options_set(CELL_1S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_1S_DIS_EN_PIN);
    gpio_output_options_set(CELL_2S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_2S_DIS_EN_PIN);
    gpio_output_options_set(CELL_3S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_3S_DIS_EN_PIN);
    gpio_output_options_set(CELL_4S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_4S_DIS_EN_PIN);

    gpio_bit_reset(CELL_1S_DIS_EN_PORT,CELL_1S_DIS_EN_PIN); 
    gpio_bit_reset(CELL_2S_DIS_EN_PORT,CELL_2S_DIS_EN_PIN); 
    gpio_bit_reset(CELL_3S_DIS_EN_PORT,CELL_3S_DIS_EN_PIN); 
    gpio_bit_reset(CELL_4S_DIS_EN_PORT,CELL_4S_DIS_EN_PIN); 
}


