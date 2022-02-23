/****************************************************************************
* Copyright (C), 
* 文件名: main.c
* 内容简述：
*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "cmsis_os.h"
#include "gd32e23x.h"
#include "systick.h"
#include "board.h"
extern struct Regulator regulator;
extern float valueBAT,value1S,value2S,value3S,value4S;
extern uint8_t cell;
extern struct Adc adc_values;
extern char KEY1,KEY1_Flag;
extern uint8_t send_frame[];

uint8_t charger_flag = 1;
uint32_t charge_timeout = 0;
float tempera;

osThreadId myTask01Handle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;

void Led_Task(void const * argument);
void ADC_Task(void const * argument);
void Charger_Task(void const * argument);
void None_Task(void const * argument);

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
    delay_1ms(1000);
    wwdgt_Init();


    osThreadDef(read_adc, ADC_Task, osPriorityHigh, 0, 128);
    myTask01Handle = osThreadCreate(osThread(read_adc), NULL);

    osThreadDef(Charger_regulator, Charger_Task, osPriorityHigh, 0, 128);
    myTask02Handle = osThreadCreate(osThread(Charger_regulator), NULL);

    osThreadDef(RGB_LED, Led_Task, osPriorityHigh, 0, 128);
    myTask03Handle = osThreadCreate(osThread(RGB_LED), NULL);

    osThreadDef(NONE_TIME, None_Task, osPriorityHigh, 0, 128);
    myTask04Handle = osThreadCreate(osThread(NONE_TIME), NULL);

    /* start scheduler */
    vTaskStartScheduler();

    while(1){
    }
}

void ADC_Task(void const * pvParameters)
{
    Husb238_Voltage_Query();
    for( ;; ){ 
        Read_Cell_Voltage();
        tempera = Get_MCU_Temperature();
        cell = Get_Number_Of_Cells();
        Battery_Connection_State();
        full_charger_Check(adc_values.cell_voltage[0],cell);
        vTaskDelay(5);
    }
}

void Charger_Task(void const * pvParameters)
{
    TickType_t xDelay = 250 / portTICK_PERIOD_MS;
    Regulator_HI_Z(1);
    Query_Regulator_Connection();
	regulator.connected = Query_Regulator_Connection();
    Regulator_Set_Charge_Option_0();
    Regulator_Set_ADC_Option();
    uint8_t timer_count = 0;
    for( ;; ){
        /*检查电源管理芯片状态*/
        if (Read_Charge_Okay() != 1) {
            Set_Error_State(VOLTAGE_INPUT_ERROR);
        }else if ((Get_Error_State() & VOLTAGE_INPUT_ERROR) == VOLTAGE_INPUT_ERROR) {
            Clear_Error_State(VOLTAGE_INPUT_ERROR);
        }

        if ((Get_Error_State() & REGULATOR_COMMUNICATION_ERROR) == REGULATOR_COMMUNICATION_ERROR) {
            regulator.connected = 0;
        }

        Read_Charge_Status();
        Regulator_Read_ADC();
        timer_count++;

        if( charger_flag == 0 && charge_timeout <= 100000 ){
            charge_timeout++;
        }else if( charger_flag == 1 ){
            charge_timeout = 0;
        }

        if (timer_count < 90 && Get_Balance_Connection_State() == CONNECTED && Get_MCU_Temperature() < TEMP_THROTTLE_THRESH_C) {
            if( Get_XT_Connection_State() == CONNECTED ){
                Control_Charger_Output(adc_values.cell_voltage[0],cell);
            }else if( Get_XT_Connection_State() == NOT_CONNECTED){

            }
        }else if (timer_count > 100){
            timer_count = 0;
        }else {
            Regulator_HI_Z(1);
        }

        vTaskDelay(xDelay);
    }
}

void Led_Task(void const * pvParameters)
{
	TickType_t xDelay = 6 / portTICK_PERIOD_MS;
    for( ;; ){
        if ( (Get_Balance_Connection_State() != CONNECTED) && (Get_Error_State() == 0)) {
            Colorful_gradient();
            vTaskDelay(5);
        }else if (Get_Error_State() != 0) {
            LED_Control(none);
            for (int a = 0; a < (Get_Error_State()); a++) {
                vTaskDelay(80);
                LED_Control(red);
                vTaskDelay(80);
                LED_Control(none);
            }
        }else {
            /*充电状态指示*/
            if(Get_XT_Connection_State() == CONNECTED && charger_flag == 0) {
                    LED_Control(green);
            }else if(Get_XT_Connection_State() == CONNECTED && charger_flag == 1) {
                    chargerToColor(red,green,adc_values.cell_voltage[0],Get_Number_Of_Cells());
            }else if(Get_XT_Connection_State() != CONNECTED && Get_Balancing_State() >= 1) {
                    vTaskDelay(2);
                    if(Get_Balancing_State() >= 1) {
                      chargerToColor(none, blue,0,0);
                    }
            }else if(Get_XT_Connection_State() != CONNECTED && Get_Balancing_State()==0){
                    LED_Control(blue);
            }
        }
        vTaskDelay(xDelay);
    }
}

void None_Task(void const * pvParameters)
{
    static uint8_t NUM,cellnum;
    for( ;; ){
        KEY_Scan();
        if(KEY1_Flag)
        {
            if(NUM>=15){
                NUM=0;
                if(cellnum>4){
                    cellnum=0;
                }
             /*printf("%d,%1.3f,%1.3f,%1.3f,%1.3f,%1.3f,%1.3f \r\n",cell, adc_values.cell_voltage[0],adc_values.cell_voltage[1],adc_values.cell_voltage[2],adc_values.cell_voltage[3],adc_values.cell_voltage[4],adc_values.vrefint);  */
                if(cellnum==0){
                    printf("%1.3f@%d",adc_values.cell_voltage[cellnum++],cellnum);
                }else printf("%1.3f@%d",adc_values.cell_voltage[cellnum++],cellnum);
                printf("*");
            }else NUM++;
        }else   cellnum=0;

        vTaskDelay(25);
        wwdgt_counter_update(127);
    }
}
