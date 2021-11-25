/**
  ******************************************************************************
  *	文件: board.h
  * 描述: 
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#ifndef __BOARD_H__
#define __BOARD_H__

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "cmsis_os.h"

#include "systick.h"
#include "boardConfig.h"
#include "rgbled.h"
#include "adc.h"
#include "usart.h"
#include "control.h"
#include "iic.h"
#include "bq25703a_regulator.h"
#include "battery.h"
#include "key.h"
#include "error.h"
#include "husb238_iic.h"
#include "husb238_regulator.h"


void boardInit(void);

#endif
