/* 
 * File:   ecu_sync.h
 * Author: ruslan
 *
 * Created on 11 августа 2018 г., 14:46
 */

#ifndef ECU_BASE_H
#define ECU_BASE_H

#include <stm32f4xx.h>
#include "timer_ch_it.h" //работа с прерываниями таймера

#define ECU_VR_COUNT (uint8_t)58
#define ECU_VR_COUNT_RESET (uint8_t)ECU_VR_COUNT-1

#define COIL_N 4

#define COIL_0_OFFSET_ANGLE 0
#define COIL_0_GPIO GPIOD
#define COIL_0_BSRR_MASK GPIO_ODR_ODR_13 //оранжевый

#define COIL_1_OFFSET_ANGLE 16384
#define COIL_1_GPIO GPIOD
#define COIL_1_BSRR_MASK GPIO_ODR_ODR_14 //красный

#define COIL_2_OFFSET_ANGLE 32768
#define COIL_2_GPIO GPIOD
#define COIL_2_BSRR_MASK GPIO_ODR_ODR_15 //синий

#define COIL_3_OFFSET_ANGLE 49152
#define COIL_3_GPIO GPIOD
#define COIL_3_BSRR_MASK GPIO_ODR_ODR_12 //зеленый


#define ECU_COIL_TIM_1 TIM3
#define ECU_COIL_TIM_1_IRQn TIM3_IRQn
#define ECU_COIL_TIM_1_IRQHandler TIM3_IRQHandler

#define ECU_COIL_TIM_2 TIM4
#define ECU_COIL_TIM_2_IRQn TIM4_IRQn
#define ECU_COIL_TIM_2_IRQHandler TIM4_IRQHandler

typedef struct {
    uint16_t angle[ECU_VR_COUNT]; //синхронизированная карта углов (дпкв)
    uint16_t capture[ECU_VR_COUNT]; //карта захвата (дпкв)
    uint16_t period[ECU_VR_COUNT]; //карта периода (дпкв)
} crank_time_t;

typedef struct {
    uint8_t prev_2;
    uint8_t prev_1;
    uint8_t count;
    uint8_t sync_point;
    uint8_t next_1;
    uint8_t next_2;
} vr_count_t;

typedef struct {
    uint16_t dwell_angle;
    uint16_t angle;
} ignition_t;

typedef struct {
    ignition_t ignition;
    timer_ch_it_t cap_ch; //канал захвата
    timer_ch_it_t ovf_cap_ch; //канал переполнения захвата
    crank_time_t crank; //таблица захвата
    vr_count_t vr; //номера элементов
    bool gap_found; //метка найдена
    bool gap_correct; //метка верна
    bool cap_time_norm; //время захвата меньше минимального
} ecu_t;

extern void ecu_crank_counter(vr_count_t* vr_struct);

extern uint8_t ecu_crank_vr_numb_normalization(int8_t numb);

extern void ecu_crank_capture_write(ecu_t* ecu, uint8_t numb, uint16_t capture);

extern uint16_t ecu_crank_capture_read(ecu_t* ecu, uint8_t numb);

extern void ecu_crank_period_write(ecu_t* ecu, uint8_t numb, uint16_t period);

extern uint16_t ecu_crank_period_read(ecu_t* ecu, uint8_t numb);

extern void ecu_crank_angle_write(ecu_t* ecu, uint8_t numb, uint16_t angle);

extern uint16_t ecu_crank_angle_read(ecu_t* ecu, uint8_t numb);

#endif /* ECU_BASE_H */

