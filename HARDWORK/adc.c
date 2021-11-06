/**
  ******************************************************************************
  *	文件: adc.h
  * 描述: 
  ******************************************************************************
  * @attention
  ******************************************************************************
**/

#include "adc.h"

extern float valueBAT,value1S,value2S,value3S,value4S;

#ifdef adc_1
struct Adc adc_values;
uint16_t ADC_Value[100];
float adc_data[5];
float vrefint, temperature;


void adc_init(void)
{
    /* GPIO 初始化 */
    rcu_periph_clock_enable(RCU_GPIOA);   
    gpio_mode_set          (CELL_1S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_1S_VOL_PIN); 
	gpio_mode_set          (CELL_2S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_2S_VOL_PIN); 
	gpio_mode_set          (CELL_BAT_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_BAT_VOL_PIN); 

    dma_parameter_struct            dma_init_struct;           // DMA 初始化参数结构体。
	rcu_periph_clock_enable        (RCU_DMA);                  // 使能外设时钟。
	dma_deinit                     (DMA_CH0);                  // 复位DMA通道x的所有寄存器。
	dma_init_struct.periph_addr  = (uint32_t)&(ADC_RDATA);     // 设置DMA的外设地址，也是DMA数据传输的源地址。ADC单次采样转换结束后，会将数据存入ADC_RDATA地址中
	dma_init_struct.memory_addr  = (uint32_t)ADC_Value;        // 设置DMA存储器地址，也是DMAD数据传输的目标地址。
	dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;   // 设置DMA的数据传输方向。
	dma_init_struct.number       = 5;                          // 设置DMA一个周期要传输的数据个数，单位由下方设置。ADC使能了两个采样通道，所以DMA每个周期需要传输两个数据。
	dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;// 设置DMA数据传输的源地址自增算法失能。ADC每次转换后数据存放的缓存地址不变，所以这里将之失能。
	dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE; // 设置DMA数据传输目标地址自增算法使能，因为DMA每个周期需要传送两个数据，所以在一个周期内，目标地址要自增。
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT; // 设置DMA从外设中取出的数据位数。因为ADC转化后的数值是16bit，所以这里设置为1字。
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;     // 设置DMA存入存储器中的数据位数。
	dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;    // 设置DMA数据传输高优先级。
	dma_init                       (DMA_CH0, &dma_init_struct);// 将结构体送回，初始化DMA通道x
	dma_circulation_enable         (DMA_CH0);                  // DMA循环模式使能。这样DMA在传输两个数据结束后，存储器地址复位，自动开启下一轮传输。
	dma_memory_to_memory_disable   (DMA_CH0);                  // 存储器到存储器DMA传输使能。因为这里用不上，所以失能。
	dma_channel_enable             (DMA_CH0);	               // DMA通道x传输使能
	
	/* ADC 配置 */
    RCU_CTL1 |= RCU_CTL1_IRC28MEN;
    rcu_osci_stab_wait(RCU_IRC28M);
	rcu_adc_clock_config              (RCU_ADCCK_IRC28M_DIV2);                              // 设置ADC的采样时钟分频。

	rcu_periph_clock_enable           (RCU_ADC);                                       // 使能外设时钟。
	adc_deinit                        ();                                              // 复位ADC外设。
	adc_special_function_config       (ADC_SCAN_MODE, ENABLE);                         // 配置ADC扫描转换模式。
	adc_special_function_config       (ADC_CONTINUOUS_MODE, ENABLE);                   // 配置ADC连续转换模式。
    adc_resolution_config(ADC_RESOLUTION_12B);
    adc_oversample_mode_config( ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_8B, ADC_OVERSAMPLING_RATIO_MUL256);
	adc_data_alignment_config         (ADC_DATAALIGN_RIGHT);                           // 配置ADC数据对齐方式是右对齐。
	adc_channel_length_config         (ADC_REGULAR_CHANNEL, 5);                        // 配置规则通道组或注入通道组的长度。因为要用到两个采样通道，所以是2.

    adc_tempsensor_vrefint_enable();
	adc_regular_channel_config        (0, ADC_CHANNEL_0, ADC_SAMPLETIME_239POINT5);     // 配置ADC规则通道组。rank代表扫描顺序，channel要根据ADC通道与GPIO的映射关系表确定。
	adc_regular_channel_config        (1, ADC_CHANNEL_1, ADC_SAMPLETIME_239POINT5);	
	adc_regular_channel_config        (2, ADC_CHANNEL_2, ADC_SAMPLETIME_239POINT5);	
	adc_regular_channel_config        (3, ADC_CHANNEL_16, ADC_SAMPLETIME_239POINT5);	
	adc_regular_channel_config        (4, ADC_CHANNEL_17, ADC_SAMPLETIME_239POINT5);	
    adc_oversample_mode_enable ();
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE); // 配置ADC外部触发源是软件触发。
    adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);

	adc_enable                        ();                                              // 使能ADC外设
	adc_calibration_enable            ();                                              // ADC校准复位
	adc_dma_mode_enable               ();                                              // ADCx DMA请求使能
	adc_software_trigger_enable       (ADC_REGULAR_CHANNEL);                           // ADC软件触发使能。

}

uint8_t Get_Adc_Val( uint32_t *bat,uint32_t *s ,uint32_t *ss)
{       
    *s   =0;
	*ss  =0;
	*bat =0;
    temperature=0;
    vrefint = 0;
	for(uint8_t i=0;i<5;)
	{
		*s   += ADC_Value[i++];   
		*ss  += ADC_Value[i++];
        *bat += ADC_Value[i++];  
        temperature += ADC_Value[i++];
        vrefint += ADC_Value[i++];
    } 
    *s= *s;
    *ss= *ss;
    *bat = *bat;
    temperature =temperature;
    vrefint = vrefint;
    return 0;
}

uint8_t get_low_filter(uint32_t  *BAT,uint32_t *vol1, uint32_t *vol2)
{
    float dPower = 0.5; 
    static uint16_t  temperaturelastnum ,Vrefnum = 0,BATnum0 = 0,vol1num1 = 0,vol2num2 = 0;   
    static uint16_t  temperaturenum = 0,VrefLastnum,Lastnum0,Lastnum1,Lastnum2; //
    
    temperaturenum = ((temperature) * dPower) + (1-dPower) * temperaturenum;
    Vrefnum = ( vrefint * dPower ) + ( 1 - dPower ) * VrefLastnum; //

    BATnum0 = ( *BAT * dPower ) + ( 1 - dPower ) * Lastnum0; //
    vol1num1 = ( *vol1 * dPower ) + ( 1 - dPower ) * Lastnum1; //
    vol2num2 = ( *vol2 * dPower ) + ( 1 - dPower ) * Lastnum2; // 

    temperaturelastnum = temperaturenum;
    VrefLastnum = Vrefnum ;  


    Lastnum0 = BATnum0;                                     //
    Lastnum1 = vol1num1; 
    Lastnum2 = vol2num2;
    adc_values.vrefint=  4096 *1.2/VrefLastnum;

    adc_data[0] = Lastnum0;
    adc_data[1] = Lastnum1;
    adc_data[2] = Lastnum2;
    adc_data[3] = temperaturelastnum;
    adc_data[4] = adc_values.vrefint;
    return 0;
}

uint32_t Get_Cell_Voltage(uint8_t cell_number) {
	if (cell_number > 4) {
		return UINT32_MAX;
	}
	return adc_values.cell_voltage[cell_number] * BATTERY_ADC_MULTIPLIER;
}


float Get_MCU_Temperature(){
    adc_values.temperature = ((1.450- (adc_data[3] *3.3/4096))/(4.3/1000))+25;
	return adc_values.temperature;
}
#else

struct Adc adc_values;
uint16_t ADC_Value[70];
uint32_t temperature, vrefintnum;


/****
    * @函数名     adc_init 
    * @描述       adc初始化dma传输      
    * @传入参数   延时时间
    * @传出参数   无
    * @返回值     无
    */
void adc_init(void)
{
    /* GPIO 初始化 */
    rcu_periph_clock_enable(RCU_GPIOA);
    gpio_mode_set          (CELL_BAT_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_BAT_VOL_PIN);    
    gpio_mode_set          (CELL_1S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_1S_VOL_PIN); 
	gpio_mode_set          (CELL_2S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_2S_VOL_PIN); 
    gpio_mode_set          (CELL_3S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_1S_VOL_PIN); 
	gpio_mode_set          (CELL_4S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_2S_VOL_PIN); 

    dma_parameter_struct            dma_init_struct;                                    // DMA 初始化参数结构体。
	rcu_periph_clock_enable        (RCU_DMA);                                           // 使能外设时钟。
	dma_deinit                     (DMA_CH0);                                           // 复位DMA通道x的所有寄存器。
	dma_init_struct.periph_addr  = (uint32_t)&(ADC_RDATA);                              // 设置DMA的外设地址，也是DMA数据传输的源地址。ADC单次采样转换结束后，会将数据存入ADC_RDATA地址中
	dma_init_struct.memory_addr  = (uint32_t)ADC_Value;                                 // 设置DMA存储器地址，也是DMAD数据传输的目标地址。
	dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;                            // 设置DMA的数据传输方向。
	dma_init_struct.number       = 70;                                                  // 设置DMA一个周期要传输的数据个数，单位由下方设置。ADC使能了两个采样通道，所以DMA每个周期需要传输两个数据。
	dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;                         // 设置DMA数据传输的源地址自增算法失能。ADC每次转换后数据存放的缓存地址不变，所以这里将之失能。
	dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;                          // 设置DMA数据传输目标地址自增算法使能，因为DMA每个周期需要传送两个数据，所以在一个周期内，目标地址要自增。
	dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;                          // 设置DMA从外设中取出的数据位数。因为ADC转化后的数值是16bit，所以这里设置为1字。
	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_16BIT;                              // 设置DMA存入存储器中的数据位数。
	dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;                             // 设置DMA数据传输高优先级。
	dma_init                       (DMA_CH0, &dma_init_struct);                         // 将结构体送回，初始化DMA通道x
	dma_circulation_enable         (DMA_CH0);                                           // DMA循环模式使能。这样DMA在传输两个数据结束后，存储器地址复位，自动开启下一轮传输。
	dma_memory_to_memory_disable   (DMA_CH0);                                           // 存储器到存储器DMA传输使能。因为这里用不上，所以失能。
	dma_channel_enable             (DMA_CH0);	                                        // DMA通道x传输使能

	/* ADC 配置 */
    RCU_CTL1 |= RCU_CTL1_IRC28MEN;
    rcu_osci_stab_wait(RCU_IRC28M);
	rcu_adc_clock_config              (RCU_ADCCK_IRC28M);                               // 设置ADC的采样时钟分频。
	rcu_periph_clock_enable           (RCU_ADC); 

	adc_special_function_config       (ADC_SCAN_MODE, ENABLE);                          // 配置ADC扫描转换模式。
	adc_special_function_config       (ADC_CONTINUOUS_MODE, ENABLE);                    // 配置ADC连续转换模式。
    adc_resolution_config(ADC_RESOLUTION_12B);
    adc_oversample_mode_config( ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_8B, ADC_OVERSAMPLING_RATIO_MUL256);
	adc_data_alignment_config         (ADC_DATAALIGN_RIGHT);                            // 配置ADC数据对齐方式是右对齐。
	adc_channel_length_config         (ADC_REGULAR_CHANNEL, 7U);                        // 配置规则通道组或注入通道组的长度。因为要用到两个采样通道，所以是2.

    adc_tempsensor_vrefint_enable();	
	adc_regular_channel_config        (0, ADC_CHANNEL_2, ADC_SAMPLETIME_239POINT5);     // 配置ADC规则通道组。rank代表扫描顺序，channel要根据ADC通道与GPIO的映射关系表确定。
	adc_regular_channel_config        (1, ADC_CHANNEL_3, ADC_SAMPLETIME_239POINT5);	
	adc_regular_channel_config        (2, ADC_CHANNEL_4, ADC_SAMPLETIME_239POINT5);
	adc_regular_channel_config        (3, ADC_CHANNEL_5, ADC_SAMPLETIME_239POINT5);   
	adc_regular_channel_config        (4, ADC_CHANNEL_6, ADC_SAMPLETIME_239POINT5);  
	adc_regular_channel_config        (5, ADC_CHANNEL_16, ADC_SAMPLETIME_239POINT5);	
	adc_regular_channel_config        (6, ADC_CHANNEL_17, ADC_SAMPLETIME_239POINT5);	
    adc_oversample_mode_enable ();
	adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE); // 配置ADC外部触发源是软件触发。
    adc_external_trigger_config(ADC_REGULAR_CHANNEL,ENABLE);

	adc_enable                        ();                                              // 使能ADC外设
    delay_1ms(10);
    delay_1ms(10);
    delay_1ms(500);                                                                    // 延时非常重要
	adc_calibration_enable            ();                                              // ADC校准复位
    delay_1ms(10);
	adc_dma_mode_enable               ();                                              // ADCx DMA请求使能
	adc_software_trigger_enable       (ADC_REGULAR_CHANNEL);                           // ADC软件触发使能。
}

/****
    * @函数名     Get_Adc_Val 
    * @描述       求平均值      
    * @传入参数   adc采样值
    * @传出参数   无
    * @返回值     无
    */
void Get_Adc_Val( uint32_t *bat, uint32_t *s, uint32_t *ss, uint32_t *sss, uint32_t *ssss)
{  
	*bat =0;
    *s   =0;
	*ss  =0;
    *sss   =0;
	*ssss  =0;
    temperature=0;
    vrefintnum = 0;

	for(uint8_t i=0;i<70;)
	{
        *bat += ADC_Value[i++];  
		*s   += ADC_Value[i++];   
		*ss  += ADC_Value[i++];
		*sss   += ADC_Value[i++];   
		*ssss  += ADC_Value[i++];
        temperature += ADC_Value[i++];
        vrefintnum += ADC_Value[i++];
    } 

    *bat = *bat/10;
    *s= *s/10;
    *ss= *ss/10;
    *sss= *sss/10;
    *ssss= *ssss/10;
    temperature =temperature/10;
    vrefintnum = vrefintnum/10;
}

/****
    * @函数名     get_low_filter 
    * @描述       低通滤波函数      
    * @传入参数   adc采样平均值
    * @传出参数   无
    * @返回值     无
    */
void get_low_filter(uint32_t *bat, uint32_t *s, uint32_t *ss, uint32_t *sss, uint32_t *ssss)
{
    float dPower = 0.1; 
    static uint32_t  temperaturelastnum ,Vrefnum = 0,BATnum0 = 0,vol1num1 = 0,vol2num2 = 0,vol3num3 = 0,vol4num4 = 0;   
    static uint32_t  temperaturenum = 0,VrefLastnum,Lastnum0,Lastnum1,Lastnum2,Lastnum3,Lastnum4; //
    
    BATnum0  = ( *bat * dPower ) + ( 1 - dPower ) * Lastnum0; 
    vol1num1 = ( *s * dPower ) + ( 1 - dPower ) * Lastnum1; 
    vol2num2 = ( *ss * dPower ) + ( 1 - dPower ) * Lastnum2; 
    vol3num3 = ( *sss * dPower ) + ( 1 - dPower ) * Lastnum3; 
    vol4num4 = ( *ssss * dPower ) + ( 1 - dPower ) * Lastnum4; 
    Vrefnum      = ( vrefintnum * dPower ) + ( 1 - dPower ) * VrefLastnum; 


    Lastnum0 = BATnum0;    
    Lastnum1 = vol1num1; 
    Lastnum2 = vol2num2;
    Lastnum3 = vol3num3; 
    Lastnum4 = vol4num4;
    VrefLastnum  = Vrefnum;
    
    *bat  = Lastnum0;
    *s    = vol1num1;
    *ss   = vol2num2;
    *sss  = vol3num3;
    *ssss = vol4num4;
    vrefintnum   = VrefLastnum;
}

/****
    * @函数名     Get_Cell_Voltage 
    * @描述       获取电池或某片电芯电压      
    * @传入参数   电池电芯片数
    * @传出参数   无
    * @返回值     所需要的某片电芯电压或电池电压
    */
uint32_t Get_Cell_Voltage(uint8_t cell_number) {
	if (cell_number > 4) {
		return UINT32_MAX;
	}
	return adc_values.cell_voltage[cell_number] * BATTERY_ADC_MULTIPLIER;
}

/****
    * @函数名     Get_MCU_Temperature 
    * @描述       获取MCU温度数据     
    * @传入参数   无
    * @传出参数   无
    * @返回值     MCU核心温度 
    */
float Get_MCU_Temperature(){
	return adc_values.temperature;
}

#endif

