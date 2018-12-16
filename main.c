/*
 * File:   main.c
 * Author: ruslan
 *
 * Created on 28 июля 2018 г., 15:18
 */

#include <stm32f4xx.h>
//FreeRTOS begin
//#include <FreeRTOSConfig.h>
//#include <FreeRTOS.h>
//#include <task.h>
//#include <queue.h>
//FreeRTOS end
#include <main.h>
#include <gpio.h>
//#include <usart.h>
#include <string.h>
#include <stdint.h>
#include "ecu_capture.h"
#include "ecu_coil.h"

//ecu_t ecu_struct __attribute__((section(".ccram"))); //структура эбу
ecu_t ecu_struct; //структура эбу

void ECU_CAP_TIM_IRQHandler(void) {
    ecu_cap_irq_handler(&ecu_struct);
}

void ecu_crank_handler_callback(void* channel) {
    GPIOD->BSRRL = GPIO_ODR_ODR_12;
    ecu_crank_capture_handler(&ecu_struct,channel);
    ecu_coil_handler(&ecu_struct);
    GPIOD->BSRRH = GPIO_ODR_ODR_12;
}

void ecu_init(void) {
    timer_ch_event_set(&ecu_struct.cap_ch,&ecu_crank_handler_callback); //
    timer_ch_it_enable(&ecu_struct.cap_ch,false); //const it
    ECU_CAP_TIM->CR1 |= TIM_CR1_CEN;
}

//volatile uint32_t i;
//
//void delay_1s(void)
//{
//    i = 8000000;
//    while(-- i);
//}

int main() {
	rcc_init(); //тактирование
	gpio_led_init(); //светодиоды
	gpio_master_timer_init(); //инициализация пина захвата
	ecu_crank_capture_init(&ecu_struct); //инициализация захвата
        ecu_coil_init(); //инициализация катушек
        ecu_init(); //
	while (1) {
//		delay_1s();
//		GPIOD->ODR ^= GPIO_ODR_ODR_12;
	}
}
