/* 
 * File:   ecu_coil.h
 * Author: ruslan
 *
 * Created on 13 декабря 2018 г., 0:31
 */

#ifndef ECU_COIL_H
#define ECU_COIL_H

#include "ecu_base.h"

typedef struct {
    timer_ch_it_t event_ch;
    uint16_t angle;
    bool update;
} ecu_coil_event_t;

typedef struct {
    uint16_t offset_angle;
    ecu_coil_event_t set;
    ecu_coil_event_t reset;
} ecu_coil_t;

ecu_coil_t ign_coil[COIL_N];
        
extern void ecu_coil_handler(ecu_t* ecu);
extern void ecu_coil_init(ecu_t* ecu);
extern void ecu_all_coil_reset(void);

#endif /* ECU_COIL_H */

