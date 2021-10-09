#include "battery.h"

extern struct Adc adc_values;
extern float adc_data[5];
extern uint32_t temperature, vrefintnum;

volatile struct Battery battery_state;
static uint8_t cell_connected_bitmask = 0;

float valueBAT,value1S,value2S,value3S,value4S;
uint32_t vol_1s, vol_2s, vol_3s, vol_4s, vol_bat;   //uint16_t ADC_1, ADC_2, ADC_4;对应的adc通道顺序
uint8_t cell;
//uint32_t cellvol1 ,cellvol2;

#ifdef adc_1
void SW_Init()
{
    rcu_periph_clock_enable(SWA_CLK);
    rcu_periph_clock_enable(SWB_CLK);

    gpio_mode_set(SWA_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,SWA_PIN);
    gpio_mode_set(SWB_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,SWB_PIN);

    gpio_output_options_set(SWA_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,SWA_PIN);
    gpio_output_options_set(SWB_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,SWB_PIN);    
 
}

uint8_t cell_Select(uint8_t num)
{  
   switch(num)
   {
    case 2:
        gpio_bit_reset(SWA_PORT,SWA_PIN); 
        gpio_bit_reset(SWB_PORT,SWB_PIN); 
    break; 
    case 3:
        gpio_bit_reset(SWA_PORT,SWA_PIN); 
        gpio_bit_set(SWB_PORT,SWB_PIN);  
    break; 
    case 4:
        gpio_bit_set(SWA_PORT,SWA_PIN); 
        gpio_bit_set(SWB_PORT,SWB_PIN);   
    break;  
   }return 0;
}


//获取电池电压
void Read_Cell_Voltage()
{
    //2S
    cell_Select(2);   
//    vTaskDelay(200); 
    for(uint16_t i=220;i>0;i--)
    {
        /*ADC数据处理*/      
        Get_Adc_Val(&vol_bat,&vol_1s,&vol_2s);
        get_low_filter(&vol_bat,&vol_1s,&vol_2s);
        vTaskDelay(1); 
    }
    value2S = (float)(adc_data[2]/1 *3.3/4096)*2;

    //3S
    cell_Select(3);
//    vTaskDelay(200); 
    for(uint16_t i=220;i>0;i--)
    {
        /*ADC数据处理*/      
        Get_Adc_Val(&vol_bat,&vol_1s,&vol_3s);
        get_low_filter(&vol_bat,&vol_1s,&vol_3s);
        vTaskDelay(1); 
    }
    value3S = (float)(adc_data[2]/1 *3.3/4096)*2;

    //4S
    cell_Select(4);
//    vTaskDelay(200); 
    for(uint16_t i=220;i>0;i--)
    {
        /*ADC数据处理*/      
        Get_Adc_Val(&vol_bat,&vol_1s,&vol_4s);
        get_low_filter(&vol_bat,&vol_1s,&vol_4s);
        vTaskDelay(1); 
    }
  
    valueBAT = (float)(adc_data[0]/1 * 3.3/4096)*11;
    value1S = (float)(adc_data[1]/1 *3.3/4096)*2;
    value4S = (float)(adc_data[2]/1 *3.3/4096)*2;  
     
    adc_values.cell_voltage[0]=(value1S+value2S+value3S+value4S+valueBAT)/2;
    adc_values.cell_voltage[1]=value1S;
    adc_values.cell_voltage[2]=value2S;
    adc_values.cell_voltage[3]=value3S;
    adc_values.cell_voltage[4]=value4S;
    cell_Select(2);
}
#else
//获取电池电压
void Read_Cell_Voltage()
{
    /*ADC数据处理*/      
    Get_Adc_Val(&vol_bat,&vol_1s,&vol_2s,&vol_3s,&vol_4s);

    adc_values.vrefint =4096 *1.204/vrefintnum;
    adc_values.temperature = ((1450- (temperature *adc_values.vrefint*1000/4096))/(4300/1000))+25;

    valueBAT = (float)(vol_bat *  adc_values.vrefint/4096)*5.545;
    value1S = (float)( vol_1s  *  adc_values.vrefint/4096)*6;
    value2S = (float)( vol_2s  *  adc_values.vrefint/4096)*6;
    value3S = (float)( vol_3s  *  adc_values.vrefint/4096)*6; 
    value4S = (float)( vol_4s  *  adc_values.vrefint/4096)*6;  
     
    adc_values.cell_voltage[0]=valueBAT;
    adc_values.cell_voltage[1]=value1S;
    adc_values.cell_voltage[2]=value2S-value1S;
    adc_values.cell_voltage[3]=value3S-value2S;
    adc_values.cell_voltage[4]=value4S-value3S;
}

#endif

void Balance_Connection_State(void)
{
if (( value4S*BATTERY_ADC_MULTIPLIER  > VOLTAGE_CONNECTED_THRESHOLD ) && ( Get_Cell_Voltage(4)  > VOLTAGE_CONNECTED_THRESHOLD )) {
		cell_connected_bitmask |= 0b1000;
	}
	else {
		cell_connected_bitmask &= ~0b1000;
	}
	if (( value3S*BATTERY_ADC_MULTIPLIER > VOLTAGE_CONNECTED_THRESHOLD ) && ( Get_Cell_Voltage(3)  > VOLTAGE_CONNECTED_THRESHOLD )) {
		cell_connected_bitmask |= 0b0100;
	}
	else {
		cell_connected_bitmask &= ~0b0100;
	}
	if (( value2S*BATTERY_ADC_MULTIPLIER > VOLTAGE_CONNECTED_THRESHOLD ) && ( Get_Cell_Voltage(2)  > VOLTAGE_CONNECTED_THRESHOLD )) {
		cell_connected_bitmask |= 0b0010;
	}
	else {
		cell_connected_bitmask &= ~0b0010;
	}
	if ( Get_Cell_Voltage(1) > VOLTAGE_CONNECTED_THRESHOLD ) {
		cell_connected_bitmask |= 0b0001;
	}
	else {
		cell_connected_bitmask &= ~0b0001;
	}

	if ( cell_connected_bitmask & (1<<3) ) {
		if ( (cell_connected_bitmask & THREE_S_BITMASK) == THREE_S_BITMASK ) {
			battery_state.number_of_cells = 4;
			Clear_Error_State(CELL_CONNECTION_ERROR);
		}
		else {
			battery_state.number_of_cells = 0;
			Set_Error_State(CELL_CONNECTION_ERROR);
		}
	}
	else if ( cell_connected_bitmask & (1<<2) ) {
		if ( (cell_connected_bitmask & TWO_S_BITMASK) ==  TWO_S_BITMASK ) {
			battery_state.number_of_cells = 3;
			Clear_Error_State(CELL_CONNECTION_ERROR);
		}
		else {
			battery_state.number_of_cells = 0;
			Set_Error_State(CELL_CONNECTION_ERROR);
		}
	}
	else if ( cell_connected_bitmask & (1<<1) ) {
		if ( (cell_connected_bitmask & ONE_S_BITMASK) == ONE_S_BITMASK ) {
			battery_state.number_of_cells = 2;
			Clear_Error_State(CELL_CONNECTION_ERROR);
		}
		else {
			battery_state.number_of_cells = 0;
			Set_Error_State(CELL_CONNECTION_ERROR);
		}
	}
	else {
		battery_state.number_of_cells = 0;
		Clear_Error_State(CELL_CONNECTION_ERROR);
	}

	if ( battery_state.number_of_cells > 1 ) {
		battery_state.balance_port_connected = CONNECTED;
	}
	else {
		battery_state.balance_port_connected = NOT_CONNECTED;
	}
}


//电池超压检测
void Cell_Voltage_Safety_Check()
{
	uint8_t over_voltage_temp = 0;
	uint8_t under_voltage_temp = 0;

	for (int i = 1; i <= battery_state.number_of_cells; i++) {
		if (Get_Cell_Voltage(i)  > CELL_OVER_VOLTAGE_DISABLE_CHARGING) {
			over_voltage_temp = 1;
		}

		if (Get_Cell_Voltage(i)  < MIN_CELL_VOLTAGE_SAFE_LIMIT) {
			under_voltage_temp = 1;
		}
	}

	if (under_voltage_temp == 1) {
		Set_Error_State(CELL_VOLTAGE_ERROR);
	}
	else {
		Clear_Error_State(CELL_VOLTAGE_ERROR);
	}

	battery_state.cell_over_voltage = over_voltage_temp;
}

uint32_t vol_error;
void Battery_Connection_State()
{

    vol_error = fabs( adc_values.cell_voltage[0]-(adc_values.cell_voltage[1] + adc_values.cell_voltage[2] + adc_values.cell_voltage[3] + adc_values.cell_voltage[4]))*BATTERY_ADC_MULTIPLIER;

	if ( valueBAT*BATTERY_ADC_MULTIPLIER > battery_state.number_of_cells * VOLTAGE_CONNECTED_THRESHOLD && vol_error < VOLTAGE_CONNECTED_THRESHOLD) {
		battery_state.xt_connected = CONNECTED;
	}
	else {
		battery_state.xt_connected = NOT_CONNECTED;
	}

	Balance_Connection_State();

//	MCU_Temperature_Safety_Check();

	Cell_Voltage_Safety_Check();

	//Only update the balancing state if charging is off
	if (Get_Regulator_Charging_State() == 0) {
		Balance_Battery();
	}
//cellvol1 = valueBAT * BATTERY_ADC_MULTIPLIER;
//cellvol2 = battery_state.number_of_cells * CELL_VOLTAGE_TO_ENABLE_CHARGING;
	if ((battery_state.xt_connected == CONNECTED) && (battery_state.balance_port_connected == CONNECTED)){
		if (Get_Cell_Voltage(0) < (battery_state.number_of_cells * CELL_VOLTAGE_TO_ENABLE_CHARGING)) {
			battery_state.requires_charging = 1;
		}
		else {
			battery_state.requires_charging = 0;
		}
	}
    else {
		battery_state.requires_charging = 0;
	}
}


uint8_t Get_Balancing_State()
{
    if (battery_state.balancing_enabled == 1) {
		return battery_state.cell_balance_bitmask;
	}
	return 0;
}

uint8_t Get_Balance_Connection_State()
{
	return battery_state.balance_port_connected;
}

uint8_t Get_Requires_Charging_State()
{
	return battery_state.requires_charging;
}

uint8_t Get_Number_Of_Cells()
{
    Balance_Connection_State();
	return battery_state.number_of_cells;
}

uint8_t Get_XT_Connection_State()
{
	return battery_state.xt_connected;
}




