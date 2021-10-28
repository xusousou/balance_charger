#ifndef __RGBLED_H
#define __RGBLED_H
#include "board.h"

#define  WS2812B
//#define  Common_positive_RGB

#define  white   0xFFFFFF

#define  red     0x00FF00
#define  orange  0x7FFF00
#define  yellow  0xFFFF00
#define  green   0xFF0000
#define  blue    0x0000FF
#define  indigo  0xFF00FF
#define  violet  0x008BFF

#define  none    0x000000

void rgbInit(void);
//void Charge_RGB_Control(float vol,uint8_t CELL);
void LED_Control(uint32_t color);
void chargerToColor(unsigned long color0, unsigned long color1,float bat, uint8_t cell);
unsigned long  ColorToColor(uint8_t i);
void Color_decomposition(unsigned long color0, unsigned long color1);
unsigned char Triangular(int num);//三角波

void Colorful_gradient(void);

#endif
