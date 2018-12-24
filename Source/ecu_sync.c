#include "ecu_sync.h"

/**
 * Запись углов в буфер в момент синхронизации
 * @param ecu Структура эбу
 */
void ecu_crank_angle_sync(ecu_t* ecu) {
    uint8_t count = ECU_VR_COUNT; //количество элементов таблицы захвата
    uint8_t buffer = ecu->vr.sync_point; //начало синхронизации таблицы захвата
    uint8_t tooth = ECU_VR_SYNC_POINT; //начало синхронизации таблицы углов
    while (count) {
        count--;
        ecu->crank.angle[buffer] =
                crank_tooth_angle[tooth];
        //счет буфера
        if (buffer == ECU_VR_COUNT_RESET) {
            buffer = 0;
        } else {
            buffer++;
        }
        //счет зубов
        if (tooth == ECU_VR_COUNT_RESET) {
            tooth = 0;
        } else {
            tooth++;
        }
    }
}

/**
 * Поиск метки и синхронизация углов
 * @param ecu структура эбу
 * @param tnbm2_w Период N-2
 * @param tnbm1_w Приод N-1
 * @param tnbm_w Период N
 */
void ecu_crank_gap_search(ecu_t* ecu,uint16_t tnbm2_w, uint16_t tnbm1_w, uint16_t tnbm_w) {
    if (ecu->gap_found == false) { //если метка не найдена
        if (ecu->cap_time_norm) { //если скорость вращения выше минимальной
            if (crank_gap_correct_check(tnbm2_w, tnbm1_w, tnbm_w)) {
                ecu->vr.sync_point = ecu->vr.count; //сохранение точки синхронизации
                ecu_crank_angle_sync(ecu); //синхронизация таблицы углов
                ecu->gap_found = true; //метка найдена
            }
        } else {
            ecu->gap_found = false; //метка не найдена,если скорость ниже минимальной
        }
    }
}

/**
 * Проверка метки в точке синхронизации
 * @param ecu Структура эбу
 * @param tnbm2_w Периода [57] или N-2
 * @param tnbm1_w Период [0] или N-1
 * @param tnbm_w Период [1] или N
 */
void ecu_crank_gap_check(ecu_t* ecu,uint16_t tnbm2_w, uint16_t tnbm1_w, uint16_t tnbm_w) {
    if (ecu->gap_found) { //если метка найдена
        if (ecu->vr.count == ecu->vr.sync_point) { //если точка синхронизации достигнута
            if (crank_gap_correct_check(tnbm2_w, tnbm1_w, tnbm_w)) {
                ecu->gap_correct = true; //метка верна
            } else {
                ecu->gap_correct = false; //метка не верна
                ecu->gap_found = false; //новый поиск метки
            }
        }
    } else {
        ecu->gap_correct = false; //метка не верна если скорость ниже минимальной
    }
}

/**
 * Проверка времени захвата
 * @param ecu Структура эбу
 * @param tnbm2_w Период N-2
 * @param tnbm1_w Период N-1
 * @param tnbm_w Период N
 */
void ecu_crank_min_time_check(ecu_t* ecu,uint16_t tnbm2_w, uint16_t tnbm1_w, uint16_t tnbm_w) {
    if(ecu_crank_min_time_set(ECU_MAX_TOOTH_TIME,tnbm2_w)) {
        ecu->cap_time_norm = true; //скорость выше минимальной
    } else {
        if(ecu_crank_min_time_reset(ECU_MAX_TOOTH_TIME,tnbm1_w,tnbm_w)) {
            ecu->cap_time_norm = false; //скорость ниже минимальной
        }
    }
}

/**
 * Проверка минимального захвата,поиск метки,
 * синхронизация углов(если метка найдена),
 * проверка метки в точке синхронизации.
 * @param ecu Структура эбу
 */
void ecu_crank_sync(ecu_t* ecu) {
    uint16_t tnbm2_w = ecu_crank_period_read(ecu, ecu->vr.prev_2); //период n-2
    uint16_t tnbm1_w = ecu_crank_period_read(ecu, ecu->vr.prev_1); //период n-1
    uint16_t tnbm_w = ecu_crank_period_read(ecu, ecu->vr.count); // период n
    ecu_crank_min_time_check(ecu,tnbm2_w, tnbm1_w, tnbm_w); //проверка периода захвата
    ecu_crank_gap_search(ecu,tnbm2_w, tnbm1_w, tnbm_w); //поиск метки и синхронизация углов,если метка найдена
    ecu_crank_gap_check(ecu,tnbm2_w, tnbm1_w, tnbm_w); //проверка метки в точке синхронизации
}
