#ifndef __DRIVER_I2C_H__
#define __DRIVER_I2C_H__
#include "board.h"
#include "boardConfig.h"
#include "stdbool.h"
#include "stdint.h"

void Soft_I2c_Open(uint8_t deviceNum);
bool Soft_I2c_Single_Write(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
void  Soft_I2c_Multi_Write(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t WriteAddr, uint8_t lsb_data, uint8_t msb_data);


uint8_t Soft_I2C_Single_Read(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t REG_Address);
bool Soft_I2C_Multi_Read(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size);

#endif /* __I2C_H__ */
