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

#define ECU_MAX_TOOTH_TIME (uint16_t)(0xffff/3)
#define ECU_VR_SYNC_POINT (uint8_t)1

extern void ecu_crank_gap_search(ecu_t* ecu);
extern void ecu_crank_gap_check(ecu_t* ecu);
extern void ecu_crank_min_time_check(ecu_t* ecu);

#endif /* ECU_SYNC_H */

