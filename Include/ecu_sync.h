/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ecu_sync.h
 * Author: ruslan
 *
 * Created on 11 декабря 2018 г., 14:02
 */

#ifndef ECU_SYNC_H
#define ECU_SYNC_H

#include "ecu_base.h"
#include "ecu_compare.h"
#include "ecu_crank_angle_map.h"

#define ECU_VR_SYNC_POINT (uint8_t)1

extern void ecu_crank_gap_search(ecu_t* ecu,uint8_t prev_2, uint8_t prev_1, uint8_t vr_count);
extern void ecu_crank_gap_check(ecu_t* ecu,uint8_t prev_2, uint8_t prev_1, uint8_t vr_count);

#endif /* ECU_SYNC_H */

