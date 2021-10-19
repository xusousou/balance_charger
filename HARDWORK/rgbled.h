#ifndef __RGBLED_H
#define __RGBLED_H
#include "board.h"

#define  WS2812B
//#define  Common_positive_RGB

#define  white   0xffffff
#define  red     0x00ff00
#define  green   0xff0000
#define  blue    0x0000ff
#define  none    0x000000
#define  orange  0xE9967A
void rgbInit(void);
//void Charge_RGB_Control(float vol,uint8_t CELL);
void LED_Control(uint32_t color);
void chargerToColor(unsigned long color0, unsigned long color1,float bat, uint8_t cell);
unsigned long  ColorToColor(uint8_t i);
void Color_decomposition(unsigned long color0, unsigned long color1);
unsigned char Triangular(int num);//三角波

#endif
