/*!
    \file    main.c
    \brief   Blance Charger
    
    \version 2021-08-02, V1.0.0, firmware for GD32E23k6U6
*/

#include "gd32e23x.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cmsis_os.h"

#include "board.h"

//uint16_t vol_1s, vol_2s, vol_3s, vol_4s, vol_bat;
extern float valueBAT,value1S,value2S,value3S,value4S;
//uint16_t ADC_Value[3];
//float batdata[6];
extern uint8_t cell;
float tempera;

osThreadId myTask01Handle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;

void StartTask(void const * argument);
void ADC_Task(void const * argument);
void Charger_Task(void const * argument);


int fputc(int ch, FILE *f)
{
        /* 将Printf内容发往串口 */
        usart_data_transmit(USART0, (unsigned char) ch);
        while(RESET == usart_flag_get(USART0, USART_FLAG_TC));       
        return (ch);
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
    systick_config(); 
    
    boardInit();
    usart_data_transmit(USART0,0X10);  

    osThreadDef(StartInitTask, StartTask, osPriorityHigh, 0, 128);
    myTask01Handle = osThreadCreate(osThread(StartInitTask), NULL);

    osThreadDef(read_adc, ADC_Task, osPriorityHigh, 0, 128);
    myTask02Handle = osThreadCreate(osThread(read_adc), NULL);

    osThreadDef(Charger_regulator, Charger_Task, osPriorityHigh, 0, 128);
    myTask03Handle = osThreadCreate(osThread(Charger_regulator), NULL);
    /* start scheduler */
    vTaskStartScheduler();

    while(1){
    }
}
bool status = false;
void StartTask(void const * pvParameters)
{

    for( ;; ){ 
        vTaskDelay(100);
//LED_Control(0x00ffff);
//        vTaskDelay(100);
//LED_Control(0x00ff00);
//        vTaskDelay(100);
//LED_Control(0xff0000);
//        vTaskDelay(100);
//LED_Control(0x0000ff);
        Charge_RGB_Control(valueBAT,cell);
        vTaskDelay(100);
    }
}
void ADC_Task(void const * pvParameters)
{
    for( ;; ){

        Read_Cell_Voltage();
        tempera = Get_MCU_Temperature();
        Balance_Connection_State();
        cell = Get_Number_Of_Cells();

        printf("%d -- %1.3f,%1.3f,%1.3f,%1.3f,%1.3f \r\n",cell,valueBAT,value1S,value2S,value3S,value4S);
        printf(" %1.3f \r\n",tempera);

        vTaskDelay(2);
    }
}

void Charger_Task(void const * argument)
{
    for( ;; ){
        Read_Charge_Status();
//        Control_Charger_Output(valueBAT,cell);
        vTaskDelay(5);
    }
}


