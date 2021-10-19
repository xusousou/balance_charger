#ifndef BQ25703A_REGULATOR_H_
#define BQ25703A_REGULATOR_H_

#include "board.h"


#ifdef __cplusplus
extern "C" {
#endif


struct Regulator {
	uint8_t connected;
	uint8_t charging_status;
	uint16_t max_charge_voltage;
	uint8_t input_current_limit;
	uint16_t min_input_voltage_limit;
	uint32_t vbus_voltage;
	uint32_t vbat_voltage;
	uint32_t vsys_voltage;
	uint32_t charge_current;
	uint32_t input_current;
	uint32_t max_charge_current_ma;
};

#define BQ26703A_I2C_ADDRESS		0xD6
#define BQ26703A_MANUFACTURER_ID	0x40
#define BQ26703A_DEVICE_ID			0x78

#define MANUFACTURER_ID_ADDR		0x2E
#define DEVICE_ID_ADDR				0x2F
#define MAX_CHARGE_VOLTAGE_ADDR		0x04
#define CHARGE_CURRENT_ADDR			0x02
#define CHARGE_OPTION_0_ADDR		0x00
#define MINIMUM_SYSTEM_VOLTAGE_ADDR	0x0D
#define CHARGE_STATUS_ADDR			0x20
#define ADC_OPTION_ADDR				0x3A
#define VBUS_ADC_ADDR				0x27
#define PSYS_ADC_ADDR				0x26
#define VSYS_ADC_ADDR				0x2D
#define VBAT_ADC_ADDR				0x2C
#define ICHG_ADC_ADDR				0x29
#define IDCHG_ADC_ADDR				0x28
#define IIN_ADC_ADDR				0x2B

#define EN_LWPWR					0x00
#define EN_OOA						0x01

#define CHARGING_ENABLED_MASK		0x04//0b00000100
#define ADC_ENABLED_BITMASK			0x57//0b01010111
#define ADC_START_CONVERSION_MASK	0x60//0b01100000

//Max voltage register 1 values
#define MAX_VOLT_ADD_16384_MV		0x40//0b01000000
#define MAX_VOLT_ADD_8192_MV		0x20//0b00100000
#define MAX_VOLT_ADD_4096_MV		0x10//0b00010000
#define MAX_VOLT_ADD_2048_MV		0x08//0b00001000
#define MAX_VOLT_ADD_1024_MV		0x04//0b00000100
#define MAX_VOLT_ADD_512_MV			0x02//0b00000010
#define MAX_VOLT_ADD_256_MV			0x01//0b00000001

//Max voltage register 2 values
#define MAX_VOLT_ADD_128_MV			0x80//0b10000000
#define MAX_VOLT_ADD_64_MV			0x40//0b01000000
#define MAX_VOLT_ADD_32_MV			0x20//0b00100000
#define MAX_VOLT_ADD_16_MV			0x10//0b00010000

//Minimum system voltage register values
#define MIN_VOLT_ADD_8192_MV		0x20//0b00100000
#define MIN_VOLT_ADD_4096_MV		0x10//0b00010000
#define MIN_VOLT_ADD_2048_MV		0x08//0b00001000
#define MIN_VOLT_ADD_1024_MV		0x04//0b00000100
#define MIN_VOLT_ADD_512_MV			0x02//0b00000010
#define MIN_VOLT_ADD_256_MV			0x01//0b00000001

#define REG_ADC_MULTIPLIER			100000

#define VBUS_ADC_SCALE				(uint32_t)( 0.064 * REG_ADC_MULTIPLIER )
#define VBUS_ADC_OFFSET				(uint32_t)( 3.2 * REG_ADC_MULTIPLIER )

#define PSYS_ADC_SCALE				(uint32_t)( 0.012 * REG_ADC_MULTIPLIER )

#define VSYS_ADC_SCALE				(uint32_t)(0.064 * REG_ADC_MULTIPLIER)
#define VSYS_ADC_OFFSET				(uint32_t)(2.88 * REG_ADC_MULTIPLIER)

#define VBAT_ADC_SCALE				(uint32_t)(0.064 * REG_ADC_MULTIPLIER)
#define VBAT_ADC_OFFSET				(uint32_t)(2.88 * REG_ADC_MULTIPLIER)

#define ICHG_ADC_SCALE				(uint32_t)(0.064 * REG_ADC_MULTIPLIER)

#define IIN_ADC_SCALE				(uint32_t)(0.050 * REG_ADC_MULTIPLIER)

#define MAX_CHARGE_CURRENT_MA		6000
#define ASSUME_EFFICIENCY			0.85f
#define BATTERY_DISCONNECT_THRESH	(uint32_t)(4.215 * REG_ADC_MULTIPLIER)
#define MAX_CHARGING_POWER			65000
#define NON_USB_PD_CHARGE_POWER		2500

#define TEMP_THROTTLE_THRESH_C		40


void BQ25703A_init(void);
uint8_t IIC_ReadOneByte(uint16_t ReadAddr);
void I2C_Read_Register(uint16_t ReadAddr,uint8_t data[],uint8_t len);
void I2C_Write_Register(uint16_t WriteAddr,uint8_t *DataToWrite);
void I2C_Write_Two_Byte_Register(uint8_t WriteAddr, uint8_t lsb_data, uint8_t msb_data);


uint8_t Get_Regulator_Connection_State(void);
uint8_t Get_Regulator_Charging_State(void);
uint32_t Get_VBAT_ADC_Reading(void);
uint32_t Get_VBUS_ADC_Reading(void);
uint32_t Get_Input_Current_ADC_Reading(void);
uint32_t Get_Charge_Current_ADC_Reading(void);
uint32_t Get_Max_Charge_Current(void);

void Regulator_HI_Z(uint8_t hi_z_en);
void Regulator_OTG_EN(uint8_t otg_en);
uint8_t Read_Charge_Okay(void);
void Regulator_Read_ADC();

uint8_t Query_Regulator_Connection(void);
void Regulator_Set_ADC_Option(void) ;
void Regulator_Set_Charge_Option_0(void);

void Read_Charge_Status(void);
//void Control_Charger_Output(uint8_t cell_Num,uint16_t cell_CUR,uint8_t cell_HI_Z);
void Control_Charger_Output(float vol,uint8_t CELL);
void Set_Charge_Current(uint32_t charge_current_limit);
void Set_Charge_Voltage(uint8_t number_of_cells);
#endif
