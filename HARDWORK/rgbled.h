#ifndef __RGBLED_H
#define __RGBLED_H
#include "board.h"

#define  WS2812B
//#define  Common_positive_RGB

#define  red     0x00ff00
#define  green   0xff0000
#define  blue    0x0000ff
#define  none    0x000000

void rgbInit(void);
void Charge_RGB_Control(float vol,uint8_t CELL);
void LED_Control(uint32_t color);


#endif
