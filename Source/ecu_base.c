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

void ecu_crank_counter(vr_count_t* vr_struct) {
    if(vr_struct->count == ECU_VR_COUNT_RESET) {
        vr_struct->count = 0;
    } else {
        vr_struct->count++;
    }
    uint8_t vr_count = vr_struct->count;
    vr_struct->prev_2 = ecu_crank_vr_numb_normalization(vr_count - 2); //prev->prev
    vr_struct->prev_1 = ecu_crank_vr_numb_normalization(vr_count - 1); //prev
    vr_struct->next_1 = ecu_crank_vr_numb_normalization(vr_count + 1); //next
    vr_struct->next_2 = ecu_crank_vr_numb_normalization(vr_count + 2); //next->next
}

void ecu_main_irq_handler(ecu_t* ecu) {
    timer_ch_it_handler(&ecu->cap_ch);
    timer_ch_it_handler(&ecu->ovf_cap_ch);
}
