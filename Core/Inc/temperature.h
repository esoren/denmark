/*
 * temperature.c
 *
 *  Created on: Mar 5, 2023
 *      Author: Eric
 */

#ifndef INC_TEMPERATURE_H_
#define INC_TEMPERATURE_H_


#define TEMPERATURE_SAMPLE_TIME_MS 1000
#define T1_BETA 4105
#define T2_BETA 4105
#define T3_BETA 4105

#define T1_OVERHEAT_THRESH 30
#define T2_OVERHEAT_THRESH 30
#define T3_OVERHEAT_THRESH 30


void StartTemperatureTask(void const *argument);
float convert_adc_count_to_degrees_celcius(uint32_t adc_count, float thermistor_beta);

#endif /* INC_TEMPERATURE_H_ */
