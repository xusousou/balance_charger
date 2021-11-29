#include "rgbled.h"

#ifdef  WS2812B

uint8_t DataRGB[24];
#define Data0Time 0xC0
#define Data1Time 0xFC
#define RESTime   0x00

void rgbInit()
{
    rcu_periph_clock_enable(RGB_CLK);
    rcu_periph_clock_enable(RCU_SPI0); 
    rcu_periph_clock_enable(RCU_DMA);

    gpio_af_set(RGB_PORT,GPIO_AF_0,RGB_PIN);
    gpio_mode_set(RGB_PORT,GPIO_MODE_AF,GPIO_PUPD_NONE,RGB_PIN);
    gpio_output_options_set(RGB_PORT,GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ,RGB_PIN);
    gpio_bit_reset(RGB_PORT,RGB_PIN);   

    dma_parameter_struct  dma_init_struct;
    dma_struct_para_init(&dma_init_struct);

    /* SPI0 transmit dma config */
    dma_deinit(DMA_CH2);   
    dma_init_struct.periph_addr = (uint32_t)&SPI_DATA(SPI0);//外设基地址
    dma_init_struct.memory_addr = (uint32_t)DataRGB;//内存基地址
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPHERAL;   //数据传输方向：内存到外设
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;//外设数据宽度8位
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;   //内存数据宽度8位
    dma_init_struct.priority = DMA_PRIORITY_HIGH;           //DMA通道传输软件优先级
    dma_init_struct.number = 24;                     //DMA通道数据传输数量
    dma_init_struct.periph_inc =  DMA_PERIPH_INCREASE_DISABLE;//外设地址生成算法模式使能
    dma_init_struct.memory_inc =  DMA_MEMORY_INCREASE_ENABLE;//存储器地址生成算法模式失能
    dma_init(DMA_CH2, &dma_init_struct);                     //初始化DMA通道2	
    dma_channel_enable(DMA_CH2);
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);

	spi_parameter_struct  spi_init_struct;
    spi_struct_para_init(&spi_init_struct);

    /* SPI0 parameter config */
    spi_i2s_deinit(SPI0);
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_8;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI0, &spi_init_struct);
    spi_fifo_access_size_config(SPI0, SPI_BYTE_ACCESS);
    spi_enable(SPI0);

}

void SPI_DMA_WriteReadByte(void)
{	
    dma_channel_disable(DMA_CH2);
    dma_transfer_number_config(DMA_CH2,24);
    dma_channel_enable(DMA_CH2);
    spi_dma_enable(SPI0, SPI_DMA_TRANSMIT);
    while( dma_flag_get( DMA_CH2, DMA_FLAG_FTF ) == RESET);
    dma_flag_clear( DMA_CH2, DMA_FLAG_FTF ) ;
    spi_dma_disable(SPI0, SPI_DMA_TRANSMIT);
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
    SPI_DMA_WriteReadByte();
}

void LED_Control(uint32_t color){
    SetLed(color);
}

/**************************颜色渐变函数***************************/
uint8_t Red0, Green0, Blue0; // 起始三原色
uint8_t Red1, Green1, Blue1; // 结果三原色
int  RedMinus, GreenMinus, BlueMinus; // 颜色差（color1 - color0）
float  RedStep, GreenStep, BlueStep; // 各色步进值
uint8_t NStep; // 需要几步
unsigned long color; // 结果色
uint8_t Vstep,Tstep ;

uint8_t abs0(int num)//求绝对值
{
    if(num>0) return num;
    num = -num;
    return (unsigned char) num;
}

uint8_t Triangular(int num)//三角波
{
    static  int x;
    float k;
    x++;
    if(x<num){
        k=1;
        Tstep=x;
    }else if(x>=num && x<2*num){
        k=-1;
        Tstep=2*num-x;
    }else if(x>= 2*num || x < 0){
        x=0; 
        k=0;
    }else if(Tstep<0 || Tstep>num ){
        x=0;
        Tstep=0;
    }
    return Tstep;
}

uint8_t Breathe()//三角波
{
    static  uint8_t dir,highval;
    if(dir==1)highval--; //占空比逐渐减少，小灯逐渐变亮
    if(highval==0)dir=0;
   
    if(dir==0)highval++; //占空比逐渐增加，小灯逐渐变暗
    if(highval>=255)dir=1;

    Tstep=highval;
    return Tstep;
}

void chargerToColor(unsigned long color0, unsigned long color1, float bat, uint8_t cell)
{
    Color_decomposition(color0,color1);
 	switch (cell) 
    {
        case 0: 
            Triangular(NStep); 
            LED_Control(ColorToColor(Tstep));    
        break;
        case 2: 
            if(bat <= 7){
                Vstep = 1;
            }else if(bat >7 && bat <= 8.5 ){
                Vstep = NStep - (8.5-bat)/1.5*NStep;
            }else  Vstep = 255; 
            ColorToColor(Vstep);  
            Color_decomposition(0,color);
            Breathe(); 
            LED_Control(ColorToColor(Tstep)); 
        break;
        case 3: 
            if(bat <= 10.5){
                Vstep = 1;
            }else if(bat > 10.5 && bat <= 13){
                Vstep = NStep - (13-bat)/2.5*NStep;
            }else  Vstep = 255;   
            ColorToColor(Vstep);  
            Color_decomposition(0,color);
            Breathe(); 
            LED_Control(ColorToColor(Tstep));       
        break;
        case 4: 
            if(bat <= 14){
                Vstep = 1;
            }else if(bat > 14 && bat <= 17){
                Vstep = NStep - (17-bat)/3*NStep;
            }else  Vstep = 255;      
            ColorToColor(Vstep);  
            Color_decomposition(0,color);
            Breathe(); 
            LED_Control(ColorToColor(Tstep));   
        break;
        default:
        break;
    }
}

unsigned long  ColorToColor(uint8_t i)
{
    Red1 = Red0 + (int)(RedStep * i);
    Green1 = Green0 + (int)(GreenStep * i);
    Blue1 = Blue0 + (int)(BlueStep * i);

    color = Green1<<16 | Red1<<8 | Blue1; // 合成绿红蓝
    return color;
}

void Color_decomposition(unsigned long color0, unsigned long color1)
{
    // 绿 红 蓝 三原色分解
    Red0 = color0>>8;
    Green0 = color0>>16;
    Blue0 = color0;
    Red1 = color1>>8;
    Green1 = color1>>16;
    Blue1 = color1;
    // 绿 红 蓝 三原色分解  若输入的颜色非上述三种
    Red0 =( color0&0x00FF00)>>8;
    Green0 = (color0&0xFF0000)>>16;
    Blue0 = color0;
    Red1 = (color1&0x00FF00)>>8;
    Green1 = (color1&0xFF0000)>>16;
    Blue1 = color1;

    // 计算需要多少步（取差值的最大值）
    RedMinus = Red1 - Red0;
    GreenMinus = Green1 - Green0;
    BlueMinus = Blue1 - Blue0;

    //判断两个绝对值大小。。a>b为真，则=a；否则=b
    NStep = ( abs0(RedMinus) > abs0(GreenMinus) ) ? abs0(RedMinus):abs0(GreenMinus);
    NStep = ( NStep > abs0(BlueMinus) ) ? NStep:abs0(BlueMinus);

    // 计算出各色步进值
    RedStep = (float)RedMinus / NStep;
    GreenStep = (float)GreenMinus / NStep;
    BlueStep = (float)BlueMinus / NStep;
}

uint16_t SE = 0;
uint8_t bianhuan =1;
void Colorful_gradient()
{
    SE++;
	switch (bianhuan) 
    {
        case 1: 
            Color_decomposition(red,green);
            LED_Control(ColorToColor(SE));  
            if(ColorToColor(SE) == green){
                bianhuan++;
            }   
        break;
        case 2: 
            Color_decomposition(green,blue);
            LED_Control(ColorToColor(SE));  
            if(ColorToColor(SE) == blue){
                bianhuan++;
            }    
        break;
        case 3: 
            Color_decomposition(blue,red);
            LED_Control(ColorToColor(SE));  
            if(ColorToColor(SE) == red){
                bianhuan++;
            }          
        break;
        default:
        break;
    }
    if(bianhuan > 3) {
        SE = 0;
        bianhuan = 1;
    }
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

