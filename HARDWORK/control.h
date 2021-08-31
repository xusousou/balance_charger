#ifndef __CONTROL_H__
#define __CONTROL_H__
#include "board.h"

#define MAX_CELL_VOLTAGE_SAFE_LIMIT	    4.2090
#define MIN_CELL_VOLTAGE_SAFE_LIMIT2	    1.0000

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

void balance_Init(void);

void Balance_Battery();



#endif
