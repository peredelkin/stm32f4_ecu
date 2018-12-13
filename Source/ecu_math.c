#include "ecu_math.h"

uint16_t ecu_crank_period_calc(ecu_t* ecu,uint16_t capture) {
    uint8_t prev_1 = ecu_crank_vr_numb_normalization(ecu->vr_count - 1);
    uint16_t last_capture = ecu_crank_capture_read(ecu, prev_1);
    return (capture - last_capture);
}

/**
 * Расчет следующего периода
 * @param ecu Структура эбу
 * @param next_numb Следующий номер захвата
 * @param period Пройденный период
 * @param period_angles Количество углов в пройденном периоде
 * @param next_period_angles Количество углов в следующем периоде
 */
void crank_extrapolation_period_calc(ecu_t* ecu, uint8_t next_numb, uint16_t period, uint16_t period_angles, uint16_t next_period_angles) {
    ecu->crank.period[next_numb] = (period * (next_period_angles / 2)) / (period_angles / 2); // "/2" - проблемы с математикой
}

void crank_extrapolation_next_1_period(ecu_t* ecu,uint8_t prev_1,uint8_t vr_count,uint8_t next_1) {
    //расчет текущего периода
    crank_extrapolation_period_calc(ecu, next_1, ecu->crank.period[vr_count],
            ecu->crank.angle[vr_count] - ecu->crank.angle[prev_1],
            ecu->crank.angle[next_1] - ecu->crank.angle[vr_count]);
}

void crank_extrapolation_next_1_capture(ecu_t* ecu,uint8_t vr_count,uint8_t next_1) {
    //расчет следующего захвата
    ecu->crank.capture[next_1] = ecu->crank.capture[vr_count] + ecu->crank.period[next_1];
}

void crank_extrapolation_next_2_period(ecu_t* ecu,uint8_t vr_count,uint8_t next_1,uint8_t next_2) {
    //расчет следующего периода
    crank_extrapolation_period_calc(ecu, next_2, ecu->crank.period[next_1],
            ecu->crank.angle[next_1] - ecu->crank.angle[vr_count],
            ecu->crank.angle[next_2] - ecu->crank.angle[next_1]);
}

void crank_extrapolation_next_2_capture(ecu_t* ecu,uint8_t next_1,uint8_t next_2) {
    //расчет захвата следующего периода
    ecu->crank.capture[next_2] = ecu->crank.capture[next_1] + ecu->crank.period[next_2];
}

//расчет экстраполяции в точках vr_count + 1 и vr_count + 2
void crank_extrapolation_calc(ecu_t *ecu) {
    if (ecu->gap_found) {
        uint8_t vr_count = ecu->vr_count;
        uint8_t prev_1 = ecu_crank_vr_numb_normalization(vr_count - 1); //получение предыдущего номера
        uint8_t next_1 = ecu_crank_vr_numb_normalization(vr_count + 1); //получение следующего номера
        uint8_t next_2 = ecu_crank_vr_numb_normalization(vr_count + 2); //получение следующего за следующим номером
        crank_extrapolation_next_1_period(ecu,prev_1,vr_count,next_1); //период next_1
        crank_extrapolation_next_1_capture(ecu,vr_count,next_1); //захват next_1
        crank_extrapolation_next_2_period(ecu,vr_count,next_1,next_2); //период next_2
    }
}


//интерполяция углов зажигания в периоде
void ecu_coil_interpolation_calc(coil_event_t* action, uint16_t angle, uint16_t capture, uint16_t next_period, uint16_t next_angle_period) {
    uint16_t ccr = (next_period * (((uint16_t) (action->angle - angle)) / 2)) / (next_angle_period / 2);
    timer_ch_ccr_write(&action->event_ch, ccr + capture);
    timer_ch_it_enable(&action->event_ch, true);
}
