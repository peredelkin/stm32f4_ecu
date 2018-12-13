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

typedef void (*_ecu_sync_event)(void*);

typedef struct {
	uint16_t angle[ECU_VR_COUNT]; //синхронизированная карта углов (дпкв)
	uint16_t capture[ECU_VR_COUNT]; //карта захвата (дпкв)
	uint16_t period[ECU_VR_COUNT]; //карта периода (дпкв)
}crank_time_t;

typedef struct {
        timer_ch_it_t capture_ch; //канал захвата
        timer_ch_it_t ovf_cap_ch; //канал переполнения захвата
	crank_time_t crank; //таблица захвата
        uint8_t vr_count; //счетчик зубов для записи захвата
	uint8_t vr_sync_point; //значение vr_count на момент синхронизации
	bool gap_found; //метка найдена
	bool gap_correct; //метка верна
} ecu_t;

ecu_t ecu_struct; //структура эбу

extern void ecu_crank_counter(ecu_t* ecu);

extern uint8_t ecu_crank_vr_numb_normalization(int8_t numb);

extern void ecu_crank_capture_write(ecu_t* ecu,uint8_t numb,uint16_t capture);

extern uint16_t ecu_crank_capture_read(ecu_t* ecu,uint8_t numb);

extern void ecu_crank_period_write(ecu_t* ecu,uint8_t numb,uint16_t period);

extern uint16_t ecu_crank_period_read(ecu_t* ecu,uint8_t numb);

extern void ecu_crank_angle_write(ecu_t* ecu,uint8_t numb,uint16_t angle);

extern uint16_t ecu_crank_angle_read(ecu_t* ecu,uint8_t numb);

#endif /* ECU_BASE_H */

