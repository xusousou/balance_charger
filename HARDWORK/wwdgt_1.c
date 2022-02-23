/**
  ******************************************************************************
  *	文件: wwdgt_1.c
  * 描述: 看门狗初始化
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#include "wwdgt_1.h"

void wwdgt_Init()
{
    wwdgt_deinit();
   /* check if the system has resumed from WWDGT reset */
    if(RESET != wwdgt_flag_get()){
        /* WWDGTRST flag set */
        printf("1");
        wwdgt_flag_clear();
        while(1);
    }else{
        delay_1ms(1000);
        printf("wwdgt1");
    }

    /* enable WWDGT clock */
    rcu_periph_clock_enable(RCU_WWDGT);
//    rcu_periph_reset_enable(RCU_WWDGTRST);
    /*
     *  set WWDGT clock = (PCLK1 (72MHz)/4096)/8 = 2197Hz (~455 us)
     *  set counter value to 127
     *  set window value to 80
     *  refresh window is: ~455 * (127-80)= 21.3ms < refresh window < ~455 * (127-63) =29.1ms.
     */
    wwdgt_config(127,80,WWDGT_CFG_PSC_DIV8);
    wwdgt_interrupt_enable();
    wwdgt_enable();
}
