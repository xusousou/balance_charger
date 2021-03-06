/**
  ******************************************************************************
  *	文件: battery.h
  * 描述: 
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#ifndef __BATTERY_H_
#define __BATTERY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "board.h"

struct Battery {
	uint8_t xt_connected;
	uint8_t balance_port_connected;
	uint8_t number_of_cells;
	uint8_t balancing_enabled;
	uint8_t requires_charging;
	uint8_t cell_over_voltage;
	uint8_t cell_balance_bitmask;
};

#define BATTERY_ADC_MULTIPLIER 		10000000
#define VOLTAGE_CONNECTED_THRESHOLD			(uint32_t)( 1.0 * BATTERY_ADC_MULTIPLIER )
#define CELL_DELTA_V_ENABLE_BALANCING		(uint32_t)( 0.015 * BATTERY_ADC_MULTIPLIER )
#define CELL_BALANCING_HYSTERESIS_V			(uint32_t)( 0.010 * BATTERY_ADC_MULTIPLIER )
#define CELL_BALANCING_SCALAR_MAX			(uint8_t)40
#define MIN_CELL_V_FOR_BALANCING			(uint32_t)( 3.0 * BATTERY_ADC_MULTIPLIER )
#define CELL_VOLTAGE_TO_ENABLE_CHARGING		(uint32_t)( 4.185 * BATTERY_ADC_MULTIPLIER )
#define CELL_OVER_VOLTAGE_ENABLE_DISCHARGE	(uint32_t)( 4.205 * BATTERY_ADC_MULTIPLIER )
#define CELL_OVER_VOLTAGE_DISABLE_CHARGING	(uint32_t)( 4.22 * BATTERY_ADC_MULTIPLIER )
#define MIN_CELL_VOLTAGE_SAFE_LIMIT			(uint32_t)( 2.0 * BATTERY_ADC_MULTIPLIER )
#define COMPENSATION_VOLTAGE                (uint32_t)( 0.005 * BATTERY_ADC_MULTIPLIER )

#define THREE_S_BITMASK 		0b0111
#define TWO_S_BITMASK			0b0011
#define ONE_S_BITMASK			0b0001

void SW_Init(void);
uint8_t cell_Select(uint8_t num);
void Balance_Connection_State(void);
void Battery_Connection_State(void);
void Read_Cell_Voltage(void);
void Cell_Voltage_Safety_Check(void);
void Balance_Battery(void);
void full_charger_Check(float vol, uint8_t CELL);


uint8_t Get_Balancing_State();
uint8_t Get_Requires_Charging_State();
uint8_t Get_Number_Of_Cells();
uint8_t Get_XT_Connection_State();
uint8_t Get_Balance_Connection_State();

#endif
