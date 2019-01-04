/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <usart.h>

bool usart_bus_status_register_read(usart_t* usart_struct,uint16_t status) {
    if (usart_struct->usart_bus_port->SR & status) return 1;
    else return 0;
}

void usart_bus_status_register_reset(usart_t* usart_struct,uint16_t status) {
    usart_struct->usart_bus_port->SR = ~status;
}

void usart_bus_baud_rate_register_set(usart_t* usart_struct,uint16_t mantissa, uint16_t fraction) {
    usart_struct->usart_bus_port->BRR = ((uint16_t) (mantissa << 4) & (uint16_t) USART_BRR_DIV_Mantissa) |
            ((uint16_t) fraction & (uint16_t) USART_BRR_DIV_Fraction);
}

void usart_bus_control_register_set_1(usart_t* usart_struct,uint16_t ctrl) {
    usart_struct->usart_bus_port->CR1 |= ctrl;
}

void usart_bus_control_register_set_2(usart_t* usart_struct,uint16_t ctrl) {
    usart_struct->usart_bus_port->CR2 |= ctrl;
}

void usart_bus_control_register_set_3(usart_t* usart_struct,uint16_t ctrl) {
    usart_struct->usart_bus_port->CR3 |= ctrl;
}

void usart_bus_control_register_reset_1(usart_t* usart_struct,uint16_t ctrl) {
    usart_struct->usart_bus_port->CR1 &= ~ctrl;
}

void usart_bus_control_register_reset_2(usart_t* usart_struct,uint16_t ctrl) {
    usart_struct->usart_bus_port->CR2 &= ~ctrl;
}

void usart_bus_control_register_reset_3(usart_t* usart_struct,uint16_t ctrl) {
    usart_struct->usart_bus_port->CR3 &= ~ctrl;
}

bool usart_bus_control_register_read_1(usart_t* usart_struct,uint16_t ctrl) {
    if (usart_struct->usart_bus_port->CR1 & ctrl) return 1;
    else return 0;
}

bool usart_bus_control_register_read_2(usart_t* usart_struct,uint16_t ctrl) {
    if (usart_struct->usart_bus_port->CR2 & (uint16_t) ctrl) return 1;
    else return 0;
}

bool usart_bus_control_register_read_3(usart_t* usart_struct,uint16_t ctrl) {
    if (usart_struct->usart_bus_port->CR3 & (uint16_t) ctrl) return 1;
    else return 0;
}

void usart_bus_guard_time_register_set(usart_t* usart_struct,uint16_t guart_time) {
    usart_struct->usart_bus_port->GTPR &= ~((uint16_t) USART_GTPR_GT);
    usart_struct->usart_bus_port->GTPR |= (guart_time & (uint16_t) USART_GTPR_GT);
}

void usart_bus_prescaler_register_set(usart_t* usart_struct,uint16_t prescaler) {
    usart_struct->usart_bus_port->GTPR &= ~((uint16_t) USART_GTPR_PSC);
    usart_struct->usart_bus_port->GTPR |= (prescaler & (uint16_t) USART_GTPR_PSC);
}

void usart_bus_flag_tc_reset(usart_t* usart_struct) {
    usart_bus_status_register_reset(usart_struct,USART_SR_TC);
}

bool usart_bus_flag_tc_read(usart_t* usart_struct) {
    return usart_bus_status_register_read(usart_struct,USART_SR_TC);
}

bool usart_bus_flag_txe_read(usart_t* usart_struct) {
    return usart_bus_status_register_read(usart_struct,USART_SR_TXE);
}

bool usart_bus_oversampling_mode_read(usart_t* usart_struct) {
    return usart_bus_control_register_read_1(usart_struct,USART_CR1_OVER8);
}

void usart_bus_interrupt_txe_enable(usart_t* usart_struct) {
    usart_bus_control_register_set_1(usart_struct,USART_CR1_TXEIE);
}

void usart_bus_interrupt_txe_disable(usart_t* usart_struct) {
    usart_bus_control_register_reset_1(usart_struct,USART_CR1_TXEIE);
}

uint32_t usart_bus_data_register_addr(usart_t* usart_struct) {
    return (uint32_t)&(usart_struct->usart_bus_port->DR);
}

void usart_bus_dma_tx_enable(usart_t* usart_struct) {
    usart_bus_control_register_set_3(usart_struct,USART_CR3_DMAT);
}

void usart_bus_dma_tx_disable(usart_t* usart_struct) {
    usart_bus_control_register_reset_3(usart_struct,USART_CR3_DMAT);
}

void usart_bus_dma_rx_enable(usart_t* usart_struct) {
    usart_bus_control_register_set_3(usart_struct,USART_CR3_DMAR);
}

void usart_bus_dma_rx_disable(usart_t* usart_struct) {
    usart_bus_control_register_reset_3(usart_struct,USART_CR3_DMAR);
}

void usart_bus_data_read(usart_t* usart_struct,void* data, uint16_t index) {
    ((uint8_t*) data)[index] = (uint8_t) (usart_struct->usart_bus_port->DR & (uint16_t) USART_DR_DR);
}

void usart_bus_data_write(usart_t* usart_struct,void* data, uint16_t index) {
    usart_struct->usart_bus_port->DR = ((uint16_t) ((uint8_t*) data)[index] & (uint16_t) USART_DR_DR);
}

void usart_bus_baud_rate_set(usart_t* usart_struct,uint32_t fpclk, uint32_t baud) {
    uint32_t over8 = (uint32_t) usart_bus_oversampling_mode_read(usart_struct);
    uint32_t mantissa = (fpclk / ((8 * (2 - over8)) * baud));
    uint16_t divider = (uint16_t) (fpclk / baud);
    uint16_t fraction = (uint16_t) (divider - (uint16_t) (mantissa << 4));
    if (over8) {
        usart_bus_baud_rate_register_set(usart_struct,mantissa, (fraction & (uint16_t) 0x07));
    } else {
        usart_bus_baud_rate_register_set(usart_struct,mantissa, fraction);
    }
}

void usart_bus_write_poll(usart_t* usart_struct,void* data, uint16_t size) {
    usart_struct->transmit.data = data;
    usart_struct->transmit.data_index = 0;
    usart_struct->transmit.data_size = size;
    while (usart_struct->transmit.data_index != usart_struct->transmit.data_size) {
        while (!usart_bus_flag_txe_read(usart_struct));
        usart_bus_data_write(usart_struct,usart_struct->transmit.data, usart_struct->transmit.data_index);
        usart_struct->transmit.data_index++;
    }
}

bool usart_bus_write_int(usart_t* usart_struct,void* data, uint16_t size) {
    if (usart_struct->tx_busy) {
        return usart_struct->tx_busy;
    } else {
        usart_struct->tx_busy = 1;
        usart_struct->transmit.data = data;
        usart_struct->transmit.data_index = 0;
        usart_struct->transmit.data_size = size;
        usart_bus_interrupt_txe_enable(usart_struct);
    }
}

void usart_bus_interrupt_txe_handler(usart_t* usart_struct) {
    if (usart_bus_flag_txe_read(usart_struct)) {
        usart_bus_data_write(usart_struct,usart_struct->transmit.data, usart_struct->transmit.data_index);
        usart_struct->transmit.data_index++;
        if (usart_struct->transmit.data_index >= usart_struct->transmit.data_size) {
            usart_struct->tx_busy = 0;
            usart_bus_interrupt_txe_disable(usart_struct);
        }
    }
}

void usart_bus_init(usart_t* usart_struct,uint32_t fpclk, uint32_t baud, bool te, bool re) {
    usart_bus_baud_rate_set(usart_struct,fpclk, baud);
    if (te) {
        usart_bus_control_register_set_1(usart_struct,USART_CR1_TE); //TX Enable
    } else {
        usart_bus_control_register_reset_1(usart_struct,USART_CR1_TE); //TX Disable
    }

    if (re) {
        usart_bus_control_register_set_1(usart_struct,USART_CR1_RE); //RX Enable
    } else {
        usart_bus_control_register_reset_1(usart_struct,USART_CR1_RE); //RX Disable
    }

    if (te || re) {
        usart_bus_control_register_set_1(usart_struct,USART_CR1_UE); //USART Enable
    } else {
        usart_bus_control_register_reset_1(usart_struct,USART_CR1_UE); //USART Disable
    }
}