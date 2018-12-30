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
    ecu_crank_capture_handler(&ecu_struct,channel);
    ecu_coil_handler(&ecu_struct);
}

void ecu_crank_ovf_handler_callback(void* channel) {
    timer_ch_it_disable(&ecu_struct.cap_ch); //выключение прерывания захвата
    ECU_CAP_TIM->CR1 &= ~TIM_CR1_CEN; //остановка таймера захвата
    ECU_COIL_TIM_1->CR1 &= ~TIM_CR1_CEN; //остановка таймера катушек 1
    ECU_COIL_TIM_2->CR1 &= ~TIM_CR1_CEN; //остановка таймера катушек 2
    
    ECU_CAP_TIM->CNT = 0; //сброс счета таймера захвата
    ECU_COIL_TIM_1->CNT = 0; //сброс счета таймера катушек 1
    ECU_COIL_TIM_2->CNT = 0; //сброс счета таймера катушек 2
    
    ECU_COIL_TIM_1->CR1 |= TIM_CR1_CEN; //разрешение запуска таймера катушек 1 (слейв)
    ECU_COIL_TIM_2->CR1 |= TIM_CR1_CEN; //разрешение запуска таймера катушек 2 (слейв)
    
    ecu_struct.cap_time_norm = false;
    ecu_struct.gap_correct = false;
    ecu_struct.gap_found = false;
    
    ecu_all_coil_reset();
    
//    GPIOD->BSRRH = GPIO_ODR_ODR_12; //зеленый выкл
    timer_ch_it_enable(&ecu_struct.cap_ch,false); //включение прерывания захвата
}

void ecu_init(void) {
    timer_ch_event_set(&ecu_struct.cap_ch,&ecu_crank_handler_callback); //обработчик захвата
    timer_ch_event_set(&ecu_struct.ovf_cap_ch,&ecu_crank_ovf_handler_callback); //обработчик останова
    timer_ch_it_enable(&ecu_struct.cap_ch,false); //включение прерывания захвата
}

volatile uint32_t i;

void delay_1s(void)
{
    i = 800000;
    while(-- i);
}

int main() {
	rcc_init(); //тактирование
	gpio_led_init(); //светодиоды
	gpio_master_timer_init(); //инициализация пина захвата
	ecu_crank_capture_init(&ecu_struct); //инициализация захвата
        ecu_coil_init(&ecu_struct); //инициализация катушек
        ecu_init(); //
	while (1) {
            
	}
}
