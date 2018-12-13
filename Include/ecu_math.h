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

extern uint16_t ecu_crank_period_calc(ecu_t* ecu,uint8_t prev_1,uint16_t capture);
extern void crank_extrapolation_calc(ecu_t *ecu,uint8_t prev_1,uint8_t vr_count,uint8_t next_1,uint8_t next_2);
extern void ecu_coil_interpolation_calc(coil_event_t* action, uint16_t angle, uint16_t capture, uint16_t next_period, uint16_t next_angle_period);

#endif /* ECU_MATH_H */

