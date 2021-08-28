#include "dma.h"
extern uint16_t ADC_Value[120];
extern float batdata[6];

void dma_adc_init()
{
///* DMA 閰岖疆 */
//	dma_parameter_struct            dma_init_struct;           // DMA 鍒濆鍖栧弬鏁扮粨鏋勪綋銆?
//	rcu_periph_clock_enable        (RCU_DMA);                  // 浣胯兘澶栬镞堕挓銆?
//	dma_deinit                     (DMA_CH0);                  // 澶崭綅DMA阃氶亾x镄勬墍链夊瘎瀛桦櫒銆?
//	dma_init_struct.periph_addr  = (uint32_t)&(ADC_RDATA);     // 璁剧疆DMA镄勫璁惧湴鍧€锛屼篃鏄疍MA鏁版嵁浼犺緭镄勬簮鍦板潃銆侫DC鍗曟閲囨牱杞崲缁撴潫鍚庯紝浼氩皢鏁版嵁瀛桦叆ADC_RDATA鍦板潃涓?
//	dma_init_struct.memory_addr  = (uint32_t)ADC_Value;          // 璁剧疆DMA瀛桦偍鍣ㄥ湴鍧€锛屼篃鏄疍MAD鏁版嵁浼犺緭镄勭洰镙囧湴鍧€銆?
//	dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;   // 璁剧疆DMA镄勬暟鎹紶杈撴柟鍚戙€?
//	dma_init_struct.number       = 3;                          // 璁剧疆DMA涓€涓懆链熻浼犺緭镄勬暟鎹釜鏁帮紝鍗曚綅鐢变笅鏂硅缃€侫DC浣胯兘浜嗕袱涓噰镙烽€氶亾锛屾墍浠MA姣忎釜锻ㄦ湡闇€瑕佷紶杈扑袱涓暟鎹€?
//	dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_ENABLE;// 璁剧疆DMA鏁版嵁浼犺緭镄勬簮鍦板潃镊绠楁硶澶辫兘銆侫DC姣忔杞崲鍚庢暟鎹瓨鏀剧殑缂揿瓨鍦板潃涓嶅彉锛屾墍浠ヨ繖閲屽皢涔嫔け鑳姐€?
//	dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE; // 璁剧疆DMA鏁版嵁浼犺緭鐩爣鍦板潃镊绠楁硶浣胯兘锛屽洜涓箧MA姣忎釜锻ㄦ湡闇€瑕佷紶阃佷袱涓暟鎹紝镓€浠ュ湪涓€涓懆链熷唴锛岀洰镙囧湴鍧€瑕佽嚜澧炪€?
//	dma_init_struct.periph_width = DMA_MEMORY_WIDTH_8BIT; // 璁剧疆DMA浠庡璁句腑鍙栧嚭镄勬暟鎹綅鏁般€傚洜涓篈DC杞寲鍚庣殑鏁板€兼槸16bit锛屾墍浠ヨ繖閲岃缃负1瀛椼€?
//	dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;     // 璁剧疆DMA瀛桦叆瀛桦偍鍣ㄤ腑镄勬暟鎹綅鏁般€?
//	dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;    // 璁剧疆DMA鏁版嵁浼犺緭楂树紭鍏堢骇銆?
//	dma_init                       (DMA_CH0, &dma_init_struct);// 灏嗙粨鏋勪綋阃佸洖锛屽垵濮嫔寲DMA阃氶亾x
//	dma_circulation_enable         (DMA_CH0);                  // DMA寰幆妯″纺浣胯兘銆傝繖镙稤MA鍦ㄤ紶杈扑袱涓暟鎹粨鏉熷悗锛屽瓨鍌ㄥ櫒鍦板潃澶崭綅锛岃嚜锷ㄥ紑鍚笅涓€杞紶杈撱€?
//	dma_memory_to_memory_disable   (DMA_CH0);                  // 瀛桦偍鍣ㄥ埌瀛桦偍鍣―MA浼犺緭浣胯兘銆傚洜涓鸿繖閲岀敤涓崭笂锛屾墍浠ュけ鑳姐€?
//	dma_channel_enable             (DMA_CH0);	                 // DMA阃氶亾x浼犺緭浣胯兘

}
