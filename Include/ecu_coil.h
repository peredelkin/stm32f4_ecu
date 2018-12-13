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

typedef struct {
	timer_ch_it_t event_ch;
	uint16_t angle;
	uint16_t angle_s;
        bool busy;
}coil_event_t;

typedef struct {
	coil_event_t set;
	coil_event_t reset;
}coil_t;

coil_t coil_1_4;
coil_t coil_2_3;

extern void ecu_coil_handler(ecu_t* ecu);
extern void ecu_coil_init(void);

#endif /* ECU_COIL_H */
