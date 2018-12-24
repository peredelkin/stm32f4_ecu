#include "ecu_math.h"

/**
 * Расчет пройденного периода
 * @param ecu Структура эбу
 * @return Пройденный период
 * \f period=capture[n]-capture[n-1]
 */
uint16_t ecu_crank_period_calc(ecu_t* ecu) {
    return (uint16_t)(ecu_crank_capture_read(ecu, ecu->vr.count)
            - ecu_crank_capture_read(ecu, ecu->vr.prev_1));
}

/**
 * Расчет угла периода
 * @param ecu Структура эбу
 * @param current Индекс n
 * @param prev Индекс n-1
 * @return Угол периода
 * \f period_angle = angle[n] - angle [n-1]
 */
uint16_t ecu_period_angle_calc(ecu_t* ecu, uint8_t current, uint8_t prev) {
    return (uint16_t)(ecu_crank_angle_read(ecu, current) - ecu_crank_angle_read(ecu, prev));
}

/**
 * Экстраполяция периода
 * @param ecu Структура эбу
 * @param next_numb Индекс следующего периода
 * @param period Пройденный период
 * @param period_angles Количество углов в пройденном периоде
 * @param next_period_angles Количество углов в следующем периоде
 * \f period[n+1] = ( period[n] * period_angles[n+1] ) / period_angles[n]
 * @bug Ошибка расчета в пределах 9 единиц. Возможно переполнение при расчете?
 * @todo Исправить формулу расчета
 */
void crank_extrapolation_period_calc(ecu_t* ecu, uint8_t next_numb,
        uint16_t period, uint16_t period_angles, uint16_t next_period_angles) {
    ecu_crank_period_write(ecu, next_numb, (period * (next_period_angles / 2)) / (period_angles / 2)); // "/2" - проблемы с математикой
}

/**
 * Экстраполяция периода n+1
 * @param ecu Структра эбу
 */
void crank_extrapolation_next_1_period(ecu_t* ecu) {
    crank_extrapolation_period_calc(ecu, ecu->vr.next_1, 
            ecu_crank_period_read(ecu,ecu->vr.count),
            ecu_period_angle_calc(ecu,ecu->vr.count,ecu->vr.prev_1),
            ecu_period_angle_calc(ecu,ecu->vr.next_1,ecu->vr.count));
}

/**
 * Расчет захвата n+1
 * @param ecu
 * \f capture[n+1] = capture[n] + period[n+1]
 */
void crank_extrapolation_next_1_capture(ecu_t* ecu) {
    ecu_crank_capture_write(ecu,ecu->vr.next_1,(ecu_crank_capture_read(ecu,ecu->vr.count)
            + ecu_crank_period_read(ecu,ecu->vr.next_1)));
}

/**
 * Экстраполяция периода n+2
 * @param ecu Структура эбу
 */
void crank_extrapolation_next_2_period(ecu_t* ecu) {
    crank_extrapolation_period_calc(ecu, ecu->vr.next_2, 
            ecu_crank_period_read(ecu,ecu->vr.next_1),
            ecu_period_angle_calc(ecu,ecu->vr.next_1,ecu->vr.count),
            ecu_period_angle_calc(ecu,ecu->vr.next_2,ecu->vr.next_1));
}

/**
 * Расчет захвата n+2
 * @param ecu Структура эбу
 * capture[n+2] = capture[n+1] + period[n+2]
 */
void crank_extrapolation_next_2_capture(ecu_t* ecu) {
    ecu_crank_capture_write(ecu,ecu->vr.next_2,
            (ecu_crank_capture_read(ecu,ecu->vr.next_1) 
            + ecu_crank_period_read(ecu,ecu->vr.next_2)));
}

/**
 * Экстраполяция захвата и периода n+1, и периода n+2
 * @param ecu
 */
void crank_extrapolation_calc(ecu_t *ecu) {
    if (ecu->gap_found) {
        crank_extrapolation_next_1_period(ecu); //период next_1
        crank_extrapolation_next_1_capture(ecu); //захват next_1
        crank_extrapolation_next_2_period(ecu); //период next_2
    }
}

/**
 * Расчет CCR с интерполяцией
 * @param action_angle Угол задачи
 * @param angle Угол
 * @param capture Захват
 * @param next_period Следующий период
 * @param next_angle_period Количество углов в следующем периоде
 * @return Значение регистра сравнения (CCR)
 * \f CCR = ((period[n+1] * (coil_angle - angle[n])) / period[n+1]) + capture[n]
 */
uint16_t ecu_coil_interpolation_calc(uint16_t action_angle,uint16_t angle, uint16_t capture, uint16_t next_period, uint16_t next_angle_period) {
    return (((next_period * (((uint16_t) (action_angle - angle)) / 2)) / (next_angle_period / 2)) + capture);
}

//расчет угла между set и reset от времени накопления TODO: переписать

uint16_t ecu_coil_set_angle_calc(ecu_t* ecu, uint8_t prev_1, uint8_t vr_count, uint16_t reset_angle, uint16_t time) {
    return (uint16_t)(reset_angle - ((time * (uint16_t)(ecu->crank.angle[vr_count] - ecu->crank.angle[prev_1])) / ecu->crank.period[vr_count]));
}
