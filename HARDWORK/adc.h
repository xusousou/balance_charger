#ifndef __ADC_H_
#define __ADC_H_
#include "board.h"


struct Adc {
	float bat_voltage;
	float cell_voltage[5];
	float vrefint;
	float vdda;
	float temperature;
	float two_s_battery_voltage;
	float three_s_battery_voltage;
	float four_s_battery_voltage;
};

void adc_init(void);

#ifdef adc_1
uint8_t Get_Adc_Val(uint32_t *bat,uint32_t *s, uint32_t *ss);
uint8_t get_low_filter(uint32_t *vol1, uint32_t *vol2, uint32_t  *BAT );
#else
uint8_t Get_Adc_Val( uint32_t *bat, uint32_t *s, uint32_t *ss, uint32_t *sss, uint32_t *ssss);
uint8_t get_low_filter( uint32_t  *BAT, uint32_t *vol1, uint32_t *vol2, uint32_t *vol3, uint32_t *vol4);
#endif

uint32_t Get_Cell_Voltage(uint8_t cell_number);
float Get_MCU_Temperature(void) ;
#endif

