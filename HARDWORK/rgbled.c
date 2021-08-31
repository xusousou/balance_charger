#include "rgbled.h"

#ifdef  WS2812B

uint32_t DataRGB[24];
#define Data0Time 0xC0
#define Data1Time 0xFC
#define RESTime   0x00

void rgbInit()
{
    rcu_periph_clock_enable(RGB_CLK);
    rcu_periph_clock_enable(RCU_SPI0); 
//    rcu_periph_clock_enable(RCU_DMA);

    gpio_af_set(RGB_PORT,GPIO_AF_0,RGB_PIN);
    gpio_mode_set(RGB_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,RGB_PIN);
    gpio_output_options_set(RGB_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,RGB_PIN);
    gpio_bit_reset(RGB_PORT,RGB_PIN);   

//    dma_parameter_struct  dma_init_struct;
//    dma_struct_para_init(&dma_init_struct);

//    dma_deinit(DMA_CH2);   
//    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI0);//外设基地址
//    dma_init_struct.memory_addr = (uint32_t)DataRGB ;//内存基地址
//    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;   //数据传输方向：内存到外设
//    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;//外设数据宽度8位
//    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;   //内存数据宽度8位
//    dma_init_struct.priority = DMA_PRIORITY_HIGH;           //DMA通道传输软件优先级
//    dma_init_struct.number = 1;                     //DMA通道数据传输数量
//    dma_init_struct.periph_inc =  DMA_PERIPH_INCREASE_DISABLE;//外设地址生成算法模式使能
//    dma_init_struct.memory_inc =  DMA_MEMORY_INCREASE_ENABLE;//存储器地址生成算法模式失能
//    dma_init(DMA_CH2, &dma_init_struct);                     //初始化DMA通道2	
//    dma_channel_enable(DMA_CH2);

	spi_parameter_struct  spi_init_struct;
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_i2s_deinit(SPI0);
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);
    spi_enable(SPI0);
//    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    /* SPI0 transmit dma config */

}

void SetLed(uint32_t RgbData)
{
	uint8_t i=0,j=0,RGB_G=0,RGB_R=0,RGB_B=0;
	
    // separation  RGB
    RGB_G = RgbData>>16 & 0XFF;
    RGB_R = RgbData>>8  & 0XFF;
    RGB_B = RgbData     & 0XFF;
	//Map RGB data to SPI data
	for(i=0;i<8;i++)
    {
        (RGB_G & 0x80)? (DataRGB[i] = Data1Time):(DataRGB[i] = Data0Time);
        RGB_G =RGB_G<<1;
        (RGB_R & 0x80)? (DataRGB[i+8] = Data1Time):(DataRGB[i+8] = Data0Time);
        RGB_R =RGB_R<<1;
        (RGB_B & 0x80)? (DataRGB[i+16] = Data1Time):(DataRGB[i+16] = Data0Time);
        RGB_B =RGB_B<<1;
    }
	//send SPI data
	for(i=0;i<24;i++)
	{
		spi_i2s_data_transmit(SPI0,DataRGB[i]);
		while( spi_i2s_flag_get( SPI0, SPI_FLAG_TBE ) == RESET  && j<50)
		{
			j++;
		}
	}
	spi_i2s_data_transmit(SPI0,0x00);
}


void Charge_RGB_Control(float vol,uint8_t CELL)
{
    uint32_t vol_RG;
    uint8_t R_value, G_value, vol_min, vol_max;
    vol_max=(CELL*4.20)*100;
    vol_min=(CELL*4.20-2.55)*100;

    if(vol < MIN_2SBAT_VOLTAGE && CELL>1)
    {
        SetLed(0x00ff00);
    }else if(vol > MIN_2SBAT_VOLTAGE && CELL>1 ){
       R_value = (vol_max-(vol*100));
       G_value = ((vol*100)-vol_min)*((vol*100)/vol_max);
       vol_RG |= G_value<<16;
       vol_RG |= R_value<<8;
       vol_RG |= 0x00;
       SetLed(vol_RG);
    }else SetLed(0x000000);
}

void LED_Control(uint32_t color){
    SetLed(color);
}

#elif  Common_positive_RGB
void rgbInit()
{
    rcu_periph_clock_enable(R_CLK);
    rcu_periph_clock_enable(G_CLK);
    rcu_periph_clock_enable(B_CLK);
    
    gpio_deinit(G_PORT);
    gpio_deinit(R_PORT);
    gpio_deinit(B_PORT);    
    gpio_mode_set(R_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,R_PIN);
    gpio_mode_set(G_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,G_PIN);
    gpio_mode_set(B_PORT,GPIO_MODE_OUTPUT,GPIO_PUPD_NONE,B_PIN);
    gpio_output_options_set(G_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,G_PIN);
    gpio_output_options_set(R_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,R_PIN);    
    gpio_output_options_set(B_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,B_PIN);  
 
    gpio_bit_set(R_PORT,R_PIN);       
    gpio_bit_set(G_PORT,G_PIN); 
    gpio_bit_set(B_PORT,B_PIN);    
}


void LED_Control(uint32_t color)
{
    switch(color){
        case 0://鐏?
            gpio_bit_set(R_PORT,R_PIN);       
            gpio_bit_set(G_PORT,G_PIN); 
            gpio_bit_set(B_PORT,B_PIN); 
        break;        
        case 1://绾?
            gpio_bit_reset(R_PORT,R_PIN);       
            gpio_bit_set(G_PORT,G_PIN); 
            gpio_bit_set(B_PORT,B_PIN);
        break;
        case 2://姗?
            gpio_bit_reset(R_PORT,R_PIN);       
            gpio_bit_reset(G_PORT,G_PIN); 
            gpio_bit_set(B_PORT,B_PIN);
        break;
        case 3://榛?

        break;
        case 4://缁?
            gpio_bit_set(R_PORT,R_PIN);       
            gpio_bit_reset(G_PORT,G_PIN); 
            gpio_bit_set(B_PORT,B_PIN);
        break;
        case 5://闱?
            gpio_bit_set(R_PORT,R_PIN);       
            gpio_bit_reset(G_PORT,G_PIN); 
            gpio_bit_reset(B_PORT,B_PIN);
        break;
        case 6://钃?
            gpio_bit_set(R_PORT,R_PIN);       
            gpio_bit_set(G_PORT,G_PIN); 
            gpio_bit_reset(B_PORT,B_PIN);
        break;
        case 7://绱?
            gpio_bit_reset(R_PORT,R_PIN);       
            gpio_bit_set(G_PORT,G_PIN); 
            gpio_bit_reset(B_PORT,B_PIN);
        break;
        default: 
            gpio_bit_set(R_PORT,R_PIN);       
            gpio_bit_set(G_PORT,G_PIN); 
            gpio_bit_set(B_PORT,B_PIN);    
            break;
    }
}

#endif

