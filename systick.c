#include "systick.h"
#include "gd32e23x.h"
#include "FreeRTOS.h"
#include "task.h"
/*!
    \file  systick.c
    \brief the systick configuration file
*/

volatile static float count_1us = 0;
volatile static float count_1ms = 0;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config(void)
{
    /* systick clock source is from HCLK/8 */
    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);
    count_1us = (float)SystemCoreClock/8000000;
    count_1ms = (float)count_1us * 1000;
}

/*!
    \brief      delay a time in microseconds in polling mode
    \param[in]  count: count in microseconds
    \param[out] none
    \retval     none
*/
void delay_1us(uint32_t count)
{
    uint32_t ctl;
    
    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1us);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do{
        ctl = SysTick->CTRL;
    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
}

/*!
    \brief      delay a time in milliseconds in polling mode
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_1ms(uint32_t count)
{
    uint32_t ctl;
    
    /* reload the count value */
    SysTick->LOAD = (uint32_t)(count * count_1ms);
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
    /* enable the systick timer */
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
    /* wait for the COUNTFLAG flag set */
    do{
        ctl = SysTick->CTRL;
    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
    /* disable the systick timer */
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
    /* clear the current count value */
    SysTick->VAL = 0x0000U;
}





/**********************************************************************************************************
*函 数 名: getSysTimeUs
*功能说明: 获取当前系统运行时间，单位为微秒
*形    参: 无
*返 回 值: 系统时间
**********************************************************************************************************/
uint64_t getSysTimeUs(void)
{
    uint64_t ms;
    uint64_t value;
    ms = xTaskGetTickCount();
    value = ms * 1000 + (SysTick->LOAD - SysTick->VAL) * 1000 / SysTick->LOAD;
    return value;
}

/**********************************************************************************************************
*函 数 名: softDelayUs
*功能说明: 微秒级软件延时
*形    参: 延时时间
*返 回 值: 无
**********************************************************************************************************/
void softDelayUs(uint32_t us)
{
    uint64_t now = getSysTimeUs();
    while ((getSysTimeUs() - now) < us);
}

void delay_us(int us)
{
    softDelayUs(us);
}
