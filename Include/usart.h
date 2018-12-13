/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   usart.h
 * Author: ruslan
 *
 * Created on 29 июля 2018 г., 15:44
 */

#ifndef USART_H
#define USART_H

#include <stm32f4xx.h>
#include <stdbool.h>

typedef struct {
    void* data;
    uint16_t data_index;
    uint16_t data_size;
} usart_trx_t;

typedef struct {
    bool tx_busy;
    usart_trx_t receive;
    usart_trx_t transmit;
    USART_TypeDef* usart_bus_port;
} usart_t;

extern bool usart_bus_status_register_read(usart_t* usart_struct,uint16_t status);

extern void usart_bus_status_register_reset(usart_t* usart_struct,uint16_t status);

extern void usart_bus_baud_rate_register_set(usart_t* usart_struct,uint16_t mantissa, uint16_t fraction);

extern void usart_bus_control_register_set_1(usart_t* usart_struct,uint16_t ctrl);

extern void usart_bus_control_register_set_2(usart_t* usart_struct,uint16_t ctrl);

extern void usart_bus_control_register_set_3(usart_t* usart_struct,uint16_t ctrl);

extern void usart_bus_control_register_reset_1(usart_t* usart_struct,uint16_t ctrl);

extern void usart_bus_control_register_reset_2(usart_t* usart_struct,uint16_t ctrl);

extern void usart_bus_control_register_reset_3(usart_t* usart_struct,uint16_t ctrl);

extern bool usart_bus_control_register_read_1(usart_t* usart_struct,uint16_t ctrl);

extern bool usart_bus_control_register_read_2(usart_t* usart_struct,uint16_t ctrl);

extern bool usart_bus_control_register_read_3(usart_t* usart_struct,uint16_t ctrl);

extern void usart_bus_guard_time_register_set(usart_t* usart_struct,uint16_t guart_time);

extern void usart_bus_prescaler_register_set(usart_t* usart_struct,uint16_t prescaler);

extern void usart_bus_flag_tc_reset(usart_t* usart_struct);

extern bool usart_bus_flag_tc_read(usart_t* usart_struct);

extern bool usart_bus_flag_txe_read(usart_t* usart_struct);

extern bool usart_bus_oversampling_mode_read(usart_t* usart_struct);

extern void usart_bus_interrupt_txe_enable(usart_t* usart_struct);

extern void usart_bus_interrupt_txe_disable(usart_t* usart_struct);

extern uint32_t usart_bus_data_register_addr(usart_t* usart_struct);

extern void usart_bus_dma_tx_enable(usart_t* usart_struct);

extern void usart_bus_dma_tx_disable(usart_t* usart_struct);

extern void usart_bus_dma_rx_enable(usart_t* usart_struct);

extern void usart_bus_dma_rx_disable(usart_t* usart_struct);

extern void usart_bus_data_read(usart_t* usart_struct,void* data, uint16_t index);

extern void usart_bus_data_write(usart_t* usart_struct,void* data, uint16_t index);

extern void usart_bus_baud_rate_set(usart_t* usart_struct,uint32_t fpclk, uint32_t baud);

extern void usart_bus_write_poll(usart_t* usart_struct,void* data, uint16_t size);

extern bool usart_bus_write_int(usart_t* usart_struct,void* data, uint16_t size);

extern void usart_bus_interrupt_txe_handler(usart_t* usart_struct);

extern void usart_bus_init(usart_t* usart_struct,uint32_t fpclk, uint32_t baud, bool te, bool re);

#endif /* USART_H */

