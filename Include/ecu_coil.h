/* 
 * File:   ecu_coil.h
 * Author: ruslan
 *
 * Created on 13 декабря 2018 г., 0:31
 */

#ifndef ECU_COIL_H
#define ECU_COIL_H

#include "ecu_base.h"
        
extern void ecu_coil_handler(ecu_t* ecu);
extern void ecu_coil_init(ecu_t* ecu);
extern void ecu_all_coil_reset(void);

#endif /* ECU_COIL_H */

