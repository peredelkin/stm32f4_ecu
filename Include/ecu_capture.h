/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ecu_capture.h
 * Author: ruslan
 *
 * Created on 11 декабря 2018 г., 14:28
 */

#ifndef ECU_CAPTURE_H
#define ECU_CAPTURE_H

#include "ecu_math.h"
#include "ecu_sync.h"

extern void ecu_crank_capture_handler(ecu_t* ecu,void* tim_ch);
extern void ecu_crank_capture_init(ecu_t* ecu);
extern void ecu_cap_irq_handler(ecu_t* ecu);

#endif /* ECU_CAPTURE_H */

