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

#include "ecu_coil.h"

extern uint16_t ecu_crank_period_calc(ecu_t* ecu);
extern void crank_extrapolation_calc(ecu_t *ecu);
extern uint16_t ecu_coil_interpolation_calc(uint16_t action_angle,uint16_t angle, uint16_t capture, uint16_t next_period, uint16_t next_angle_period);
extern uint16_t ecu_coil_set_angle_calc(ecu_t* ecu, uint8_t prev_1, uint8_t vr_count, uint16_t reset_angle, uint16_t time);

#endif /* ECU_MATH_H */

