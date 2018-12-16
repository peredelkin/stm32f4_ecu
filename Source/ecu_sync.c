#include "ecu_sync.h"

//запись углов в буфер в момент синхронизации

void ecu_crank_angle_sync(ecu_t* ecu) {
    uint8_t count = ECU_VR_COUNT;
    uint8_t buffer = ecu->vr.sync_point;
    uint8_t tooth = ECU_VR_SYNC_POINT;
    while (count) {
        count--;
        ecu->crank.angle[buffer] =
                crank_tooth_angle[tooth];

        if (buffer == ECU_VR_COUNT_RESET) {
            buffer = 0;
        } else {
            buffer++;
        }
        
        if (tooth == ECU_VR_COUNT_RESET) {
            tooth = 0;
        } else {
            tooth++;
        }
    }
}

//поиск метки и синхронизация углов

void ecu_crank_gap_search(ecu_t* ecu) {
    if (ecu->gap_found == false) {
        uint16_t tnbm2_w = ecu_crank_period_read(ecu, ecu->vr.prev_2);
        uint16_t tnbm1_w = ecu_crank_period_read(ecu, ecu->vr.prev_1);
        uint16_t tnbm_w = ecu_crank_period_read(ecu, ecu->vr.count);
        if (crank_gap_correct_check(tnbm2_w, tnbm1_w, tnbm_w)) {
            ecu->vr.sync_point = ecu->vr.count;
            ecu_crank_angle_sync(ecu);
            ecu->gap_found = true;
        }
    }
}

//проверка метки в точке синхронизации

void ecu_crank_gap_check(ecu_t* ecu) {
    if ((ecu->vr.count == ecu->vr.sync_point) && ecu->gap_found) {
        uint16_t tnbm2_w = ecu_crank_period_read(ecu, ecu->vr.prev_2);
        uint16_t tnbm1_w = ecu_crank_period_read(ecu, ecu->vr.prev_1);
        uint16_t tnbm_w = ecu_crank_period_read(ecu, ecu->vr.count);
        if (crank_gap_correct_check(tnbm2_w, tnbm1_w, tnbm_w)) {
            ecu->gap_correct = true; //метка верна
        } else {
            ecu->gap_correct = false; //метка не верна
            ecu->gap_found = false; //новый поиск метки
        }
    }
}

//проверка времени захвата
void ecu_crank_min_time_check(ecu_t* ecu) {
    uint16_t tnbm2_w = ecu_crank_period_read(ecu, ecu->vr.prev_2);
    uint16_t tnbm1_w = ecu_crank_period_read(ecu, ecu->vr.prev_1);
    uint16_t tnbm_w = ecu_crank_period_read(ecu, ecu->vr.count);
    if(ecu_crank_min_time_reset(ECU_MAX_TOOTH_TIME,tnbm1_w,tnbm_w)) {
        ecu->cap_time_norm = true;
    } else {
        if(ecu_crank_min_time_set(ECU_MAX_TOOTH_TIME,tnbm2_w)) {
            ecu->cap_time_norm = false;
            ecu->gap_correct = false; //(!)
            ecu->gap_found = false; //(!)
        }
    }
}
