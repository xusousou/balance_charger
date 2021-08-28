#include "battery.h"



volatile struct Battery battery_state;



float valueBAT,value1S,value2S,value3S,value4S;
uint16_t vol_1s, vol_2s, vol_3s, vol_4s, vol_bat;   //uint16_t ADC_1, ADC_2, ADC_4;对应的adc通道顺序
uint8_t cell;

extern float batdata[6];

void SW_Init()
{
    rcu_periph_clock_enable(SWA_CLK);
    rcu_periph_clock_enable(SWB_CLK);
    
    gpio_deinit(SWA_PORT);
    gpio_deinit(SWB_PORT);

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


void Balance_Connection_State(void)
{
    battery_state.number_of_cells = 0;

    if(value1S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value2S<MIN_CELL_VOLTAGE_SAFE_LIMIT && value3S<MIN_CELL_VOLTAGE_SAFE_LIMIT && value4S<MIN_CELL_VOLTAGE_SAFE_LIMIT){
        if(valueBAT >= MIN_1SBAT_VOLTAGE && valueBAT <= MAX_1SBAT_VOLTAGE)
        battery_state.number_of_cells = 1;
    }else if(value1S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value2S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value3S<MIN_CELL_VOLTAGE_SAFE_LIMIT && value4S<MIN_CELL_VOLTAGE_SAFE_LIMIT){
        if(valueBAT >= MIN_2SBAT_VOLTAGE && valueBAT <= MAX_2SBAT_VOLTAGE)
        battery_state.number_of_cells = 2;
    }else if(value1S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value2S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value3S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value4S<MIN_CELL_VOLTAGE_SAFE_LIMIT){
        if(valueBAT >= MIN_3SBAT_VOLTAGE && valueBAT <= MAX_3SBAT_VOLTAGE)
        battery_state.number_of_cells = 3;
    }else if(value1S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value2S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value3S>MIN_CELL_VOLTAGE_SAFE_LIMIT && value4S>MIN_CELL_VOLTAGE_SAFE_LIMIT){
        if(valueBAT >= MIN_4SBAT_VOLTAGE && valueBAT <= MAX_4SBAT_VOLTAGE)
         battery_state.number_of_cells = 4;
    }else  battery_state.number_of_cells = 0;

}
//获取电池电压
void Read_Cell_Voltage()
{
    Balance_Connection_State();
    Battery_Connection_State();
//    vTaskDelay(1);
    //2S
    cell_Select(2);    
    for(uint16_t i=2000;i>0;i--)
    {
        /*ADC数据处理*/      
        Get_Adc_Val(&vol_bat,&vol_1s,&vol_2s);
        get_low_filter(&vol_bat,&vol_1s,&vol_2s);
    }
    value2S = (float)(batdata[2]/10 *3.3/4096)*2;

//    vTaskDelay(1);
    //3S
    cell_Select(3);
    for(uint16_t i=2000;i>0;i--)
    {
        /*ADC数据处理*/      
        Get_Adc_Val(&vol_bat,&vol_1s,&vol_3s);
        get_low_filter(&vol_bat,&vol_1s,&vol_3s);
    }
    value3S = (float)(batdata[2]/10 *3.3/4096)*2;

//    vTaskDelay(1);
    //4S
    cell_Select(4);
    for(uint16_t i=2000;i>0;i--)
    {
        /*ADC数据处理*/      
        Get_Adc_Val(&vol_bat,&vol_1s,&vol_4s);
        get_low_filter(&vol_bat,&vol_1s,&vol_4s);
    }
    valueBAT = (float)(batdata[0]/10 * 3.3/4096)*11;
    value1S = (float)(batdata[1]/10 *3.3/4096)*2+0.01;
    value4S = (float)(batdata[2]/10 *3.3/4096)*2;  
     
}

uint8_t Battery_Connection_State()
{
    if(battery_state.number_of_cells != 0) return CONNECTED;
    else return NOT_CONNECTED;
}

uint8_t Get_Number_Of_Cells()
{
	return battery_state.number_of_cells;
}

uint8_t Get_XT_Connection_State()
{
	return battery_state.xt_connected;
}

uint8_t Get_Balance_Connection_State()
{
	return battery_state.balance_port_connected;
}

uint8_t Get_Balancing_State()
{
    if (battery_state.balancing_enabled == 1) {
		return battery_state.cell_balance_bitmask;
	}
	return 0;
}

uint8_t Get_Requires_Charging_State()
{
	return battery_state.requires_charging;
}



