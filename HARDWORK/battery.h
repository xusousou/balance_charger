#ifndef BATTERY_H_
#define BATTERY_H_

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



/**********************************************************************************************************/
#define MAX_CELL_VOLTAGE_SAFE_LIMIT	    4.2090
#define MIN_CELL_VOLTAGE_SAFE_LIMIT	    1.5000

#define MAX_CELL_FULL_VOLTAGE_CHARGER	    4.2090
#define MIN_CELL_FULL_VOLTAGE_CHARGER	    4.1790


#define CELL_VOLTAGE_PRECISION      0.005

#define MAX_1SBAT_VOLTAGE	    4.2090
#define MIN_1SBAT_VOLTAGE	    1.0000

#define MAX_2SBAT_VOLTAGE	    8.4090
#define MIN_2SBAT_VOLTAGE	    4.2000

#define MAX_3SBAT_VOLTAGE	    12.6090
#define MIN_3SBAT_VOLTAGE	    6.3000

#define MAX_4SBAT_VOLTAGE	    16.8090
#define MIN_4SBAT_VOLTAGE	    8.4000

void SW_Init(void);
uint8_t cell_Select(uint8_t num);
void Balance_Connection_State(void);
uint8_t Battery_Connection_State(void);
void Read_Cell_Voltage(void);

/**********************************************************************************************************/

uint8_t Get_Number_Of_Cells();
uint8_t Get_XT_Connection_State();
uint8_t Get_Balance_Connection_State();
#endif
