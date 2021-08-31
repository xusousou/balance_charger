#include "adc.h"


extern float valueBAT,value1S,value2S,value3S,value4S;

struct Adc adc_values;
uint16_t ADC_Value[5];
float adc_data[5];
float vrefint, temperature;

void adc_init(void)
{
    /* GPIO 閰嶇疆 */
    rcu_periph_clock_enable(RCU_GPIOA);    // 浣胯?澶?鏃墮挓銆?
    gpio_mode_set          (CELL_1S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_1S_VOL_PIN); // 璁劇疆GPIO妯〃紡錛?誕絀鴻?鍏?紝鏃??涓?媺銆?
	gpio_mode_set          (CELL_2S_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_2S_VOL_PIN); // 璁劇疆GPIO妯〃紡錛?誕絀鴻?鍏?紝鏃??涓?媺銆?
	gpio_mode_set          (CELL_BAT_VOL_PORT, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, CELL_BAT_VOL_PIN); // 璁劇疆GPIO妯〃紡錛?誕絀鴻?鍏?紝鏃??涓?媺銆?

    dma_parameter_struct            dma_init_struct;           // DMA 初始化参数结构体。
	rcu_periph_clock_enable        (RCU_DMA);                  // 使能外设时钟。
	dma_deinit                     (DMA_CH0);                  // 复位DMA通道x的所有寄存器。
	dma_init_struct.periph_addr  = (uint32_t)&(ADC_RDATA);     // 设置DMA的外设地址，也是DMA数据传输的源地址。ADC单次采样转换结束后，会将数据存入ADC_RDATA地址中
	dma_init_struct.memory_addr  = (uint32_t)ADC_Value;    // 设置DMA存储器地址，也是DMAD数据传输的目标地址。
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
	dma_channel_enable             (DMA_CH0);	                 // DMA通道x传输使能
	
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
    delay_1ms(100);
	adc_calibration_enable            ();                                              // ADC校准复位
	adc_dma_mode_enable               ();                                              // ADCx DMA请求使能

	adc_software_trigger_enable       (ADC_REGULAR_CHANNEL);                           // ADC软件触发使能。

}

uint8_t Get_Adc_Val( uint16_t *bat,uint16_t *s ,uint16_t *ss)
{       
    *s   =0;
	*ss  =0;
	*bat =0;
    temperature=0;
    vrefint = 0;
	for(uint8_t i=0;i<10;)
	{
		*s   += ADC_Value[0];   
		*ss  += ADC_Value[1];
        *bat += ADC_Value[2];  
        temperature += ADC_Value[3];
        vrefint += ADC_Value[4];
        i=i+1;
    } 
    return 0;
}

uint8_t get_low_filter(uint16_t  *BAT,uint16_t *vol1, uint16_t *vol2)
{
    float dPower = 0.1; 
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

//    vref_cal = (float)(adcref_read((adcrefcal *) 0x1FFF75AA)); 
//    VDDA_Volatge = (3.0*vref_cal)/VrefLastnum;   
    adc_values.vrefint=  4096 *1.2/VrefLastnum;
    adc_data[0] = Lastnum0;
    adc_data[1] = Lastnum1;
    adc_data[2] = Lastnum2;
    adc_data[3] = temperaturelastnum/10;
    adc_data[4] = adc_values.vrefint/10;
    return 0;
}

uint32_t Get_Cell_Voltage(uint8_t cell_number) {
	if (cell_number > 4) {
		return UINT32_MAX;
	}
	return adc_values.cell_voltage[cell_number];
}


float Get_MCU_Temperature(){
    adc_values.temperature = ((1.450- (adc_data[3] *3.3/4096))/(4.3/1000))+25;
	return adc_values.temperature;
}


