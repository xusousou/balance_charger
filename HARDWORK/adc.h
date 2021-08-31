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

uint8_t Get_Adc_Val(uint16_t *bat,uint16_t *s, uint16_t *ss);

uint8_t get_low_filter(uint16_t *vol1, uint16_t *vol2, uint16_t  *BAT );

uint32_t Get_Cell_Voltage(uint8_t cell_number);    

float Get_MCU_Temperature(void) ;

#endif

