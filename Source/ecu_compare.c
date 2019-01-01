#include "ecu_compare.h"

//возвращает true,если метка верна
bool crank_gap_correct_check(uint16_t tnbm2_w, uint16_t tnbm1_w, uint16_t tnbm_w) {
	return ((tnbm2_w < (tnbm1_w / 2)) && ((tnbm1_w / 2) > tnbm_w));
}
//возвращает true,если один зуб лишний
bool crank_gap_one_to_much_check(uint16_t tnbm1_w, uint16_t tnbm_w) {
	return (tnbm1_w < (tnbm_w / 2));
}
//возвращает true,если один зуб пропущен
bool crank_gap_one_missing_check(uint16_t tnbm2_w, uint16_t tnbm1_w) {
	return ((tnbm2_w / 2) > tnbm1_w);
}

//возвращает true,если искомый углол в заданном окне
bool ecu_coil_window_angle_check(uint16_t check_angle,uint16_t angle, uint16_t next_angle) {
    return ((angle <= check_angle) && (check_angle <= (uint16_t) (next_angle - 1)));
}

//возвращает true,если обороты выше минимальных
bool ecu_crank_min_time_set(uint16_t min_time,uint16_t tnbm2_w) {
    return (tnbm2_w < min_time);
}
//возвращает true,если ниже минимальных
bool ecu_crank_min_time_reset(uint16_t min_time,uint16_t tnbm1_w,uint16_t tnbm_w) {
    return ((tnbm1_w > min_time) && (tnbm_w > min_time));
}
//возвращает true,если углы события не в окне поиска
bool ecu_coil_update_window_angle_check(uint16_t set_angle,uint16_t reset_angle,uint16_t angle,uint16_t next_angle) {
    return (((int16_t)(set_angle - next_angle) > 0) && ((int16_t)(angle - reset_angle) > 0));
}
