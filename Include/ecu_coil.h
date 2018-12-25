/* 
 * File:   ecu_coil.h
 * Author: ruslan
 *
 * Created on 13 декабря 2018 г., 0:31
 */

#ifndef ECU_COIL_H
#define ECU_COIL_H

#include "ecu_base.h"

#define ECU_COIL_TIM_1 TIM3
#define ECU_COIL_TIM_1_IRQn TIM3_IRQn
#define ECU_COIL_TIM_1_IRQHandler TIM3_IRQHandler

#define ECU_COIL_TIM_2 TIM4
#define ECU_COIL_TIM_2_IRQn TIM4_IRQn
#define ECU_COIL_TIM_2_IRQHandler TIM4_IRQHandler

typedef struct _coil_event_t {
    struct _coil_event_t* next;
    struct _coil_event_t* prev;
    timer_ch_it_t event_ch;
    uint16_t angle;
} coil_event_t;

coil_event_t coil_set[4];
coil_event_t *coil_set_current;
coil_event_t coil_reset[4];
coil_event_t *coil_reset_current;
        
extern void ecu_coil_handler(ecu_t* ecu);
extern void ecu_coil_init(void);
extern void ecu_all_coil_reset(void);

#endif /* ECU_COIL_H */

