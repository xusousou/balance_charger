/**
  ******************************************************************************
  *	文件: husb238_iic.c
  * 描述: PD诱骗芯片IIC初始化
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#include "husb238_iic.h"


//IIC起始信号
void HUSB238_i2c_start(void)
{
	HUSB238_SDA_OUT();//SDA输出模式
	HUSB238_IIC_SDA(1);
	delay_us(4);

	HUSB238_IIC_SCL(1);
	HUSB238_IIC_SDA(0);
	delay_us(4);
	HUSB238_IIC_SCL(0);
}

//IIC停止信号
void HUSB238_i2c_stop(void)
{
	HUSB238_SDA_OUT();
	HUSB238_IIC_SCL(0);
	delay_us(1);

	HUSB238_IIC_SDA(0);
	delay_us(4);

	HUSB238_IIC_SCL(1);
	delay_us(1);

	HUSB238_IIC_SDA(1);
	delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t HUSB238_i2c_wait_ack(void)
{
	uint16_t ucErrTime=0;
	HUSB238_SDA_IN();      //SDA设置为输入  
	HUSB238_IIC_SDA(1);
    delay_us(1);	   
	HUSB238_IIC_SCL(1);
    delay_us(1);	

	while(HUSB238_READ_SDA == 1)
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			HUSB238_i2c_stop();
			return 1;
		}
	}

	HUSB238_IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void HUSB238_i2c_ack(void)
{
	HUSB238_SDA_OUT();
	delay_us(4);
	HUSB238_IIC_SCL(0);
	delay_us(4);
	HUSB238_IIC_SDA(0);
	delay_us(4);
	HUSB238_IIC_SCL(1);
	delay_us(4);
	HUSB238_IIC_SCL(0);
	delay_us(4);
}
//不产生ACK应答		    
void HUSB238_i2c_nack(void)
{
	HUSB238_IIC_SCL(0);
	HUSB238_SDA_OUT();
	HUSB238_IIC_SDA(1);
	delay_us(2);
	HUSB238_IIC_SCL(1);
	delay_us(2);
	HUSB238_IIC_SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void HUSB238_i2c_send_byte(uint8_t byte)
{                        
    uint8_t t;   
	HUSB238_SDA_OUT(); 	 
    delay_us(2);   
    HUSB238_IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {  
        HUSB238_IIC_SDA((byte & 0x80) >> 7);
        byte <<= 1;
        delay_us(2);
        HUSB238_IIC_SCL(1);
        delay_us(2);
        HUSB238_IIC_SCL(0);
//        delay_us(2);
    }
} 

//读1个字节，ack=1时，发送ACK; ack=0，发送nACK   
uint8_t HUSB238_i2c_read_byte()
{
	unsigned char i,receive=0;

	HUSB238_IIC_SDA(1);
    delay_us(2);
	HUSB238_SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        HUSB238_IIC_SCL(0); 
        delay_us(2);
		HUSB238_IIC_SCL(1);
        receive<<=1;
        if(HUSB238_READ_SDA) receive|=1;   
		delay_us(1); 
    }
    HUSB238_IIC_SCL(0);					 
    return receive;
}


void HUSB238_i2c_init(void)
{
    rcu_periph_clock_enable(HUSB238_IIC_SCL_CLK);
    rcu_periph_clock_enable(HUSB238_IIC_SDA_CLK);

    gpio_mode_set(HUSB238_IIC_SCL_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,HUSB238_IIC_SCL_PIN);
    gpio_mode_set(HUSB238_IIC_SDA_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,HUSB238_IIC_SDA_PIN);

    gpio_output_options_set(HUSB238_IIC_SCL_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, HUSB238_IIC_SCL_PIN);
    gpio_output_options_set(HUSB238_IIC_SDA_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, HUSB238_IIC_SDA_PIN);

	HUSB238_IIC_SCL(1);
	HUSB238_IIC_SDA(1);
}
