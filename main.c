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
//uint16_t vol_1s, vol_2s, vol_3s, vol_4s, vol_bat;
extern float valueBAT,value1S,value2S,value3S,value4S;
//uint16_t ADC_Value[3];
//float batdata[6];
extern uint8_t cell;
extern struct Adc adc_values;

float tempera;

osThreadId myTask01Handle;
osThreadId myTask02Handle;
osThreadId myTask03Handle;
osThreadId myTask04Handle;

void Led_Task(void const * argument);
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

    osThreadDef(read_adc, ADC_Task, osPriorityAboveNormal, 0, 128);
    myTask01Handle = osThreadCreate(osThread(read_adc), NULL);

    osThreadDef(Charger_regulator, Charger_Task, osPriorityHigh, 0, 128);
    myTask02Handle = osThreadCreate(osThread(Charger_regulator), NULL);

    osThreadDef(RGB_LED, Led_Task, osPriorityAboveNormal, 0, 128);
    myTask03Handle = osThreadCreate(osThread(RGB_LED), NULL);

    osThreadDef(NONE_TIME, Led_Task, osPriorityHigh, 0, 128);
    myTask04Handle = osThreadCreate(osThread(NONE_TIME), NULL);

    /* start scheduler */
    vTaskStartScheduler();

    while(1){
    }
}
//bool status = false;
void ADC_Task(void const * pvParameters)
{
    static int i;
    for( ;; ){ 
        Read_Cell_Voltage();
        tempera = Get_MCU_Temperature();
        cell = Get_Number_Of_Cells();
        Battery_Connection_State();
        i++;
        printf("%d,%d,%1.3f,%1.3f,%1.3f,%1.3f,%1.3f \r\n",i,cell, adc_values.cell_voltage[0],value1S,value2S,value3S,value4S);
//        printf("%d,%d,%1.3f,%1.3f,%1.3f,%1.3f,%1.3f \r\n",i,cell, adc_values.cell_voltage[0],value1S,value2S,value3S,value4S);
//        printf(" %1.3f,%1.3f\r\n",tempera,adc_values.vrefint);
        vTaskDelay(2);
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
        //Check if power into regulator is okay
		if (Read_Charge_Okay() != 1) {
			Set_Error_State(VOLTAGE_INPUT_ERROR);
		}
		else if ((Get_Error_State() & VOLTAGE_INPUT_ERROR) == VOLTAGE_INPUT_ERROR) {
			Clear_Error_State(VOLTAGE_INPUT_ERROR);
		}

		if ((Get_Error_State() & REGULATOR_COMMUNICATION_ERROR) == REGULATOR_COMMUNICATION_ERROR) {
			regulator.connected = 0;
		}
        Read_Charge_Status();
        Regulator_Read_ADC();

        timer_count++;
		if (timer_count < 70) {
        Control_Charger_Output(adc_values.cell_voltage[0],cell);
		}
		else if (timer_count > 100){
			timer_count = 0;
		}
		else {
        Regulator_HI_Z(1);
		}
        vTaskDelay(2);
    }
}

void Led_Task(void const * argument)
{
    uint8_t count = 0;

    for( ;; ){
        vTaskDelay(5);

	    if ( (Get_Balance_Connection_State() != CONNECTED) && (Get_Error_State() == 0)) {
 			switch (count) {
			case 0:       
                LED_Control(green);
 		        break;
			case 1:  
                LED_Control(blue);
 		        break; 
			case 2:   
                LED_Control(red);
 		        break;      
        }
        if (count == 2) {
			count = 0;
		}
		else {
			count++;
		}
		}
		else if (Get_Error_State() != 0) {
            LED_Control(none);
            for (int i = 0; i < (Get_Error_State()); i++) {
                vTaskDelay(50);
                LED_Control(red);
	            vTaskDelay(50);
                LED_Control(none);
            }
            vTaskDelay(30);
        }
        else {
            if (Get_Balancing_State() >= 1) {
                LED_Control(blue);
                vTaskDelay(5);
            }
            else if(Get_Requires_Charging_State() == 1) {
//				LED_Control(red);
                Charge_RGB_Control(valueBAT,cell);
                vTaskDelay(5);
			}
			else if ((Get_Requires_Charging_State() == 0) && (Get_Balancing_State() == 0)) {
				LED_Control(green);
                vTaskDelay(5);
			}
			else {
				LED_Control(none);
			}
		}
        vTaskDelay(1);
    }
        
}

void NONE_TIME()
{
    for( ;; ){
        vTaskDelay(1);
    }    
}
