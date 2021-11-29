#include "control.h"

extern volatile struct Battery battery_state;

void balance_Init()
{

    rcu_periph_clock_enable(CELL_1S_DIS_EN_CLK);
    rcu_periph_clock_enable(CELL_2S_DIS_EN_CLK);
    rcu_periph_clock_enable(CELL_3S_DIS_EN_CLK);
    rcu_periph_clock_enable(CELL_4S_DIS_EN_CLK);

    gpio_mode_set(CELL_1S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_1S_DIS_EN_PIN);
    gpio_mode_set(CELL_2S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_2S_DIS_EN_PIN);
    gpio_mode_set(CELL_3S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_3S_DIS_EN_PIN);
    gpio_mode_set(CELL_4S_DIS_EN_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,CELL_4S_DIS_EN_PIN);

    gpio_output_options_set(CELL_1S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_1S_DIS_EN_PIN);
    gpio_output_options_set(CELL_2S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_2S_DIS_EN_PIN);
    gpio_output_options_set(CELL_3S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_3S_DIS_EN_PIN);
    gpio_output_options_set(CELL_4S_DIS_EN_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,CELL_4S_DIS_EN_PIN);

    gpio_bit_reset(CELL_1S_DIS_EN_PORT,CELL_1S_DIS_EN_PIN); 
    gpio_bit_reset(CELL_2S_DIS_EN_PORT,CELL_2S_DIS_EN_PIN); 
    gpio_bit_reset(CELL_3S_DIS_EN_PORT,CELL_3S_DIS_EN_PIN); 
    gpio_bit_reset(CELL_4S_DIS_EN_PORT,CELL_4S_DIS_EN_PIN); 

}
void Balancing_GPIO_Control(uint8_t cell_balancing_gpio_bitmask)
{
	if ( cell_balancing_gpio_bitmask & (1<<3) ) {
        gpio_bit_set(CELL_4S_DIS_EN_PORT,CELL_4S_DIS_EN_PIN); 
	}
	else {
        gpio_bit_reset(CELL_4S_DIS_EN_PORT,CELL_4S_DIS_EN_PIN); 
	}

	if ( cell_balancing_gpio_bitmask & (1<<2) ) {
        gpio_bit_set(CELL_3S_DIS_EN_PORT,CELL_3S_DIS_EN_PIN); 
	}
	else {
        gpio_bit_reset(CELL_3S_DIS_EN_PORT,CELL_3S_DIS_EN_PIN); 
	}

	if ( cell_balancing_gpio_bitmask & (1<<1) ) {
        gpio_bit_set(CELL_2S_DIS_EN_PORT,CELL_2S_DIS_EN_PIN); 
	}
	else {
        gpio_bit_reset(CELL_2S_DIS_EN_PORT,CELL_2S_DIS_EN_PIN); 
	}

	if ( cell_balancing_gpio_bitmask & (1<<0) ) {
        gpio_bit_set(CELL_1S_DIS_EN_PORT,CELL_1S_DIS_EN_PIN); 
	}
	else {
        gpio_bit_reset(CELL_1S_DIS_EN_PORT,CELL_1S_DIS_EN_PIN); 
	}
}
float scalar = 0.0f;
void Balance_Battery()
{
	if ( (battery_state.balance_port_connected == CONNECTED) && (Get_Error_State() == 0) ) {

		uint32_t min_cell_voltage = Get_Cell_Voltage(1);
        uint32_t max_cell_voltage = Get_Cell_Voltage(1);
		for(int i = 2; i <= battery_state.number_of_cells; i++) {
			if (Get_Cell_Voltage(i)  < min_cell_voltage) {
				min_cell_voltage = Get_Cell_Voltage(i) ;
			}
			if (Get_Cell_Voltage(i)  > max_cell_voltage) {
				max_cell_voltage = Get_Cell_Voltage(i) ;
			}
		}

		if (battery_state.xt_connected == CONNECTED) {
			scalar = (float)CELL_BALANCING_SCALAR_MAX * (1.0f - (((float)max_cell_voltage - (float)MIN_CELL_V_FOR_BALANCING)/((float)CELL_VOLTAGE_TO_ENABLE_CHARGING - (float)MIN_CELL_V_FOR_BALANCING)));
			if (scalar < 1.0f) {
				scalar = 1.0f;
			}
		}else {
			scalar = 1.0f;
		}

        if ( ((max_cell_voltage - min_cell_voltage) >= ((float)CELL_DELTA_V_ENABLE_BALANCING * scalar)) && (min_cell_voltage > MIN_CELL_V_FOR_BALANCING) && (battery_state.balancing_enabled == 0)) {
            battery_state.balancing_enabled = 1;
        }else if ( (((max_cell_voltage - min_cell_voltage) < ((float)CELL_BALANCING_HYSTERESIS_V * scalar)) && (battery_state.balancing_enabled == 1)) || (min_cell_voltage < MIN_CELL_V_FOR_BALANCING) ) {
            battery_state.balancing_enabled = 0;
        }

		for(int i = 1; i <= battery_state.number_of_cells; i++) {
			if (Get_Cell_Voltage(i)  >= CELL_OVER_VOLTAGE_ENABLE_DISCHARGE) {
				battery_state.cell_balance_bitmask |= (1<<(i-1));
			}
			else if ( (battery_state.balancing_enabled == 1) && ((Get_Cell_Voltage(i)  - min_cell_voltage) >= ((float)CELL_BALANCING_HYSTERESIS_V * scalar))) {
				battery_state.cell_balance_bitmask |= (1<<(i-1));
			}
			else {
				battery_state.cell_balance_bitmask &= ~(1<<(i-1));
			}
            Balancing_GPIO_Control(battery_state.cell_balance_bitmask);
        }
    }
	else {
		Balancing_GPIO_Control(0);
		battery_state.balancing_enabled = 0;
	}
}

