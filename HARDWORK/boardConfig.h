/**
  ******************************************************************************
  *	文件名:   boardConfig.h
  * 描述：    
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#ifndef __BOARDCONFIG_H
#define __BOARDCONFIG_H

#include "systick.h"
#include "gd32e23x.h"

//#define balance_board_1
//#define adc_1
#define balance_board_2
#define adc_2

#ifdef  balance_board_2

/**********************************************************************************************************
*RGB_LED:   WS281B
**********************************************************************************************************/
#define RGB_PIN          GPIO_PIN_5
#define RGB_PORT         GPIOB
#define RGB_CLK          RCU_GPIOB

/**********************************************************************************************************
*平衡控制
**********************************************************************************************************/
#define CELL_1S_DIS_EN_PIN          GPIO_PIN_7
#define CELL_1S_DIS_EN_PORT         GPIOA
#define CELL_1S_DIS_EN_CLK          RCU_GPIOA

#define CELL_2S_DIS_EN_PIN          GPIO_PIN_0
#define CELL_2S_DIS_EN_PORT         GPIOB
#define CELL_2S_DIS_EN_CLK          RCU_GPIOB

#define CELL_3S_DIS_EN_PIN          GPIO_PIN_1
#define CELL_3S_DIS_EN_PORT         GPIOB
#define CELL_3S_DIS_EN_CLK          RCU_GPIOB

#define CELL_4S_DIS_EN_PIN          GPIO_PIN_2
#define CELL_4S_DIS_EN_PORT         GPIOB
#define CELL_4S_DIS_EN_CLK          RCU_GPIOB

/**********************************************************************************************************
*模拟选通开关
**********************************************************************************************************/
#define SWA_PIN          GPIO_PIN_11
#define SWA_PORT         GPIOA
#define SWA_CLK          RCU_GPIOA

#define SWB_PIN          GPIO_PIN_12
#define SWB_PORT         GPIOA
#define SWB_CLK          RCU_GPIOA

/**********************************************************************************************************
*ADC
**********************************************************************************************************/
#ifdef  adc_1

#define CELL_1S_VOL_PIN          GPIO_PIN_0
#define CELL_1S_VOL_PORT         GPIOA
#define CELL_1S_VOL_CLK          RCU_GPIOA

#define CELL_2S_VOL_PIN          GPIO_PIN_1
#define CELL_2S_VOL_PORT         GPIOA
#define CELL_2S_VOL_CLK          RCU_GPIOA

#define CELL_BAT_VOL_PIN         GPIO_PIN_2
#define CELL_BAT_VOL_PORT        GPIOA
#define CELL_BAT_VOL_CLK         RCU_GPIOA

#else
#define CELL_BAT_VOL_PIN         GPIO_PIN_2
#define CELL_BAT_VOL_PORT        GPIOA
#define CELL_BAT_VOL_CLK         RCU_GPIOA

#define CELL_1S_VOL_PIN          GPIO_PIN_3
#define CELL_1S_VOL_PORT         GPIOA
#define CELL_1S_VOL_CLK          RCU_GPIOA

#define CELL_2S_VOL_PIN          GPIO_PIN_4
#define CELL_2S_VOL_PORT         GPIOA
#define CELL_2S_VOL_CLK          RCU_GPIOA

#define CELL_3S_VOL_PIN         GPIO_PIN_5
#define CELL_3S_VOL_PORT        GPIOA
#define CELL_3S_VOL_CLK         RCU_GPIOA

#define CELL_4S_VOL_PIN         GPIO_PIN_6
#define CELL_4S_VOL_PORT        GPIOA
#define CELL_4S_VOL_CLK         RCU_GPIOA

#endif

/**********************************************************************************************************
*充电芯片bq25703
**********************************************************************************************************/
#define IIC_SCL_PIN                         GPIO_PIN_6
#define IIC_SCL_GPIO_PORT                   GPIOB
#define IIC_SCL_GPIO_CLK                    RCU_GPIOB

#define IIC_SDA_PIN                         GPIO_PIN_7
#define IIC_SDA_GPIO_PORT                   GPIOB
#define IIC_SDA_GPIO_CLK                    RCU_GPIOB

#define EN_OTG_PIN          GPIO_PIN_3
#define EN_OTG_PORT         GPIOB
#define EN_OTG_CLK          RCU_GPIOB

#define PROCHOT_PIN         GPIO_PIN_8
#define PROCHOT_PORT        GPIOB
#define PROCHOT_CLK         RCU_GPIOB

#define ILIM_HIZ_PIN        GPIO_PIN_4
#define ILIM_HIZ_PORT       GPIOB
#define ILIM_HIZ_CLK        RCU_GPIOB

#define CHRG_OK_PIN         GPIO_PIN_15
#define CHRG_OK_PORT        GPIOA
#define CHRG_OK_CLK         RCU_GPIOA

/**********************************************************************************************************
*USART
**********************************************************************************************************/
#define RX1_PIN          GPIO_PIN_10
#define RX1_PORT         GPIOA

#define TX1_PIN          GPIO_PIN_9
#define TX1_PORT         GPIOA


/**********************************************************************************************************
*按键
**********************************************************************************************************/
#define WAKEUP_KEY_PIN                   GPIO_PIN_8
#define WAKEUP_KEY_GPIO_PORT             GPIOA
#define WAKEUP_KEY_GPIO_CLK              RCU_GPIOA

#elif balance_board_1

/**********************************************************************************************************
*RGB_LED
**********************************************************************************************************/

#define R_PIN          GPIO_PIN_3
#define R_PORT         GPIOB
#define R_CLK          RCU_GPIOB

#define G_PIN          GPIO_PIN_15
#define G_PORT         GPIOA
#define G_CLK          RCU_GPIOA

#define B_PIN          GPIO_PIN_4
#define B_PORT         GPIOB
#define B_CLK          RCU_GPIOB

//WS2812B
#define RGB_PIN          GPIO_PIN_5
#define RGB_PORT         GPIOB
#define RGB_CLK          RCU_GPIOB


/**********************************************************************************************************
*平衡控制
**********************************************************************************************************/

#define CELL_1S_DIS_EN_PIN          GPIO_PIN_7
#define CELL_1S_DIS_EN_PORT         GPIOA
#define CELL_1S_DIS_EN_CLK          RCU_GPIOA

#define CELL_2S_DIS_EN_PIN          GPIO_PIN_0
#define CELL_2S_DIS_EN_PORT         GPIOB
#define CELL_2S_DIS_EN_CLK          RCU_GPIOB

#define CELL_3S_DIS_EN_PIN          GPIO_PIN_1
#define CELL_3S_DIS_EN_PORT         GPIOB
#define CELL_3S_DIS_EN_CLK          RCU_GPIOB

#define CELL_4S_DIS_EN_PIN          GPIO_PIN_2
#define CELL_4S_DIS_EN_PORT         GPIOB
#define CELL_4S_DIS_EN_CLK          RCU_GPIOB


/**********************************************************************************************************
*模拟选通开关
**********************************************************************************************************/

#define SWA_PIN          GPIO_PIN_11
#define SWA_PORT         GPIOA
#define SWA_CLK          RCU_GPIOA

#define SWB_PIN          GPIO_PIN_12
#define SWB_PORT         GPIOA
#define SWB_CLK          RCU_GPIOA

/**********************************************************************************************************
*ADC
**********************************************************************************************************/

#define CELL_1S_VOL_PIN          GPIO_PIN_0
#define CELL_1S_VOL_PORT         GPIOA
//#define CELL_1S_VOL_CLK          RCU_GPIOA

#define CELL_2S_VOL_PIN          GPIO_PIN_1
#define CELL_2S_VOL_PORT         GPIOA
//#define CELL_2S_VOL_CLK          RCU_GPIOA

#define CELL_BAT_VOL_PIN         GPIO_PIN_2
#define CELL_BAT_VOL_PORT        GPIOA
//#define CELL_BAT_VOL_CLK         RCU_GPIOA


/**********************************************************************************************************
*充电芯片
**********************************************************************************************************/
#define IIC_SCL_PIN                         GPIO_PIN_6
#define IIC_SCL_GPIO_PORT                   GPIOB
#define IIC_SCL_GPIO_CLK                    RCU_GPIOB

#define IIC_SDA_PIN                         GPIO_PIN_7
#define IIC_SDA_GPIO_PORT                   GPIOB
#define IIC_SDA_GPIO_CLK                    RCU_GPIOB

#define EN_OTG_PIN          GPIO_PIN_3
#define EN_OTG_PORT         GPIOA
#define EN_OTG_CLK          RCU_GPIOA

#define PROCHOT_PIN         GPIO_PIN_4
#define PROCHOT_PORT        GPIOA
#define PROCHOT_CLK         RCU_GPIOA

#define ILIM_HIZ_PIN        GPIO_PIN_5
#define ILIM_HIZ_PORT       GPIOA
#define ILIM_HIZ_CLK        RCU_GPIOA

#define CHRG_OK_PIN         GPIO_PIN_6
#define CHRG_OK_PORT        GPIOA
#define CHRG_OK_CLK         RCU_GPIOA

/**********************************************************************************************************
*USART
**********************************************************************************************************/

#define RX1_PIN          GPIO_PIN_10
#define RX1_PORT         GPIOA

#define TX1_PIN          GPIO_PIN_9
#define TX1_PORT         GPIOA


/**********************************************************************************************************
*
**********************************************************************************************************/

#define KEY1_PIN          GPIO_PIN_8
#define KEY1_PORT         GPIOB

#endif

#endif

