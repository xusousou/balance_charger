#include "husb238_regulator.h"

struct Husb238_Regulator PD_regulator;
 

void Husb238_IIC_Read_Register(uint8_t ReadAddr,uint8_t *data,uint8_t len)	
{
    uint8_t ia=0;
	HUSB238_i2c_start();
	HUSB238_i2c_send_byte(Husb238_PDO);
	HUSB238_i2c_wait_ack();
	HUSB238_i2c_send_byte(ReadAddr);
	HUSB238_i2c_wait_ack();
	HUSB238_i2c_start();
	HUSB238_i2c_send_byte(Husb238_PDO+1);
	HUSB238_i2c_wait_ack();

    for(ia=0; ia<len; ia++)
    {
        *data = HUSB238_i2c_read_byte();
    }
    HUSB238_i2c_nack();
    HUSB238_i2c_stop();
    softDelayUs(10);
}

void Husb238_IIC_Write_Register(uint8_t WriteAddr,uint8_t DataToWrite)
{
	HUSB238_i2c_start();
	HUSB238_i2c_send_byte(Husb238_PDO);
	HUSB238_i2c_wait_ack();
	HUSB238_i2c_send_byte(WriteAddr);
	HUSB238_i2c_wait_ack();
	HUSB238_i2c_send_byte(DataToWrite);
	HUSB238_i2c_wait_ack();
	HUSB238_i2c_stop();
    softDelayUs(10);
}

/**********************************************************************************************************
*检查HUSB238充电芯片
**********************************************************************************************************/
void HUSB238_Query_Regulator_Connection()
{
    Husb238_IIC_Read_Register(PD_STATUS0, &PD_regulator.Husb238_PD_STATUS0, 1);
    delay_us(500);
	Husb238_IIC_Read_Register(PD_STATUS1, &PD_regulator.Husb238_PD_STATUS1, 1);

	if ( (0x48 == (PD_regulator.Husb238_PD_STATUS1 & 0x48)) && (0x00 != (PD_regulator.Husb238_PD_STATUS0 & 0xf0)) ){
        if(0x00 != (PD_regulator.Husb238_PD_STATUS0 & 0x0f)){
            PD_regulator.Husb238_connected = 1;
        }else PD_regulator.Husb238_connected = 2;
    }else PD_regulator.Husb238_connected = 0;

}

void Husb238_Set_PDO_Voltage()
{   
    if( Get_HUSB238_Connection() > 0 )
    {
        Husb238_IIC_Read_Register( SRC_PDO_20V, &PD_regulator.Husb238_VOLTAGE,1 );
        if(0x80 == (PD_regulator.Husb238_VOLTAGE & 0x80) )
        {
            Husb238_IIC_Write_Register(SRC_PDO,Husb238_PDO_20V);   
            Husb238_IIC_Write_Register(GO_COMMAND,SET_POD_SELECT);
            return;
        }
        delay_us(500);
        Husb238_IIC_Read_Register( SRC_PDO_18V, &PD_regulator.Husb238_VOLTAGE,1 );
        if(0x80 == (PD_regulator.Husb238_VOLTAGE & 0x80) )
        {
            Husb238_IIC_Write_Register(SRC_PDO,Husb238_PDO_18V); 
            Husb238_IIC_Write_Register(GO_COMMAND,SET_POD_SELECT);  
            return;
        }
        delay_us(500);
        Husb238_IIC_Read_Register( SRC_PDO_15V, &PD_regulator.Husb238_VOLTAGE,1 );

        if(0x80 == (PD_regulator.Husb238_VOLTAGE & 0x80) )
        {
            Husb238_IIC_Write_Register(SRC_PDO,Husb238_PDO_15V);   
            Husb238_IIC_Write_Register(GO_COMMAND,SET_POD_SELECT);
            return;
        }
        delay_us(500);
        Husb238_IIC_Read_Register( SRC_PDO_12V, &PD_regulator.Husb238_VOLTAGE,1 );
        if(0x80 == (PD_regulator.Husb238_VOLTAGE & 0x80) )
        {
            Husb238_IIC_Write_Register(SRC_PDO,Husb238_PDO_12V);  
            Husb238_IIC_Write_Register(GO_COMMAND,SET_POD_SELECT); 
            return;
        }
        delay_us(500);
        Husb238_IIC_Read_Register( SRC_PDO_9V, &PD_regulator.Husb238_VOLTAGE,1 );
        if(0x80 == (PD_regulator.Husb238_VOLTAGE & 0x80) )
        {
            Husb238_IIC_Write_Register(SRC_PDO,Husb238_PDO_9V);  
            Husb238_IIC_Write_Register(GO_COMMAND,SET_POD_SELECT); 
            return;
        }
        delay_us(500);
        Husb238_IIC_Read_Register( SRC_PDO_5V, &PD_regulator.Husb238_VOLTAGE,1 );
        if(0x80 == (PD_regulator.Husb238_VOLTAGE & 0x80) )
        {
            Husb238_IIC_Write_Register(SRC_PDO,Husb238_PDO_5V);  
            Husb238_IIC_Write_Register(GO_COMMAND,SET_POD_SELECT); 
            return;
        }
        delay_us(500);
    }
}

void HUSB238_Input_Power()
{
    Husb238_IIC_Read_Register(PD_STATUS0, &PD_regulator.Husb238_PD_STATUS0, 1);
    switch(PD_regulator.Husb238_PD_STATUS0 & 0xf0)
    {
        case 0x00:
        break;
        case 0x10:
            PD_regulator.PD_SRC_VOLTAGE = 5;
        break;
        case 0x20:
            PD_regulator.PD_SRC_VOLTAGE = 9;
        break;
        case 0x30:
            PD_regulator.PD_SRC_VOLTAGE = 12;
        break;
        case 0x40:
            PD_regulator.PD_SRC_VOLTAGE = 15;
        break;
        case 0x50:
            PD_regulator.PD_SRC_VOLTAGE = 18;
        break;
        case 0x60:
            PD_regulator.PD_SRC_VOLTAGE = 20;
        break;
    }

    switch(PD_regulator.Husb238_PD_STATUS0 & 0x0f)
    {
        case 0x00:
            PD_regulator.PD_SRC_CURRENT = 0.5;
        break;
        case 0x01:
            PD_regulator.PD_SRC_CURRENT = 0.7;
        break;
        case 0x02:
            PD_regulator.PD_SRC_CURRENT = 1.0;
        break;
        case 0x03:
            PD_regulator.PD_SRC_CURRENT = 1.25;
        break;
        case 0x04:
            PD_regulator.PD_SRC_CURRENT = 1.5;
        break;
        case 0x05:
            PD_regulator.PD_SRC_CURRENT = 1.75;
        break;
        case 0x06:
            PD_regulator.PD_SRC_CURRENT = 2.0;
        break;
        case 0x07:
            PD_regulator.PD_SRC_CURRENT = 2.25;
        break;
        case 0x08:
            PD_regulator.PD_SRC_CURRENT = 2.5;
        break;
        case 0x09:
            PD_regulator.PD_SRC_CURRENT = 2.75;
        break;
        case 0x0A:
            PD_regulator.PD_SRC_CURRENT = 3.0;
        break;
        case 0x0B:
            PD_regulator.PD_SRC_CURRENT = 3.25;
        break;
        case 0x0C:
            PD_regulator.PD_SRC_CURRENT = 3.5;
        break;
        case 0x0D:
            PD_regulator.PD_SRC_CURRENT = 4.0;
        break;
        case 0x0E:
            PD_regulator.PD_SRC_CURRENT = 4.5;
        break;
        case 0x0F:
            PD_regulator.PD_SRC_CURRENT = 5.0;
        break;
    }
    if( 1 == Get_HUSB238_Connection() ){
        PD_regulator.PD_INPUT_POWER = PD_regulator.PD_SRC_CURRENT * PD_regulator.PD_SRC_VOLTAGE;
    }else if( 2 == Get_HUSB238_Connection() ){
        PD_regulator.PD_INPUT_POWER = 30;
    }
    else PD_regulator.PD_INPUT_POWER = 4.50;
}

void Husb238_Voltage_Query()
{
    HUSB238_Query_Regulator_Connection();
    Husb238_Set_PDO_Voltage();
    HUSB238_Input_Power();
}


uint32_t Get_HUSB238_Connection() {
	return PD_regulator.Husb238_connected;
}

uint32_t Get_HUSB238_Input_Power(){
	return PD_regulator.PD_INPUT_POWER;
}
