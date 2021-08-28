#include "drv_i2c.h"

#define SOFT_I2C1_DELAY 4
#define READ_SDA_PIN     gpio_input_bit_get(GPIOB,GPIO_PIN_7)


#define SDA_MODE_IN()  {GPIO_CTL(GPIOB)&=0X0FFFFFFF;GPIO_CTL(GPIOB)|=(uint32_t)8<<28;}
#define SDA_MODE_OUT() {GPIO_CTL(GPIOB)&=0X0FFFFFFF;GPIO_CTL(GPIOB)|=(uint32_t)3<<28;}

static void delay_us(int us)
{
    softDelayUs(us);
}
void Soft_I2c_Open(uint8_t deviceNum)
{

    rcu_periph_clock_enable(IIC_SCL_GPIO_CLK);
    rcu_periph_clock_enable(IIC_SDA_GPIO_CLK);

    gpio_mode_set(IIC_SCL_GPIO_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,IIC_SCL_PIN);
    gpio_mode_set(IIC_SDA_GPIO_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_PULLUP,IIC_SDA_PIN);

    gpio_output_options_set(IIC_SCL_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, IIC_SCL_PIN);
    gpio_output_options_set(IIC_SDA_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC_SDA_PIN);
	
	IIC_SCL(1);
	IIC_SDA(1);

//    GPIO_InitTypeDef  GPIO_InitStructure;

//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//    if(deviceNum == 1)
//    {
//        GPIO_InitStructure.GPIO_Pin =  SOFT_I2C1_PIN_SCL | SOFT_I2C1_PIN_SDA;
//        GPIO_Init(SOFT_I2C1_PORT, &GPIO_InitStructure);
//    }
//    else if(deviceNum == 2)
//    {

//    }
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_Delay
*功能说明: 软件IIC延时函数
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_Delay(uint8_t deviceNum)
{


    if(deviceNum == 1)
    {
        delay_us(SOFT_I2C1_DELAY);
    }
    else if(deviceNum == 2)
    {

    }


}

/**********************************************************************************************************
*函 数 名: Soft_I2c_SCL_H
*功能说明: SCL引脚拉高
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_SCL_H(uint8_t deviceNum)
{
    if(deviceNum == 1)
    {
        IIC_SCL(1);
    }
    else if(deviceNum == 2)
    {

    }
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_SCL_L
*功能说明: SCL引脚拉低
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_SCL_L(uint8_t deviceNum)
{
    if(deviceNum == 1)
    {
        IIC_SCL(0);
    }
    else if(deviceNum == 2)
    {
    }
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_SDA_L
*功能说明: SDA引脚拉高
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_SDA_H(uint8_t deviceNum)
{
    if(deviceNum == 1)
    {
       IIC_SDA(1);
    }
    else if(deviceNum == 2)
    {

    }
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_SDA_L
*功能说明: SDA引脚拉低
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_SDA_L(uint8_t deviceNum)
{
    if(deviceNum == 1)
    {
        IIC_SDA(0);
    }
    else if(deviceNum == 2)
    {

    }
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_SCL_Read
*功能说明: 读取SCL引脚状态：0或1
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
uint8_t Soft_I2c_SCL_Read(uint8_t deviceNum)
{
    if(deviceNum == 1)
    {
        if(!READ_SDA_PIN)
            return 0;
        else
            return 1;
    }
    else if(deviceNum == 2)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_SDA_Read
*功能说明: 读取SDA引脚状态：0或1
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
uint8_t Soft_I2c_SDA_Read(uint8_t deviceNum)
{
    if(deviceNum == 1)
    {
        if(!(READ_SDA_PIN))
            return 0;
        else
            return 1;
    }
    else if(deviceNum == 2)
    {
        return 0;
    }
    else
    {
        return 0;
    }
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_Start
*功能说明: 发送I2C总线启动信号
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
bool Soft_I2c_Start(uint8_t deviceNum)
{
    SDA_MODE_OUT();    
    Soft_I2c_SDA_H(deviceNum);
    Soft_I2c_SCL_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
    SDA_MODE_IN();
    if(!Soft_I2c_SDA_Read(deviceNum))
    {
        //复位I2C总线
        Soft_I2c_SCL_L(deviceNum);
        return 0;
    }
    Soft_I2c_SDA_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    if(Soft_I2c_SDA_Read(deviceNum))
        return 0;
    Soft_I2c_SDA_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    return 1;
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_Stop
*功能说明: 发送I2C总线停止信号
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_Stop(uint8_t deviceNum)
{
    SDA_MODE_OUT();
    Soft_I2c_SCL_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SDA_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SCL_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SDA_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_Ack
*功能说明: 发送I2C应答信号
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_Ack(uint8_t deviceNum)
{
    
    SDA_MODE_OUT();
    Soft_I2c_SCL_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SDA_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SCL_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SCL_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_NoAck
*功能说明: 发送I2C非应答信号
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_NoAck(uint8_t deviceNum)
{
    SDA_MODE_OUT();
    Soft_I2c_SCL_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SDA_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SCL_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SCL_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_WaitAck
*功能说明: 等待从机应答信号
*形    参: 设备号
*返 回 值: 应答状态——1表示有应答
**********************************************************************************************************/
bool Soft_I2c_WaitAck(uint8_t deviceNum)
{
   SDA_MODE_OUT();   
    Soft_I2c_SCL_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SDA_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
    Soft_I2c_SCL_H(deviceNum);
    Soft_I2c_Delay(deviceNum);
    SDA_MODE_IN();
    if(Soft_I2c_SDA_Read(deviceNum))
    {
        Soft_I2c_SCL_L(deviceNum);
        Soft_I2c_Delay(deviceNum);
        return 0;
    }
    Soft_I2c_SCL_L(deviceNum);
    Soft_I2c_Delay(deviceNum);
    return 1;
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_SendByte
*功能说明: 发送一个字节
*形    参: 设备号
*返 回 值: 无
**********************************************************************************************************/
void Soft_I2c_SendByte(uint8_t deviceNum, uint8_t SendByte)
{
    
SDA_MODE_OUT();
    uint8_t i=8;
    while(i--)
    {
        Soft_I2c_SCL_L(deviceNum);
        Soft_I2c_Delay(deviceNum);
        if(SendByte&0x80)
            Soft_I2c_SDA_H(deviceNum);
        else
            Soft_I2c_SDA_L(deviceNum);
        SendByte<<=1;
        Soft_I2c_Delay(deviceNum);
        Soft_I2c_SCL_H(deviceNum);
        Soft_I2c_Delay(deviceNum);
    }
    Soft_I2c_SCL_L(deviceNum);
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_ReadByt
*功能说明: 读取一个字节
*形    参: 设备号
*返 回 值: 读取到的数据
**********************************************************************************************************/
uint8_t Soft_I2c_ReadByte(uint8_t deviceNum)
{
    uint8_t i=8;
    uint8_t ReceiveByte=0;
SDA_MODE_OUT();
    Soft_I2c_SDA_H(deviceNum);
SDA_MODE_IN();
    while(i--)
    {
        ReceiveByte<<=1;
        Soft_I2c_SCL_L(deviceNum);
        Soft_I2c_Delay(deviceNum);
        Soft_I2c_SCL_H(deviceNum);
        Soft_I2c_Delay(deviceNum);
        if(Soft_I2c_SDA_Read(deviceNum))
        {
            ReceiveByte|=0x01;
        }
    }
    Soft_I2c_SCL_L(deviceNum);
    return ReceiveByte;
}

/**********************************************************************************************************
*函 数 名: Soft_I2c_Single_Write
*功能说明: 单个寄存器写入
*形    参: 设备号 从机地址 寄存器地址 写入数据
*返 回 值: 写入状态
**********************************************************************************************************/
bool Soft_I2c_Single_Write(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)
{
    if(!Soft_I2c_Start(deviceNum))
        return false;
    Soft_I2c_SendByte(deviceNum, SlaveAddress);
    if(!Soft_I2c_WaitAck(deviceNum))
    {
        Soft_I2c_Stop(deviceNum);
        return false;
    }
    Soft_I2c_SendByte(deviceNum, REG_Address);
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_SendByte(deviceNum, REG_data);
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_Stop(deviceNum);
    return true;
}


/**********************************************************************************************************
*函 数 名: Soft_I2c_Multi_Write
*功能说明: 多个寄存器写入
*形    参: 设备号 从机地址 寄存器地址 写入数据
*返 回 值: 写入状态
**********************************************************************************************************/

void Soft_I2c_Multi_Write(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t WriteAddr, uint8_t lsb_data, uint8_t msb_data)
{
    
    Soft_I2c_Start(deviceNum);
    Soft_I2c_SendByte(deviceNum, SlaveAddress);//发送设备地址0xA0 + 写
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_SendByte(deviceNum, WriteAddr); //发送低地址
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_SendByte(deviceNum,lsb_data);
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_SendByte(deviceNum, msb_data);
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_Stop(deviceNum);

    softDelayUs(10);


}

//bool Soft_I2c_Multi_Write(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t REG_Address,uint8_t *REG_data,uint8_t len)
//{
//    if(!Soft_I2c_Start(deviceNum))
//        return false;
//    Soft_I2c_SendByte(deviceNum, SlaveAddress);
//    if(!Soft_I2c_WaitAck(deviceNum))
//    {
//        Soft_I2c_Stop(deviceNum);
//        return false;
//    }
//    Soft_I2c_SendByte(deviceNum, REG_Address);
//    Soft_I2c_WaitAck(deviceNum);
//    for(int i=0; i<len; i++)
//    {
//        Soft_I2c_SendByte(deviceNum, REG_data[i]);
//        Soft_I2c_WaitAck(deviceNum);
//    }

//    Soft_I2c_Stop(deviceNum);
//    return true;
//}

/**********************************************************************************************************
*函 数 名: Soft_I2c_Single_Read
*功能说明: 单个寄存器读取
*形    参: 设备号 从机地址 寄存器地址
*返 回 值: 读出数据
**********************************************************************************************************/
uint8_t Soft_I2C_Single_Read(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t REG_Address)
{
    uint8_t REG_data;
    if(!Soft_I2c_Start(deviceNum))
        return false;
    Soft_I2c_SendByte(deviceNum, SlaveAddress);
    if(!Soft_I2c_WaitAck(deviceNum))
    {
        Soft_I2c_Stop(deviceNum);
        return false;
    }
    Soft_I2c_SendByte(deviceNum, (uint8_t)REG_Address);
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_Start(deviceNum);
    Soft_I2c_SendByte(deviceNum, SlaveAddress+1);
    Soft_I2c_WaitAck(deviceNum);

    REG_data = Soft_I2c_ReadByte(deviceNum);
    Soft_I2c_NoAck(deviceNum);
    Soft_I2c_Stop(deviceNum);
    return REG_data;
}

/**********************************************************************************************************
*函 数 名: Soft_I2C_Multi_Read
*功能说明: 多个寄存器连续读取
*形    参: 设备号 从机地址 寄存器地址 读出缓冲区指针 读出长度
*返 回 值: 读取状态
**********************************************************************************************************/
bool Soft_I2C_Multi_Read(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t REG_Address,uint8_t * ptChar,uint8_t size)
{
    uint8_t i;

    if(size < 1)
        return false;
    if(!Soft_I2c_Start(deviceNum))
        return false;
    Soft_I2c_SendByte(deviceNum, SlaveAddress);
    if(!Soft_I2c_WaitAck(deviceNum))
    {
        Soft_I2c_Stop(deviceNum);
        return false;
    }
    Soft_I2c_SendByte(deviceNum, REG_Address);
    if(!Soft_I2c_WaitAck(deviceNum))
    {
        Soft_I2c_Stop(deviceNum);
        return false;
    }

    Soft_I2c_Start(deviceNum);
    Soft_I2c_SendByte(deviceNum, SlaveAddress+1);
    if(!Soft_I2c_WaitAck(deviceNum))
    {
        Soft_I2c_Stop(deviceNum);
        return false;
    }

    for(i=1; i<size; i++)
    {
        *ptChar++ = Soft_I2c_ReadByte(deviceNum);
        Soft_I2c_Ack(deviceNum);
    }
    *ptChar++ = Soft_I2c_ReadByte(deviceNum);
    Soft_I2c_NoAck(deviceNum);
    Soft_I2c_Stop(deviceNum);
    return true;
}


