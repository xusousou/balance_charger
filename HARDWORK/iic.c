#include "iic.h"

//配置成OD模式不需要进行输入输出模式切换

#define SDA_IN()  {GPIO_CTL(GPIOB)&=0X0FFFFFFF;GPIO_CTL(GPIOB)|=(uint32_t)8<<28;}
#define SDA_OUT() {GPIO_CTL(GPIOB)&=0X0FFFFFFF;GPIO_CTL(GPIOB)|=(uint32_t)3<<28;}


//IIC起始信号
void i2c_start(void)
{
	SDA_OUT();//SDA输出模式
	IIC_SCL(1);
	IIC_SDA(1);
	delay_us(4);
	IIC_SDA(0);
	delay_us(4);
	IIC_SCL(0);
}

//IIC停止信号
void i2c_stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);
	delay_us(4);
	IIC_SCL(1);
	delay_us(4);
	IIC_SDA(1);
	delay_us(4);
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t i2c_wait_ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA(1);delay_us(1);	   
	IIC_SCL(1);delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			i2c_stop();
			return 1;
		}
	}
	IIC_SCL(0);//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void i2c_ack(void)
{
	SDA_OUT();
	delay_us(4);
	IIC_SCL(0);
	delay_us(4);
	IIC_SDA(0);
	delay_us(4);
	IIC_SCL(1);
	delay_us(4);
	IIC_SCL(0);
	delay_us(4);
}
//不产生ACK应答		    
void i2c_nack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us(2);
	IIC_SCL(1);
	delay_us(2);
	IIC_SCL(0);
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void i2c_send_byte(uint8_t byte)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL(0);//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if(byte&0x80) IIC_SDA(1);
		else IIC_SDA(0);

        byte<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL(1);
		delay_us(2); 
		IIC_SCL(0);	
		delay_us(2);
    }	 
} 

//读1个字节，ack=1时，发送ACK; ack=0，发送nACK   
uint8_t i2c_read_byte()
{
	unsigned char i,receive=0;

	IIC_SDA(1);
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL(0); 
        delay_us(2);
		IIC_SCL(1);
        receive<<=1;
        if(READ_SDA) receive|=1;   
		delay_us(1); 
    }
    IIC_SCL(0);					 
    return receive;
}


void i2c_init(void)
{
    rcu_periph_clock_enable(IIC_SCL_GPIO_CLK);
    rcu_periph_clock_enable(IIC_SDA_GPIO_CLK);

    gpio_mode_set(IIC_SCL_GPIO_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,IIC_SCL_PIN);
    gpio_mode_set(IIC_SDA_GPIO_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,IIC_SDA_PIN);

    gpio_output_options_set(IIC_SCL_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, IIC_SCL_PIN);
    gpio_output_options_set(IIC_SDA_GPIO_PORT, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, IIC_SDA_PIN);
	
	IIC_SCL(1);
	IIC_SDA(1);

}
