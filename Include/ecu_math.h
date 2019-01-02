/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ecu_maths.h
 * Author: ruslan
 *
 * Created on 11 декабря 2018 г., 15:14
 */

#ifndef ECU_MATH_H
#define ECU_MATH_H

#include "ecu_base.h"

extern uint16_t ecu_crank_period_calc(ecu_t* ecu);
extern void crank_extrapolation_calc(ecu_t *ecu);
extern uint16_t ecu_coil_interpolation_calc(uint16_t action_angle,uint16_t angle, uint16_t capture, uint16_t next_period, uint16_t next_angle_period);
extern uint16_t ecu_coil_delta_angle_calc(ecu_t* ecu,const uint8_t prev_1,const uint8_t vr_count,const uint16_t time);
extern void ecu_instant_rpm_calc(ecu_t* ecu);
extern uint16_t ecu_ign_angle_mg_by_cycle_calc(uint16_t rpm_data,uint16_t mg_data);

#endif /* ECU_MATH_H */

