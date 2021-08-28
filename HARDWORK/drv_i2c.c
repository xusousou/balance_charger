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
*�� �� ��: Soft_I2c_Delay
*����˵��: ���IIC��ʱ����
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_SCL_H
*����˵��: SCL��������
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_SCL_L
*����˵��: SCL��������
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_SDA_L
*����˵��: SDA��������
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_SDA_L
*����˵��: SDA��������
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_SCL_Read
*����˵��: ��ȡSCL����״̬��0��1
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_SDA_Read
*����˵��: ��ȡSDA����״̬��0��1
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_Start
*����˵��: ����I2C���������ź�
*��    ��: �豸��
*�� �� ֵ: ��
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
        //��λI2C����
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
*�� �� ��: Soft_I2c_Stop
*����˵��: ����I2C����ֹͣ�ź�
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_Ack
*����˵��: ����I2CӦ���ź�
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_NoAck
*����˵��: ����I2C��Ӧ���ź�
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_WaitAck
*����˵��: �ȴ��ӻ�Ӧ���ź�
*��    ��: �豸��
*�� �� ֵ: Ӧ��״̬����1��ʾ��Ӧ��
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
*�� �� ��: Soft_I2c_SendByte
*����˵��: ����һ���ֽ�
*��    ��: �豸��
*�� �� ֵ: ��
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
*�� �� ��: Soft_I2c_ReadByt
*����˵��: ��ȡһ���ֽ�
*��    ��: �豸��
*�� �� ֵ: ��ȡ��������
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
*�� �� ��: Soft_I2c_Single_Write
*����˵��: �����Ĵ���д��
*��    ��: �豸�� �ӻ���ַ �Ĵ�����ַ д������
*�� �� ֵ: д��״̬
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
*�� �� ��: Soft_I2c_Multi_Write
*����˵��: ����Ĵ���д��
*��    ��: �豸�� �ӻ���ַ �Ĵ�����ַ д������
*�� �� ֵ: д��״̬
**********************************************************************************************************/

void Soft_I2c_Multi_Write(uint8_t deviceNum, uint8_t SlaveAddress,uint8_t WriteAddr, uint8_t lsb_data, uint8_t msb_data)
{
    
    Soft_I2c_Start(deviceNum);
    Soft_I2c_SendByte(deviceNum, SlaveAddress);//�����豸��ַ0xA0 + д
    Soft_I2c_WaitAck(deviceNum);
    Soft_I2c_SendByte(deviceNum, WriteAddr); //���͵͵�ַ
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
*�� �� ��: Soft_I2c_Single_Read
*����˵��: �����Ĵ�����ȡ
*��    ��: �豸�� �ӻ���ַ �Ĵ�����ַ
*�� �� ֵ: ��������
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
*�� �� ��: Soft_I2C_Multi_Read
*����˵��: ����Ĵ���������ȡ
*��    ��: �豸�� �ӻ���ַ �Ĵ�����ַ ����������ָ�� ��������
*�� �� ֵ: ��ȡ״̬
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


