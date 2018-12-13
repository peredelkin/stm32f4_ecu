#include "ecu_base.h"

void ecu_crank_capture_write(ecu_t* ecu,uint8_t numb,uint16_t capture) {
    ecu->crank.capture[numb] = capture;
}

uint16_t ecu_crank_capture_read(ecu_t* ecu,uint8_t numb) {
    return ecu->crank.capture[numb];
}

void ecu_crank_period_write(ecu_t* ecu,uint8_t numb,uint16_t period) {
    ecu->crank.period[numb] = period;
}

uint16_t ecu_crank_period_read(ecu_t* ecu,uint8_t numb) {
    return ecu->crank.period[numb];
}

void ecu_crank_angle_write(ecu_t* ecu,uint8_t numb,uint16_t angle) {
    ecu->crank.angle[numb] = angle;
}

uint16_t ecu_crank_angle_read(ecu_t* ecu,uint8_t numb) {
    return ecu->crank.angle[numb];
}

uint8_t ecu_crank_vr_numb_normalization(int8_t numb) {
    if(numb > ECU_VR_COUNT_RESET) {
        return (numb - ECU_VR_COUNT);
    } else {
        if(numb < 0) {
            return (ECU_VR_COUNT + numb);
        } else {
            return (uint8_t)numb;
        }
    }
}

void ecu_crank_counter(ecu_t* ecu) {
    if(ecu->vr_count == ECU_VR_COUNT_RESET) {
        ecu->vr_count = 0;
    } else {
        ecu->vr_count++;
    }
}
