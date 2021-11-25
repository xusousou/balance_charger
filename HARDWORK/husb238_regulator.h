#ifndef __HUSB238_REGULATOR_H
#define __HUSB238_REGULATOR_H

#include "board.h"

#ifdef __cplusplus
extern "C" {
#endif


struct Husb238_Regulator {
    uint8_t Husb238_connected;

	uint8_t Husb238_PD_STATUS0;
    uint8_t PD_SRC_VOLTAGE;
    float PD_SRC_CURRENT;
    float PD_INPUT_POWER;

	uint8_t Husb238_PD_STATUS1;
	uint8_t Husb238_CC_DIR;
	uint8_t Husb238_ATTACH;
	uint8_t Husb238_PD_RESPONSE;

	uint8_t Husb238_VOLTAGE;
};

#define Husb238_PDO      (0x08<<1)

#define PD_STATUS0		0x00
#define PD_STATUS1		0x01
#define SRC_PDO_5V		0x02
#define SRC_PDO_9V		0x03
#define SRC_PDO_12V		0x04
#define SRC_PDO_15V		0x05
#define SRC_PDO_18V		0x06
#define SRC_PDO_20V		0x07
#define SRC_PDO	    	0x08
#define GO_COMMAND		0x09

#define SET_POD_SELECT  0x01

#define Notselected        0x00//0b0000
#define Husb238_PDO_5V     0x10//0b0001
#define Husb238_PDO_9V     0x20//0b0010
#define Husb238_PDO_12V    0x30//0b0011
#define Husb238_PDO_15V    0x80//0b1000
#define Husb238_PDO_18V    0x90//0b1001
#define Husb238_PDO_20V    0xA0//0b1010


uint8_t Husb238_IIC_ReadOneByte(uint8_t ReadAddr);
void Husb238_IIC_Read_Register(uint8_t ReadAddr,uint8_t *data,uint8_t len);
void Husb238_IIC_Write_Register(uint8_t WriteAddr,uint8_t DataToWrite);

void HUSB238_Query_Regulator_Connection();
void Husb238_Set_PDO_Voltage();
void HUSB238_Input_Power();
void Husb238_Voltage_Query();

uint32_t Get_HUSB238_Connection();
uint32_t Get_HUSB238_Input_Power();

#endif
