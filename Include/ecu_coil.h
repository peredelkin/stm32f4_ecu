/* 
 * File:   ecu_coil.h
 * Author: ruslan
 *
 * Created on 13 декабря 2018 г., 0:31
 */

#ifndef ECU_COIL_H
#define ECU_COIL_H

#include "ecu_base.h"

#define COIL_0_GPIO GPIOD
#define COIL_0_BSRR_MASK GPIO_ODR_ODR_13 //оранжевый

#define COIL_1_GPIO GPIOD
#define COIL_1_BSRR_MASK GPIO_ODR_ODR_14 //красный

#define COIL_2_GPIO GPIOD
#define COIL_2_BSRR_MASK GPIO_ODR_ODR_15 //синий

#define COIL_3_GPIO GPIOD
#define COIL_3_BSRR_MASK GPIO_ODR_ODR_12 //зеленый


#define ECU_COIL_TIM_1 TIM3
#define ECU_COIL_TIM_1_IRQn TIM3_IRQn
#define ECU_COIL_TIM_1_IRQHandler TIM3_IRQHandler

#define ECU_COIL_TIM_2 TIM4
#define ECU_COIL_TIM_2_IRQn TIM4_IRQn
#define ECU_COIL_TIM_2_IRQHandler TIM4_IRQHandler

typedef struct {
    timer_ch_it_t event_ch;
    uint16_t angle;
    bool update;
} coil_event_t;

typedef struct {
    uint16_t offset_angle;
    coil_event_t set;
    coil_event_t reset;
} coil_t;

coil_t ign_coil[4];
        
extern void ecu_coil_handler(ecu_t* ecu);
extern void ecu_coil_init(ecu_t* ecu);
extern void ecu_all_coil_reset(void);

#endif /* ECU_COIL_H */

