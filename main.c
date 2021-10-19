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
extern struct Regulator regulator;
extern float valueBAT,value1S,value2S,value3S,value4S;
extern uint8_t cell;
extern struct Adc adc_values;
extern char KEY1,KEY1_Flag;
extern unsigned char NStep; // RGB色彩需要几步，0-255.
uint8_t charger_flag = 1;

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

    osThreadDef(read_adc, ADC_Task, osPriorityAboveNormal, 0, 128);
    myTask01Handle = osThreadCreate(osThread(read_adc), NULL);

    osThreadDef(Charger_regulator, Charger_Task, osPriorityHigh, 0, 128);
    myTask02Handle = osThreadCreate(osThread(Charger_regulator), NULL);

    osThreadDef(RGB_LED, Led_Task, osPriorityAboveNormal, 0, 128);
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
    static int NUM;
    for( ;; ){ 
        Read_Cell_Voltage();
        tempera = Get_MCU_Temperature();
        cell = Get_Number_Of_Cells();
 		switch (KEY1_Flag) {
            case 0: 
            Battery_Connection_State();
            full_charger_Check(adc_values.cell_voltage[0],cell);
            break;
            case 1:

            break;
        }
//        NUM++;
//        printf("%d,%d,%1.3f,%1.3f,%1.3f,%1.3f,%1.3f \r\n",NUM,cell, adc_values.cell_voltage[0],adc_values.cell_voltage[1],adc_values.cell_voltage[2],adc_values.cell_voltage[3],adc_values.cell_voltage[4]);   
//        if(NUM>10000) NUM=0;
        vTaskDelay(1);
    }
}
void Charger_Task(void const * pvParameters)
{
    Regulator_HI_Z(1);
    Regulator_OTG_EN(0);
    Query_Regulator_Connection();
	regulator.connected = Query_Regulator_Connection();
    Regulator_Set_Charge_Option_0() ;
    Regulator_Set_ADC_Option();
    uint8_t timer_count = 0;
    for( ;; ){

 		switch (KEY1_Flag) {
		case 0: 
            //Check if power into regulator is okay
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
            if (timer_count < 90) {
                Control_Charger_Output(adc_values.cell_voltage[0],cell);
            }else if (timer_count > 100){
                timer_count = 0;
            }else {
                Regulator_HI_Z(1);
            }
            break;
        case 1:
          Control_Charger_Output(0,0);
            break;
        default:
          Control_Charger_Output(0,0);
            break;
       }
       vTaskDelay(2);
    }
}

void Led_Task(void const * pvParameters)
{
    for( ;; ){
 		switch (KEY1_Flag) {
		case 0: 
            vTaskDelay(5);
            if ( (Get_Balance_Connection_State() != CONNECTED) && (Get_Error_State() == 0)) {
                Color_decomposition(none,white);
                LED_Control(ColorToColor(Triangular(NStep)));
                vTaskDelay(5);
            }else if (Get_Error_State() != 0) {
                LED_Control(none);
                for (int a = 0; a < (Get_Error_State()); a++) {
                    vTaskDelay(50);
                    LED_Control(red);
                    vTaskDelay(50);
                    LED_Control(none);
                }
                vTaskDelay(5);
            }else {
                /*充电状态指示*/
                if (Get_XT_Connection_State() == CONNECTED && (Get_Balancing_State() == 0) && charger_flag == 0) {
                        chargerToColor(none, green,0,0);
                }else if(Get_XT_Connection_State() == CONNECTED && charger_flag == 1) {
                    vTaskDelay(10);
                    if (Get_Requires_Charging_State() == 1) {
                        chargerToColor(red,0xFEFF00,adc_values.cell_voltage[0],Get_Number_Of_Cells());
                    }
                }else if (Get_XT_Connection_State() != CONNECTED) {
                    vTaskDelay(10);
                    if (Get_Balancing_State() >= 1) {
                        LED_Control(blue);
                    }else{
                        chargerToColor(none, blue,0,0);
                    }
                }
            }
            break;
        case 1:
            chargerToColor(none, orange,adc_values.cell_voltage[0],Get_Number_Of_Cells());
            vTaskDelay(5);
            break;
        default:
            Control_Charger_Output(0,0);
            break;
       }
       vTaskDelay(10);
    }
        
}

void None_Task(void const * pvParameters)
{
    for( ;; ){
        KEY_Scan();
        if(KEY1_Flag)
        {
            if(adc_values.cell_voltage[0] >= cell*3.5){
                Control_Charger_Output(0,0);
                chargerToColor(red,0xFDFF00,adc_values.cell_voltage[0],Get_Number_Of_Cells());
                Balancing_GPIO_Control(0xff);
            }else if(adc_values.cell_voltage[0] > cell*3.3 && adc_values.cell_voltage[0] < cell*3.5){
                Set_Charge_Voltage(cell);
                Set_Charge_Current(64);
                Regulator_HI_Z(0);
                Balance_Battery();
            }else{
                Control_Charger_Output(0,0);
                chargerToColor(none,red,0,0);
                Balancing_GPIO_Control(0);
            }
        }
        vTaskDelay(10);
    }
}
