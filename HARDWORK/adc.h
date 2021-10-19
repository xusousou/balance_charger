#ifndef __ADC_H_
#define __ADC_H_
#include "board.h"


struct Adc {
	float cell_voltage[5];
	float vrefint;
	float temperature;
};

void adc_init(void);

#ifdef adc_1
uint8_t Get_Adc_Val(uint32_t *bat,uint32_t *s, uint32_t *ss);
uint8_t get_low_filter(uint32_t *vol1, uint32_t *vol2, uint32_t  *BAT );
#else
void Get_Adc_Val( uint32_t *bat, uint32_t *s, uint32_t *ss, uint32_t *sss, uint32_t *ssss);
void get_low_filter(uint32_t *bat, uint32_t *s, uint32_t *ss, uint32_t *sss, uint32_t *ssss);
#endif

uint32_t Get_Cell_Voltage(uint8_t cell_number);
float Get_MCU_Temperature(void) ;
#endif

