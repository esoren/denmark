/*
 * temperature.c
 *
 *  Created on: Mar 5, 2023
 *      Author: Eric
 */
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "gpio.h"
#include "adc.h"
#include "math.h"
#include "temperature.h"
#include "display.h"
#include "fault.h"


QueueHandle_t xTemperatureQueue;

extern QueueHandle_t xDisplayQueue;

//these global variables are only used for SWV data tracing to monitor temperatures

volatile float therm1_global;
volatile float therm2_global;
volatile float therm3_global;

void StartTemperatureTask(void const *argument) {
	uint32_t adc_values[3];

	// start the ADC in DMA mode and store the values for THERM1, THERM2, and THERM3 in adc_values
	HAL_ADC_Start_DMA(&hadc1, adc_values, 3);
	osDelay(20); //delay on startup to let the first conversions start

	float therm1, therm2, therm3;

	uint8_t temperatureMessage;
	uint8_t temperature_monitor_is_running = 0; //if this flag is set, the temperature monitor will run

	displayMessage_t displayMessage;

	BaseType_t xStatus;


	for(;;)
	{


		if (uxQueueMessagesWaiting(xTemperatureQueue) > 0) {
			xStatus = xQueueReceive(xTemperatureQueue, &temperatureMessage, 0);
			if (xStatus == pdPASS) {
				if(temperatureMessage == TEMPERATURE_MONITOR_START) {
					temperature_monitor_is_running = 1;
				} else if(temperatureMessage == TEMPERATURE_MONITOR_STOP) {
					temperature_monitor_is_running = 0;
				} else {
					//should not reach here. Do nothing.
				}
			}
		}

		therm1 = convert_adc_count_to_degrees_celcius(adc_values[0], T1_BETA);
		therm1_global = therm1;

		therm2 = convert_adc_count_to_degrees_celcius(adc_values[1], T2_BETA);
		therm2_global = therm2;

		therm3 = convert_adc_count_to_degrees_celcius(adc_values[2], T3_BETA);
		therm3_global = therm3;

		if(temperature_monitor_is_running) {



			displayMessage.displayCommand = SET_LED_STATE;
			displayMessage.modify_mask = LED_MONITOR_OVERTEMP;


			if( (therm1 > T1_OVERHEAT_THRESH) || (therm2 > T2_OVERHEAT_THRESH) || (therm3 > T3_OVERHEAT_THRESH) )
			{
				displayMessage.new_values = 0xffff;
			} else {
				displayMessage.new_values = 0;
			}

			xQueueSend(xDisplayQueue, &displayMessage, 0);

			if( therm1 > T1_OVERHEAT_THRESH ) {
				set_clear_fault_flags(FAULT_HF_TEMP, 1);
			}

			if( therm2 > T2_OVERHEAT_THRESH ) {
				set_clear_fault_flags(FAULT_MF_TEMP, 1);
			}

			if( therm3 > T3_OVERHEAT_THRESH ) {
				set_clear_fault_flags(FAULT_LF_TEMP, 1);
			}



			osDelay(TEMPERATURE_SAMPLE_TIME_MS);

		} else {
			osDelay(TEMPERATURE_STARTUP_MONITOR_TIME_MS);
		}


	}

}



float convert_adc_count_to_degrees_celcius(uint32_t adc_count, float thermistor_beta) {

	const float R0 		= 100000.0;

	float resistance = R0 / (4095.0 / adc_count - 1.0);
	float temperature = (1.0 / ((log(resistance / R0) / thermistor_beta) + (1.0 / 298.15))) - 273.15;

	return temperature;

}
