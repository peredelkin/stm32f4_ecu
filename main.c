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
#include "modbus_rtu.h"

usart_bus_t usart2;

modbus_rtu_t modbus;

//! Адрес регистра флагов со светодиодом.
#define LED_ADDRESS 0x1
//! Адрес регистра хранения со счётчиком.
#define CNT_ADDRESS 0x1

volatile uint16_t cnt_reg = 0;

modbus_rtu_message_t modbus_rx_msg, modbus_tx_msg;

uint8_t usart2_data[20];

void USART2_IRQHandler() {
//    COIL_2_GPIO->BSRRL = COIL_2_BSRR_MASK;
    usart_bus_irq_handler(&usart2);
//    COIL_2_GPIO->BSRRH = COIL_2_BSRR_MASK;
}

void DMA1_Stream5_IRQHandler() {
    COIL_2_GPIO->BSRRL = COIL_2_BSRR_MASK;
    usart_bus_dma_rx_channel_irq_handler(&usart2);
    COIL_2_GPIO->BSRRH = COIL_2_BSRR_MASK;
}

void DMA1_Stream6_IRQHandler() {
    usart_bus_dma_tx_channel_irq_handler(&usart2);
}

static bool usart_rx_callback(void)
{
    return modbus_rtu_usart_rx_callback(&modbus);
}

static bool usart_tx_callback(void)
{
    return modbus_rtu_usart_tx_callback(&modbus);
}

static bool usart_rx_byte_callback(uint8_t byte)
{
    return modbus_rtu_usart_rx_byte_callback(&modbus, byte);
}

void init_usart() {
    gpio_usart2_init(); //USART2 gpio init
    
    usart2.usart_device = USART2;
    
    usart_bus_baud_rate_set(&usart2,SystemCoreClock/4, 115200);
    
    usart_bus_transmitter_enable(&usart2);
    
    usart_bus_receiver_enable(&usart2);
    
    dma_stream_struct_init(&usart2.dma_rx_channel,DMA1,DMA1_Stream5,5);
    dma_stream_struct_init(&usart2.dma_tx_channel,DMA1,DMA1_Stream6,6);
    
    usart2.dma_rx_channel_n = 4;
    usart2.dma_tx_channel_n = 4;
    
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    NVIC_EnableIRQ(DMA1_Stream6_IRQn);
    
    usart_bus_init(&usart2);

    // Установка каллбэков.
    usart_bus_set_rx_callback(&usart2, usart_rx_callback);
    usart_bus_set_tx_callback(&usart2, usart_tx_callback);
    usart_bus_set_rx_byte_callback(&usart2, usart_rx_byte_callback);
    
    // При обнаружении свободной линии - прекратить принимать данные.
    usart_bus_set_idle_mode(&usart2, USART_IDLE_MODE_END_RX);
    
    sprintf(usart2_data,"Usart inited\r\n");
    
    usart_bus_send(&usart2, usart2_data, strlen(usart2_data));
}

static void modbus_on_msg_recv_callback(void)
{
    modbus_rtu_dispatch(&modbus);
}

static void led_on() {
    COIL_3_GPIO->BSRRL = COIL_3_BSRR_MASK;
}

static void led_off() {
    COIL_3_GPIO->BSRRH = COIL_3_BSRR_MASK;
}

static bool led_status() {
    if(COIL_3_GPIO->ODR & COIL_3_BSRR_MASK) return true;
    else return false;
}

static modbus_rtu_error_t modbus_on_read_coil(uint16_t address, modbus_rtu_coil_value_t* value)
{
    // Если не адрес светодиода - возврат ошибки.
    if(address != LED_ADDRESS) return MODBUS_RTU_ERROR_INVALID_ADDRESS;

    // Передадим состояние светодиода.
    *value = led_status();
    
    return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_on_write_coil(uint16_t address, modbus_rtu_coil_value_t value)
{
    // Если не адрес светодиода - возврат ошибки.
    if(address != LED_ADDRESS) return MODBUS_RTU_ERROR_INVALID_ADDRESS;
    
    // Зажжём или погасим светодиод
    // в зависимости от значения.
    if(value) led_on();
    else led_off();
    
    return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_on_report_slave_id(modbus_rtu_slave_id_t* slave_id)
{
    // Состояние - работаем.
    slave_id->status = MODBUS_RTU_RUN_STATUS_ON;
    // Идентификатор - для пример возьмём 0xaa.
    slave_id->id = 32;
    // В дополнительных данных передадим наше имя.
    slave_id->data = "STM32 MCU Modbus v1.0";
    // Длина имени.
    slave_id->data_size = strlen(slave_id->data);
    
    return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_on_read_hold_reg(uint16_t address, uint16_t* value)
{
    // Если не адрес регистра счётчика - возврат ошибки.
    if(address != CNT_ADDRESS) return MODBUS_RTU_ERROR_INVALID_ADDRESS;
    
    // Передадим значение регистра счётчика.
    *value = cnt_reg;
    
    return MODBUS_RTU_ERROR_NONE;
}

static modbus_rtu_error_t modbus_on_write_reg(uint16_t address, uint16_t value)
{
    // Если не адрес регистра счётчика - возврат ошибки.
    if(address != CNT_ADDRESS) return MODBUS_RTU_ERROR_INVALID_ADDRESS;
    
    // Установим значение регистра счётчика.
    cnt_reg = value;
    
    return MODBUS_RTU_ERROR_NONE;
}

void init_modbus(void)
{
    // Структура инициализации Modbus.
    modbus_rtu_init_t modbus_is;
    
    modbus_is.usart = &usart2; // Шина USART.
    modbus_is.mode = MODBUS_RTU_MODE_SLAVE; // Режим - ведомый.
    modbus_is.address = 32; // Адрес.
    modbus_is.rx_message = &modbus_rx_msg; // Сообщение для приёма.
    modbus_is.tx_message = &modbus_tx_msg; // Сообщение для передачи.
    
    // Инициализируем Modbus.
    modbus_rtu_init(&modbus, &modbus_is);
    // Установка каллбэка получения сообщения.
    modbus_rtu_set_msg_recv_callback(&modbus, modbus_on_msg_recv_callback);
    // Установка каллбэков доступа к данным.
    modbus_rtu_set_read_coil_callback(&modbus, modbus_on_read_coil);
    modbus_rtu_set_write_coil_callback(&modbus, modbus_on_write_coil);
    modbus_rtu_set_report_slave_id_callback(&modbus, modbus_on_report_slave_id);
    modbus_rtu_set_read_holding_reg_callback(&modbus, modbus_on_read_hold_reg);
    modbus_rtu_set_write_holding_reg_callback(&modbus, modbus_on_write_reg);
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

volatile uint32_t i;

void delay_1s(void)
{
    i = 16800000;
    while(-- i);
}

int main() {
    ecu_struct.mg_by_cycle = 250;
    rcc_init(); //тактирование
    gpio_led_init(); //светодиоды
    gpio_master_timer_init(); //инициализация пина захвата
    init_usart(); //уарт автобус
    init_modbus(); //модбас
    ecu_crank_capture_init(&ecu_struct); //инициализация захвата
    ecu_coil_init(&ecu_struct); //инициализация катушек
    ecu_init(); //
    while (1) {
        delay_1s();
        //COIL_2_GPIO->ODR ^= COIL_2_BSRR_MASK;
        cnt_reg++;
       // sprintf(usart2_data,"RX err %u\r\n",usart2.rx_errors);
       // if(usart2.rx_errors) {
        //    usart_bus_send(&usart2, usart2_data, strlen(usart2_data));
       // }
    }
}
