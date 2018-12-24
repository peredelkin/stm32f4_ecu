/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   timer_ch_it.h
 * Author: ruslan
 *
 * Created on 10 августа 2018 г., 19:41
 */

#ifndef TIMER_CH_IT_H
#define TIMER_CH_IT_H

#include <stm32f4xx.h>
#include <stdbool.h>
#include "mutex.h"

//#define TIM_CH_IT_BB

#define make_timer_ch_it_init(tim_ch_it_struct,TIM,channel) \
timer_ch_it_init(tim_ch_it_struct,TIM,channel,TIM_SR_CC##channel##IF,TIM_DIER_CC##channel##IE);

typedef void (*_timer_event)(void*);

typedef struct {
    __IO uint32_t* DIER;
    __IO uint32_t* SR;
    __IO uint32_t* CCR;
    uint8_t SR_MASK;
    uint8_t IE_MASK;
    bool once;
    _timer_event event;
    mutex_t mutex;
} timer_ch_it_t;

extern void timer_ch_it_init(timer_ch_it_t* t_it_ch, TIM_TypeDef* TIM,
        const uint8_t channel, const uint16_t status_mask,
        const uint16_t interrupt_mask);

extern void timer_ch_it_handler(timer_ch_it_t* t_it_ch);

extern void timer_ch_it_enable(timer_ch_it_t* t_it_ch, bool once);

extern void timer_ch_it_disable(timer_ch_it_t* t_it_ch);

extern uint16_t timer_ch_ccr_read(timer_ch_it_t* t_it_ch);

extern void timer_ch_ccr_write(timer_ch_it_t* t_it_ch, const uint16_t ccr);

extern void timer_ch_event_set(timer_ch_it_t* t_it_ch, void (*event)(void*));

#endif /* TIMER_CH_IT_H */

