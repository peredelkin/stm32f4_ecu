/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ecu_compare.h
 * Author: ruslan
 *
 * Created on 11 декабря 2018 г., 8:27
 */

#ifndef ECU_COMPARE_H
#define ECU_COMPARE_H

#include <stdbool.h>
#include <stdint.h>

//возвращает true если метка верна
extern bool crank_gap_correct_check(uint16_t tnbm2_w, uint16_t tnbm1_w, uint16_t tnbm_w);
//возвращает true если один зуб лишний
extern bool crank_gap_one_to_much_check(uint16_t tnbm1_w, uint16_t tnbm_w);
//возвращает true если один зуб пропущен
extern bool crank_gap_one_missing_check(uint16_t tnbm2_w, uint16_t tnbm1_w);
//возвращает true,если искомый углол в заданном окне
extern bool ecu_coil_window_angle_check(uint16_t check_angle,uint16_t angle, uint16_t next_angle);
//возвращает true,если обороты выше минимальных
extern bool ecu_crank_min_time_set(uint16_t min_time,uint16_t tnbm2_w);
//возвращает true,если ниже минимальных
extern bool ecu_crank_min_time_reset(uint16_t min_time,uint16_t tnbm1_w,uint16_t tnbm_w);
//возвращает true,если углы события не в окне поиска
extern bool ecu_coil_update_window_angle_check(uint16_t set_angle,uint16_t reset_angle,uint16_t angle,uint16_t next_angle);

#endif /* ECU_COMPARE_H */

