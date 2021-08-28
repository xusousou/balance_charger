#include "bq25703a_regulator.h"
#include "iic.h"


struct Regulator regulator;


void BQ25703A_init(void)
{
    rcu_periph_clock_enable(CHRG_OK_CLK);
    rcu_periph_clock_enable(ILIM_HIZ_CLK);
    rcu_periph_clock_enable(EN_OTG_CLK);

    gpio_mode_set(EN_OTG_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,EN_OTG_PIN);
    gpio_mode_set(ILIM_HIZ_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,ILIM_HIZ_PIN);
    gpio_mode_set(CHRG_OK_PORT,GPIO_MODE_INPUT,GPIO_PUPD_NONE,CHRG_OK_PIN);

    gpio_output_options_set(EN_OTG_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,EN_OTG_PIN);
    gpio_output_options_set(ILIM_HIZ_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, ILIM_HIZ_PIN);
}



uint8_t IIC_ReadOneByte(uint16_t ReadAddr)	
{
	uint8_t temp;
	i2c_start();
	i2c_send_byte(BQ26703A_I2C_ADDRESS);
	i2c_wait_ack();
	i2c_send_byte(ReadAddr);
	i2c_wait_ack();
	i2c_start();
	i2c_send_byte(BQ26703A_I2C_ADDRESS+1);
	i2c_wait_ack();
	temp=i2c_read_byte(); 
    i2c_nack();
	i2c_stop();
    softDelayUs(10);
	return temp;
}

void I2C_Read_Register(uint16_t ReadAddr,uint8_t data[],uint8_t len)	
{
    uint8_t i=0;
	i2c_start();
	i2c_send_byte(BQ26703A_I2C_ADDRESS);
	i2c_wait_ack();
	i2c_send_byte(ReadAddr);
	i2c_wait_ack();
	i2c_start();
	i2c_send_byte(BQ26703A_I2C_ADDRESS+1);
	i2c_wait_ack();

    for(i=0; i<len; i++)
    {
        data[i] = i2c_read_byte();
        i2c_ack();
    }
    data[i++] = i2c_read_byte();

    i2c_nack();
    i2c_stop();
    softDelayUs(10);
}

void I2C_Write_Register(uint16_t WriteAddr,uint8_t *DataToWrite)
{
	i2c_start();
	i2c_send_byte(BQ26703A_I2C_ADDRESS);
	i2c_wait_ack();
	i2c_send_byte(WriteAddr);
	i2c_wait_ack();
	i2c_send_byte(*DataToWrite);
	i2c_wait_ack();
	i2c_stop();
    softDelayUs(10);
}

void I2C_Write_Two_Byte_Register(uint8_t WriteAddr, uint8_t lsb_data, uint8_t msb_data)		
{
	i2c_start();
	i2c_send_byte(BQ26703A_I2C_ADDRESS);
	i2c_wait_ack();
	i2c_send_byte(WriteAddr);
	i2c_wait_ack();
	i2c_send_byte(lsb_data);
	i2c_wait_ack();
	i2c_send_byte(msb_data);
	i2c_wait_ack();
	i2c_stop();
    softDelayUs(10);
}

/**********************************************************************************************************
*获取充电芯片连接状态
**********************************************************************************************************/
uint8_t Get_Regulator_Connection_State(void){
	return regulator.connected;
}
/**********************************************************************************************************
*获取充电芯片充电状态
**********************************************************************************************************/
uint8_t Get_Regulator_Charging_State(void){
	return regulator.charging_status;
}
/**********************************************************************************************************
*获取充电芯片VBAT电压
**********************************************************************************************************/
uint32_t Get_VBAT_ADC_Reading() {
	return regulator.vbat_voltage;
}
/**********************************************************************************************************
*获取充电芯片VBUS电压
**********************************************************************************************************/
uint32_t Get_VBUS_ADC_Reading() {
	return regulator.vbus_voltage;
}
/**********************************************************************************************************
*获取充电芯片输入电流
**********************************************************************************************************/
uint32_t Get_Input_Current_ADC_Reading() {
	return regulator.input_current;
}
/**********************************************************************************************************
*获取充电芯片输出电流
**********************************************************************************************************/
uint32_t Get_Charge_Current_ADC_Reading() {
	return regulator.charge_current;
}
/**********************************************************************************************************
*获取充电芯片最大充电电流
**********************************************************************************************************/
uint32_t Get_Max_Charge_Current() {
	return regulator.max_charge_current_ma;
}

/**********************************************************************************************************
*检查充电芯片I2C连接
**********************************************************************************************************/
uint8_t Query_Regulator_Connection() {
	/* Get the manufacturer id */
	uint8_t manufacturer_id;
	I2C_Read_Register(MANUFACTURER_ID_ADDR, (uint8_t *) &manufacturer_id, 1);

	/* Get the device id */
	uint8_t device_id;
	I2C_Read_Register(DEVICE_ID_ADDR, (uint8_t *) &device_id, 1);

	if ( (device_id == BQ26703A_DEVICE_ID) && (manufacturer_id == BQ26703A_MANUFACTURER_ID) ) {
		Clear_Error_State(REGULATOR_COMMUNICATION_ERROR);
		return CONNECTED;
	}
	else {
		Set_Error_State(REGULATOR_COMMUNICATION_ERROR);
		return NOT_CONNECTED;
	}
}

/**********************************************************************************************************
*检查 Charge OK 引脚的状态并返回值
**********************************************************************************************************/
uint8_t Read_Charge_Okay() {
	return gpio_input_bit_get(CHRG_OK_PORT, CHRG_OK_PIN);
}

/**********************************************************************************************************
*读取 ChargeStatus 寄存器并设置状态
**********************************************************************************************************/
void Read_Charge_Status() {
	uint8_t data[2];
	I2C_Read_Register(CHARGE_STATUS_ADDR, data, 2);

	if (data[1] & CHARGING_ENABLED_MASK) {
		regulator.charging_status = 1;
	}
	else {
		regulator.charging_status = 0;
	}
}

/**********************************************************************************************************
*设置充电芯片 ADC 设置
**********************************************************************************************************/
void Regulator_Set_ADC_Option() {

	uint8_t ADC_lsb_3A = ADC_ENABLED_BITMASK;

	I2C_Write_Register(ADC_OPTION_ADDR, (uint8_t *) &ADC_lsb_3A);
}

/**********************************************************************************************************
*启动并读取充电芯片单个 ADC 转换
**********************************************************************************************************/
void Regulator_Read_ADC() {
	TickType_t xDelay = 80 / portTICK_PERIOD_MS;

	uint8_t ADC_msb_3B = ADC_START_CONVERSION_MASK;

	I2C_Write_Register((ADC_OPTION_ADDR+1), (uint8_t *) &ADC_msb_3B);

	/* Wait for the conversion to finish */
	while (ADC_msb_3B & (1<<6)) {
		vTaskDelay(xDelay);
		I2C_Read_Register((ADC_OPTION_ADDR+1), (uint8_t *) &ADC_msb_3B, 1);
	}

	uint8_t temp = 0;

	I2C_Read_Register(VBAT_ADC_ADDR, (uint8_t *) &temp, 1);
	regulator.vbat_voltage = (temp * VBAT_ADC_SCALE) + VBAT_ADC_OFFSET;

	I2C_Read_Register(VSYS_ADC_ADDR, (uint8_t *) &temp, 1);
	regulator.vsys_voltage = (temp * VSYS_ADC_SCALE) + VSYS_ADC_OFFSET;

	I2C_Read_Register(ICHG_ADC_ADDR, (uint8_t *) &temp, 1);
	regulator.charge_current = temp * ICHG_ADC_SCALE;

	I2C_Read_Register(IIN_ADC_ADDR, (uint8_t *) &temp, 1);
	regulator.input_current = temp * IIN_ADC_SCALE;

	I2C_Read_Register(VBUS_ADC_ADDR, (uint8_t *) &temp, 1);
	regulator.vbus_voltage = (temp * VBUS_ADC_SCALE) + VBUS_ADC_OFFSET;
}

/**********************************************************************************************************
*充电芯片输出启用或禁用高阻抗模式
**********************************************************************************************************/
void Regulator_HI_Z(uint8_t hi_z_en) {
	if (hi_z_en == 1) {
		gpio_bit_reset(ILIM_HIZ_PORT, ILIM_HIZ_PIN);
	}
	else {
		gpio_bit_set(ILIM_HIZ_PORT, ILIM_HIZ_PIN);
	}
}

/**********************************************************************************************************
*启用或禁用 On the Go 模式
**********************************************************************************************************/
void Regulator_OTG_EN(uint8_t otg_en) {
	if (otg_en == 0) {
		gpio_bit_reset(EN_OTG_PORT, EN_OTG_PIN);
	}
	else {
		gpio_bit_reset(EN_OTG_PORT, EN_OTG_PIN);
	}
}

/**********************************************************************************************************
*设置Charge Option 0 
**********************************************************************************************************/
void Regulator_Set_Charge_Option_0() {

	uint8_t charge_option_0_register_1_value = 0b00100110;
	uint8_t charge_option_0_register_2_value = 0b00001110;

	I2C_Write_Two_Byte_Register(CHARGE_OPTION_0_ADDR, charge_option_0_register_2_value, charge_option_0_register_1_value);

	return;
}

/**********************************************************************************************************
*设置充电电流限制。 从 64mA 到 8.128A，步进为 64mA。 映射从 0 - 128。7 位值。
**********************************************************************************************************/
void Set_Charge_Current(uint32_t charge_current_limit) {

	uint8_t charge_current_register_1_value = 0;
	uint8_t charge_current_register_2_value = 0;
	uint32_t charge_current = 0,current_limit = 0 ;
  current_limit = (charge_current_limit*10)/10.0;   //采样电阻约为12.6mΩ；

	if (current_limit > MAX_CHARGE_CURRENT_MA) {
		current_limit = MAX_CHARGE_CURRENT_MA;
	}  
    
//	regulator.max_charge_current_ma = charge_current_limit;

	if (current_limit != 0){
		charge_current = current_limit/64;
	}
	if (charge_current > 128) {
		charge_current = 128;
	}
	//0-128 which remaps from 64mA-8.128A. 7 bit value.

    
	if (charge_current <= 128) {
		charge_current_register_1_value = ((charge_current & 0x03)<<6);
		charge_current_register_2_value = ((charge_current >> 2)&0x1f);
	}

	I2C_Write_Two_Byte_Register(CHARGE_CURRENT_ADDR, charge_current_register_1_value,charge_current_register_2_value );

	return ;


//	uint32_t charge_current = 0;

//	if (charge_current_limit > MAX_CHARGE_CURRENT_MA) {
//		charge_current_limit = MAX_CHARGE_CURRENT_MA;
//	}

//	regulator.max_charge_current_ma = charge_current_limit;

//	if (charge_current_limit != 0){
//		charge_current = charge_current_limit/64;
//	}

//	if (charge_current > 128) {
//		charge_current = 128;
//	}

//	//0-128 which remaps from 64mA-8.128A. 7 bit value.
//	uint8_t charge_current_register_1_value = 0;
//	uint8_t charge_current_register_2_value = 0;

//	if ((charge_current >= 0) || (charge_current <= 128)) {
//		charge_current_register_1_value = (charge_current >> 2);
//		charge_current_register_2_value = (charge_current << 6);
//	}

//	I2C_Write_Two_Byte_Register(CHARGE_CURRENT_ADDR, charge_current_register_2_value, charge_current_register_1_value);

//	return;
}


/**********************************************************************************************************
*根据电池数量设置充电电压。 1 - 4.192V, 2 - 8.400V, 3 - 12.592V, 4 - 16.800V
**********************************************************************************************************/
void Set_Charge_Voltage(uint8_t number_of_cells) {

	uint8_t max_charge_register_1_value = 0;
	uint8_t max_charge_register_2_value = 0;

	uint8_t	minimum_system_voltage_value = MIN_VOLT_ADD_1024_MV;

	if ((number_of_cells > 0) || (number_of_cells < 5)) {
		switch (number_of_cells) {
			case 1:
				max_charge_register_1_value = MAX_VOLT_ADD_4096_MV;
				max_charge_register_2_value = MAX_VOLT_ADD_64_MV | MAX_VOLT_ADD_32_MV;
				minimum_system_voltage_value = MIN_VOLT_ADD_2048_MV | MIN_VOLT_ADD_512_MV | MIN_VOLT_ADD_256_MV;
				break;
			case 2:
				max_charge_register_1_value = MAX_VOLT_ADD_8192_MV;
				max_charge_register_2_value = MAX_VOLT_ADD_128_MV | MAX_VOLT_ADD_64_MV | MAX_VOLT_ADD_16_MV;
				minimum_system_voltage_value = MIN_VOLT_ADD_4096_MV | MIN_VOLT_ADD_1024_MV | MIN_VOLT_ADD_512_MV;
				break;
			case 3:
				max_charge_register_1_value = MAX_VOLT_ADD_8192_MV | MAX_VOLT_ADD_4096_MV | MAX_VOLT_ADD_256_MV;
				max_charge_register_2_value = MAX_VOLT_ADD_32_MV | MAX_VOLT_ADD_16_MV;
				minimum_system_voltage_value = MIN_VOLT_ADD_8192_MV |  MIN_VOLT_ADD_256_MV;
				break;
			case 4:
				max_charge_register_1_value = MAX_VOLT_ADD_16384_MV | MAX_VOLT_ADD_256_MV;
				max_charge_register_2_value = MAX_VOLT_ADD_128_MV | MAX_VOLT_ADD_32_MV;
				minimum_system_voltage_value = MIN_VOLT_ADD_8192_MV | MIN_VOLT_ADD_2048_MV | MIN_VOLT_ADD_1024_MV;
				break;
			default:
				max_charge_register_1_value = 0;
				max_charge_register_2_value = 0;
				minimum_system_voltage_value = MIN_VOLT_ADD_1024_MV;
				break;
			}
	}

	I2C_Write_Register(MINIMUM_SYSTEM_VOLTAGE_ADDR, (uint8_t *) &minimum_system_voltage_value);

	I2C_Write_Two_Byte_Register(MAX_CHARGE_VOLTAGE_ADDR, max_charge_register_2_value, max_charge_register_1_value);

	return;
}

/**********************************************************************************************************
*根据 MCU 温度计算最大充电功率
**********************************************************************************************************/
uint32_t Calculate_Max_Charge_Power() {

//	//Account for system losses with ASSUME_EFFICIENCY fudge factor to not overload source
//	uint32_t charging_power_mw = (((float)(regulator.vbus_voltage/REG_ADC_MULTIPLIER) * Get_Max_Input_Current()) * ASSUME_EFFICIENCY);

//	if (charging_power_mw > MAX_CHARGING_POWER) {
//		charging_power_mw = MAX_CHARGING_POWER;
//	}

//	if (charging_power_mw > Get_Max_Input_Power()){
//		charging_power_mw = Get_Max_Input_Power() * ASSUME_EFFICIENCY;
//	}

//	//Throttle charging power if temperature is too high
//	if (Get_MCU_Temperature() > TEMP_THROTTLE_THRESH_C){
//		float temperature = (float)Get_MCU_Temperature();

//		float power_scalar = 1.0f - ((float)(0.0333 * temperature) - 1.33f);

//		if (power_scalar > 1.0f) {
//			power_scalar = 1.0f;
//		}
//		if (power_scalar < 0.00f) {
//			power_scalar = 0.00f;
//		}

//		charging_power_mw = charging_power_mw * power_scalar;
//	}

//	return charging_power_mw;
    return 0;
}

/**********************************************************************************************************
*确定充电器输出是否应该打开并根据需要设置电压和电流参数
**********************************************************************************************************/
//void Control_Charger_Output(uint8_t cell_Num,uint16_t cell_CUR,uint8_t cell_HI_Z) {

//		Set_Charge_Voltage(cell_Num);

//		Set_Charge_Current(cell_CUR);

//		Regulator_HI_Z(cell_HI_Z);
//}
void Control_Charger_Output(float vol,uint8_t CELL)
{
    uint8_t cell_Num, cell_HI_Z;
    uint16_t cell_CUR;

    uint16_t  CUR_value, CUR_min, CUR_max,vol_min,vol_max;
    CUR_max=2000;
    CUR_min=200;

    Balance_Connection_State();
    cell_Num = Get_Number_Of_Cells();
    vol_max=(CELL*4.20)*1000;
    vol_min=(CELL*4.20-2.55)*1000;

    if((vol*1000)>vol_min && (vol*1000)<(vol_max-400) && cell_Num>1){
        cell_CUR = CUR_max;
    }else if((vol*1000)>(vol_max-400) && (vol*1000)<(vol_max-100)){
        cell_CUR = CUR_max*(1-(vol*1000)/vol_max);
    }else if((vol*1000)>(vol_max-100) && (vol*1000)<vol_max){
        cell_CUR= CUR_min;
    }else cell_CUR= 0;
        
    if(Battery_Connection_State() == CONNECTED && Get_Error_State() == 0){
	Set_Charge_Voltage(cell_Num);

	Set_Charge_Current(cell_CUR);

	Regulator_HI_Z(0);
    }
}
