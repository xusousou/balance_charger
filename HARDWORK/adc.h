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

uint8_t Get_Adc_Val(uint32_t *bat,uint32_t *s, uint32_t *ss);

uint8_t get_low_filter(uint32_t *vol1, uint32_t *vol2, uint32_t  *BAT );

uint32_t Get_Cell_Voltage(uint8_t cell_number);

float Get_MCU_Temperature(void) ;

#endif

