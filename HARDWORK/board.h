#ifndef __BOARD_H__
#define __BOARD_H__

#include "systick.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "error.h"

#include "boardConfig.h"
#include "rgbled.h"
#include "dma.h"
#include "adc.h"
#include "usart.h"
#include "control.h"
#include "iic.h"
#include "drv_i2c.h"
#include "bq25703a_regulator.h"
#include "battery.h"

void boardInit(void);




#endif


