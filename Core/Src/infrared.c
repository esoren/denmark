/*
 * infrared.c
 *
 *  Created on: Mar 10, 2023
 *      Author: Eric
 */


#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "infrared.h"
#include "tim.h"
#include "gpio.h"



__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds-au32_ticks);
}


void setup_ir_transmitter() {

	//start PWM output timer

	HAL_GPIO_WritePin(IR_SIGNAL_OUT_GPIO_Port, IR_SIGNAL_OUT_Pin, GPIO_PIN_RESET); //must hold this pin low since we are using an OR gate and starting and stopping PWM manually

	DWT_Delay_Init();

#ifdef IR_TRAINING
	train_minidsp();
#endif

}

#ifdef IR_TRAINING

int train_minidsp() {
	volatile int trainbreak = 0;

	trainbreak = 0;
	ir_send_message(0, IR_MASTER_MUTE);
	HAL_Delay(100);
	ir_send_message(0, IR_MASTER_MUTE);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_VOLUME_UP);
	HAL_Delay(100);
	ir_send_message(0, IR_VOLUME_UP);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_VOLUME_DOWN);
	HAL_Delay(100);
	ir_send_message(0, IR_VOLUME_DOWN);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_ANALOG);
	HAL_Delay(100);
	ir_send_message(0, IR_ANALOG);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_TOSLINK);
	HAL_Delay(100);
	ir_send_message(0, IR_TOSLINK);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_USB);
	HAL_Delay(100);
	ir_send_message(0, IR_USB);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_CONFIG_1);
	HAL_Delay(100);
	ir_send_message(0, IR_CONFIG_1);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_CONFIG_2);
	HAL_Delay(100);
	ir_send_message(0, IR_CONFIG_2);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_CONFIG_3);
	HAL_Delay(100);
	ir_send_message(0, IR_CONFIG_3);
	HAL_Delay(100);

	trainbreak = 0;
	ir_send_message(0, IR_CONFIG_4);
	HAL_Delay(100);
	ir_send_message(0, IR_CONFIG_4);
	HAL_Delay(100);

	trainbreak = 0;

	return trainbreak;
}

#endif

void ir_send_message(uint8_t addr, uint8_t cmd) {


	HAL_Delay(1);

	ir_turn_on_carrier();
	DWT_Delay_us(9000);
	ir_turn_off_carrier();
	DWT_Delay_us(4500);



	ir_send_byte(addr);
	ir_send_byte(~addr);
	ir_send_byte(cmd);
	ir_send_byte(~cmd);

	ir_send_zero(); //final pulse
	ir_turn_off_carrier();

}


void ir_send_byte(uint8_t payload) {
	for(int i = 0; i < 8; i++) {
		if((payload >> i) & 0x01) {
			ir_send_one();
		} else {
			ir_send_zero();
		}
	}
	return;
}

void ir_turn_off_carrier() {

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	//set pin to PP output low and turn off PWM
	GPIO_InitStruct.Pin = IR_CARRIER_OUT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_WritePin(IR_CARRIER_OUT_GPIO_Port, IR_CARRIER_OUT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_Init(IR_CARRIER_OUT_GPIO_Port, &GPIO_InitStruct);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
	return;
}

void ir_turn_on_carrier() {

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	//set pin to AF mode and turn on PWM
	GPIO_InitStruct.Pin = IR_CARRIER_OUT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(IR_CARRIER_OUT_GPIO_Port, &GPIO_InitStruct);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	return;
}

void ir_send_zero() {
	ir_turn_on_carrier();
	DWT_Delay_us(562);
	ir_turn_off_carrier();
	DWT_Delay_us(563);
}

void ir_send_one() {
	ir_turn_on_carrier();
	DWT_Delay_us(562);
	ir_turn_off_carrier();
	DWT_Delay_us(1688);

}



uint32_t DWT_Delay_Init(void)
{
    /* Disable TRC */
    CoreDebug->DEMCR &= ~CoreDebug_DEMCR_TRCENA_Msk; // ~0x01000000;
    /* Enable TRC */
    CoreDebug->DEMCR |=  CoreDebug_DEMCR_TRCENA_Msk; // 0x01000000;

    /* Disable clock cycle counter */
    DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; //~0x00000001;
    /* Enable  clock cycle counter */
    DWT->CTRL |=  DWT_CTRL_CYCCNTENA_Msk; //0x00000001;

    /* Reset the clock cycle counter value */
    DWT->CYCCNT = 0;

    /* 3 NO OPERATION instructions */
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");
    __ASM volatile ("NOP");

    /* Check if clock cycle counter has started */
    if(DWT->CYCCNT)
    {
       return 0; /*clock cycle counter started*/
    }
    else
    {
      return 1; /*clock cycle counter not started*/
    }
}


