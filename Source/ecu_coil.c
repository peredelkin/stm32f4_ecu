#include "ecu_coil.h"
#include "ecu_math.h"
#include "ecu_compare.h"

void ecu_coil_slave_timer_1_init() {
    //	NVIC_SetPriority(TIM3_IRQn, 34); //36 default
    NVIC_EnableIRQ(ECU_COIL_TIM_1_IRQn); //Compare

    ECU_COIL_TIM_1->PSC = (uint16_t) (84 - 1); // Prescaler

    ECU_COIL_TIM_1->EGR = TIM_EGR_UG; // Re-initialize

    //=====================Slave=============================

    ECU_COIL_TIM_1->SMCR |= TIM_SMCR_MSM; // For better Sync (?)

    ECU_COIL_TIM_1->SMCR |= (TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0); // Slave 101 Gated Mode

    ECU_COIL_TIM_1->SMCR &= ~TIM_SMCR_TS; // ITR0

    ECU_COIL_TIM_1->CR1 |= TIM_CR1_CEN; //Need Enable in Slave
    //===================Slave End========================
}

void ecu_coil_slave_timer_2_init() {
    //	NVIC_SetPriority(TIM3_IRQn, 34); //37 default
    NVIC_EnableIRQ(ECU_COIL_TIM_2_IRQn); //Compare

    ECU_COIL_TIM_2->PSC = (uint16_t) (84 - 1); // Prescaler

    ECU_COIL_TIM_2->EGR = TIM_EGR_UG; // Re-initialize

    //=====================Slave=============================

    ECU_COIL_TIM_2->SMCR |= TIM_SMCR_MSM; // For better Sync (?)

    ECU_COIL_TIM_2->SMCR |= (TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0); // Slave 101 Gated Mode

    ECU_COIL_TIM_2->SMCR &= ~TIM_SMCR_TS; // ITR0

    ECU_COIL_TIM_2->CR1 |= TIM_CR1_CEN; //Need Enable in Slave
    //===================Slave End========================
}

void ecu_coil_1_4_on(void* channel) {
    GPIOD->BSRRL = GPIO_ODR_ODR_13; //оранжевый
}

void ecu_coil_1_4_off(void* channel) {
    GPIOD->BSRRH = GPIO_ODR_ODR_13; //оранжевый
}

void ecu_coil_2_3_on(void* channel) {
    GPIOD->BSRRL = GPIO_ODR_ODR_14; //красный
}

void ecu_coil_2_3_off(void* channel) {
    GPIOD->BSRRH = GPIO_ODR_ODR_14; //красный
}

void ECU_COIL_TIM_1_IRQHandler(void) {
    timer_ch_it_handler(&coil_1_4.set.event_ch);
    timer_ch_it_handler(&coil_1_4.reset.event_ch);
    timer_ch_it_handler(&coil_2_3.set.event_ch);
    timer_ch_it_handler(&coil_2_3.reset.event_ch);
}

void ecu_coil_angle_check(coil_event_t* action, uint16_t angle, uint16_t next_angle, uint16_t capture, uint16_t next_period) {
    if (ecu_coil_window_angle_check(action->angle,angle,next_angle)) {
        ecu_coil_interpolation_calc(action, angle, capture, next_period, ((uint16_t) (next_angle - angle)));
    }
}

void ecu_coil_angle_update(coil_event_t* action, uint16_t angle, uint16_t next_angle) {
    if (ecu_coil_window_angle_check(action->angle_s,angle,next_angle)) {
        action->angle = action->angle_s;
    }
}

void ecu_coil_handler(ecu_t* ecu) {
    if (ecu->gap_correct) {
        uint8_t next_1 = ecu_crank_vr_numb_normalization(ecu->vr_count + 1);
        uint8_t next_2 = ecu_crank_vr_numb_normalization(ecu->vr_count + 2);

        uint16_t angle = ecu->crank.angle[next_1];
        uint16_t next_angle = ecu->crank.angle[next_2];
        uint16_t capture = ecu->crank.capture[next_1];
        uint16_t next_period = ecu->crank.period[next_2];

        ecu_coil_angle_check(&coil_1_4.set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&coil_1_4.reset, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&coil_2_3.set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&coil_2_3.reset, angle, next_angle, capture, next_period);

        ecu_coil_angle_update(&coil_1_4.set, angle, next_angle);
        ecu_coil_angle_update(&coil_1_4.reset, angle, next_angle);
        ecu_coil_angle_update(&coil_2_3.set, angle, next_angle);
        ecu_coil_angle_update(&coil_2_3.reset, angle, next_angle);

        //test begin
        coil_1_4.set.angle_s++; 
        coil_1_4.reset.angle_s++;

        coil_2_3.set.angle_s++;
        coil_2_3.reset.angle_s++;
    }
}

void ecu_coil_init(void) {
    ecu_coil_slave_timer_1_init();
    //    ecu_coil_slave_timer_2_init();

    coil_1_4.set.angle_s = 0;
    coil_1_4.reset.angle_s = 1092*3;

    coil_2_3.set.angle_s = 0;
    coil_2_3.reset.angle_s = 1092*3;

    make_timer_ch_it_init(&coil_1_4.set.event_ch, ECU_COIL_TIM_1, 1);
    timer_ch_event_set(&coil_1_4.set.event_ch, &ecu_coil_1_4_on);

    make_timer_ch_it_init(&coil_1_4.reset.event_ch, ECU_COIL_TIM_1, 2);
    timer_ch_event_set(&coil_1_4.reset.event_ch, &ecu_coil_1_4_off);

    make_timer_ch_it_init(&coil_2_3.set.event_ch, ECU_COIL_TIM_1, 3);
    timer_ch_event_set(&coil_2_3.set.event_ch, &ecu_coil_2_3_on);

    make_timer_ch_it_init(&coil_2_3.reset.event_ch, ECU_COIL_TIM_1, 4);
    timer_ch_event_set(&coil_2_3.reset.event_ch, &ecu_coil_2_3_off);
}
