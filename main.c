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
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "ecu_capture.h"
#include "ecu_coil.h"
#include "usart_bus.h"

usart_bus_t usart2;

void usart_bus_init_common() {
    gpio_usart2_init(); //USART2 gpio init
    
    usart_bus_init_t usart2_init = {
        .dma_rx_channel = DMA1_Stream5, /*CH4*/
        .dma_tx_channel = DMA1_Stream6,  /*CH4*/
        .usart_device = USART2
    };
    
    dma_stream_channel_selection(&usart2_init.dma_rx_channel,4);
    dma_stream_channel_selection(&usart2_init.dma_tx_channel,4);
    
    usart_bus_baud_rate_set(&usart2_init,SystemCoreClock/4,115200);
    
    usart_bus_init(&usart2,&usart2_init);
}

void ecu_crank_handler_callback(void* channel) {
    ecu_crank_capture_handler(&ecu_struct, channel);
    ecu_common_angle_handler(&ecu_struct);
}


//\todo: перенести в ecu_capture.c
void ecu_crank_ovf_handler_callback(void* channel) {
    timer_ch_it_disable(&ecu_struct.cap_ch); //выключение прерывания захвата
    ECU_CAP_TIM->CR1 &= ~TIM_CR1_CEN; //остановка таймера захвата
    ECU_COIL_TIM_1->CR1 &= ~TIM_CR1_CEN; //остановка таймера катушек 1
    ECU_COIL_TIM_2->CR1 &= ~TIM_CR1_CEN; //остановка таймера катушек 2

    ECU_CAP_TIM->CNT = 0; //сброс счета таймера захвата
    ECU_COIL_TIM_1->CNT = 0; //сброс счета таймера катушек 1
    ECU_COIL_TIM_2->CNT = 0; //сброс счета таймера катушек 2

    ecu_struct.cap_time_norm = false;
    ecu_struct.gap_correct = false;
    ecu_struct.gap_found = false;

    ecu_all_coil_reset();

    timer_ch_it_enable(&ecu_struct.cap_ch, false); //включение прерывания захвата
}

void ecu_init(void) {
    timer_ch_event_set(&ecu_struct.cap_ch, &ecu_crank_handler_callback); //обработчик захвата
    timer_ch_event_set(&ecu_struct.ovf_cap_ch, &ecu_crank_ovf_handler_callback); //обработчик останова
    timer_ch_it_enable(&ecu_struct.cap_ch, false); //включение прерывания захвата
}

int main() {
    ecu_struct.mg_by_cycle = 250;
    rcc_init(); //тактирование
    usart_bus_init_common();//уарт автобус
    gpio_led_init(); //светодиоды
    gpio_master_timer_init(); //инициализация пина захвата
    ecu_crank_capture_init(&ecu_struct); //инициализация захвата
    ecu_coil_init(&ecu_struct); //инициализация катушек
    ecu_init(); //
    while (1) {

    }
}
