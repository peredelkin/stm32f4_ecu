#include "ecu_coil.h"
#include "ecu_math.h"
#include "ecu_compare.h"
#include <stddef.h>

//\todo Переписать обновление углов.

void ecu_coil_slave_timer_1_init() {
    NVIC_SetPriority(TIM3_IRQn, 5); //36 default
    NVIC_EnableIRQ(ECU_COIL_TIM_1_IRQn); //Compare

    ECU_COIL_TIM_1->PSC = (uint16_t) (84 - 1); // Prescaler

    ECU_COIL_TIM_1->EGR = TIM_EGR_UG; // Re-initialize

    //=====================Slave=============================

    ECU_COIL_TIM_1->SMCR |= TIM_SMCR_MSM; // For better Sync (?)

    ECU_COIL_TIM_1->SMCR |= (TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0); // Slave 101 Gated Mode

    ECU_COIL_TIM_1->SMCR &= ~TIM_SMCR_TS; // ITR0
    //===================Slave End========================
}

void ecu_coil_slave_timer_2_init() {
    NVIC_SetPriority(TIM3_IRQn, 5); //37 default
    NVIC_EnableIRQ(ECU_COIL_TIM_2_IRQn); //Compare

    ECU_COIL_TIM_2->PSC = (uint16_t) (84 - 1); // Prescaler

    ECU_COIL_TIM_2->EGR = TIM_EGR_UG; // Re-initialize

    //=====================Slave=============================

    ECU_COIL_TIM_2->SMCR |= TIM_SMCR_MSM; // For better Sync (?)

    ECU_COIL_TIM_2->SMCR |= (TIM_SMCR_SMS_2 | TIM_SMCR_SMS_0); // Slave 101 Gated Mode

    ECU_COIL_TIM_2->SMCR &= ~TIM_SMCR_TS; // ITR0
    //===================Slave End========================
}

void ecu_coil_0_on(void* channel) {
    COIL_0_GPIO->BSRRL = COIL_0_BSRR_MASK; //оранжевый
}
void ecu_coil_0_off(void* channel) {
    COIL_0_GPIO->BSRRH = COIL_0_BSRR_MASK; //оранжевый
}

void ecu_coil_1_on(void* channel) {
    COIL_1_GPIO->BSRRL = COIL_1_BSRR_MASK; //красный
}
void ecu_coil_1_off(void* channel) {
    COIL_1_GPIO->BSRRH = COIL_1_BSRR_MASK; //красный
}

void ecu_coil_2_on(void* channel) {
    COIL_2_GPIO->BSRRL = COIL_2_BSRR_MASK; //синий
}
void ecu_coil_2_off(void* channel) {
    COIL_2_GPIO->BSRRH = COIL_2_BSRR_MASK; //синий
}

void ecu_coil_3_on(void * channel) {
    COIL_3_GPIO->BSRRL = COIL_3_BSRR_MASK; //зеленый
}
void ecu_coil_3_off(void * channel) {
    COIL_3_GPIO->BSRRH = COIL_3_BSRR_MASK; //зеленый
}

void ecu_all_coil_reset(void) {
    ecu_coil_0_off(NULL);
    ecu_coil_1_off(NULL);
    ecu_coil_2_off(NULL);
    ecu_coil_3_off(NULL);
}

void ECU_COIL_TIM_1_IRQHandler(void) {
    timer_ch_it_handler(&ign_coil[0].set.event_ch);
    timer_ch_it_handler(&ign_coil[0].reset.event_ch);
    timer_ch_it_handler(&ign_coil[1].set.event_ch);
    timer_ch_it_handler(&ign_coil[1].reset.event_ch);
}

void ECU_COIL_TIM_2_IRQHandler(void) {
    timer_ch_it_handler(&ign_coil[2].set.event_ch);
    timer_ch_it_handler(&ign_coil[2].reset.event_ch);
    timer_ch_it_handler(&ign_coil[3].set.event_ch);
    timer_ch_it_handler(&ign_coil[3].reset.event_ch);
}

/**
 * Проверка угла события в окне углов захвата N+1 и N+2 с заданием события,если достигнуто искомое окно
 * @param action Задание
 * @param angle Угол захвата N+1
 * @param next_angle Угол захвата N+2
 * @param capture Значение захвата N+1
 * @param next_period Период между захватами N+1 и N+2
 */
void ecu_coil_angle_check(coil_event_t* action, uint16_t angle,
        uint16_t next_angle, uint16_t capture, uint16_t next_period) {
    if (ecu_coil_window_angle_check(action->angle, angle, next_angle)) {
        uint16_t ccr = ecu_coil_interpolation_calc(action->angle, angle, capture, next_period, ((uint16_t) (next_angle - angle)));
        timer_ch_ccr_write(&action->event_ch, ccr);
        timer_ch_it_enable(&action->event_ch, true);
        action->update = true;
    }
}

void ecu_coil_angle_update(ecu_t* ecu, coil_t* coil) {
    if ((coil->set.update) && (coil->reset.update)) {
        coil->set.update = false;
        coil->reset.update = false;
    }
    
    if (coil->set.update == false) {
        uint16_t reset_angle = ecu->ignition.angle + coil->offset_angle;
        uint16_t set_angle = reset_angle - ecu->ignition.dwell_angle;
        if(((int16_t)(set_angle - ecu->crank.angle[ecu->vr.next_2]) > 0) && ((int16_t)(ecu->crank.angle[ecu->vr.next_1] - reset_angle) > 0)) {
            coil->reset.angle = reset_angle;
            coil->set.angle = set_angle;
        }
    }
}

void ecu_coil_handler(ecu_t* ecu) {
    if (ecu->gap_correct) {
        //угол захвата N+1
        uint16_t angle = ecu->crank.angle[ecu->vr.next_1];
        //угол захвата N+2
        uint16_t next_angle = ecu->crank.angle[ecu->vr.next_2];
        //значение захвата N+1
        uint16_t capture = ecu->crank.capture[ecu->vr.next_1];
        //период захвата N+2
        uint16_t next_period = ecu->crank.period[ecu->vr.next_2];

        ecu->ignition.angle++;
        ecu->ignition.dwell_angle = ecu_coil_delta_angle_calc(ecu, ecu->vr.prev_1, ecu->vr.count, 5000);

        //проверка углов с запуском событий
        ecu_coil_angle_check(&ign_coil[0].set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&ign_coil[0].reset, angle, next_angle, capture, next_period);
        ecu_coil_angle_update(ecu, &ign_coil[0]);

        ecu_coil_angle_check(&ign_coil[1].set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&ign_coil[1].reset, angle, next_angle, capture, next_period);
        ecu_coil_angle_update(ecu, &ign_coil[1]);

        ecu_coil_angle_check(&ign_coil[2].set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&ign_coil[2].reset, angle, next_angle, capture, next_period);
        ecu_coil_angle_update(ecu, &ign_coil[2]);

        ecu_coil_angle_check(&ign_coil[3].set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&ign_coil[3].reset, angle, next_angle, capture, next_period);
        ecu_coil_angle_update(ecu, &ign_coil[3]);
    }
}

void ecu_ign_coil_angle_init(ecu_t* ecu,coil_t* coil,uint16_t offset_angle) {
    coil->offset_angle = offset_angle;
    coil->reset.angle = ecu->ignition.angle + coil->offset_angle;
    coil->set.angle = coil->reset.angle - ecu->ignition.dwell_angle;
}

void ecu_coil_init(ecu_t* ecu) {
    ecu_coil_slave_timer_1_init();
    ecu_coil_slave_timer_2_init();
    
    ecu->ignition.dwell_angle = 1092;
    
    ecu_ign_coil_angle_init(ecu,&ign_coil[0],0);
    ecu_ign_coil_angle_init(ecu,&ign_coil[1],16384);
    ecu_ign_coil_angle_init(ecu,&ign_coil[2],32768);
    ecu_ign_coil_angle_init(ecu,&ign_coil[3],49152);

    //set 0
    make_timer_ch_it_init(&ign_coil[0].set.event_ch, ECU_COIL_TIM_1, 1);
    timer_ch_event_set(&ign_coil[0].set.event_ch, &ecu_coil_0_on);
    //reset 0
    make_timer_ch_it_init(&ign_coil[0].reset.event_ch, ECU_COIL_TIM_1, 2);
    timer_ch_event_set(&ign_coil[0].reset.event_ch, &ecu_coil_0_off);
    
    //set 1
    make_timer_ch_it_init(&ign_coil[1].set.event_ch, ECU_COIL_TIM_1, 3);
    timer_ch_event_set(&ign_coil[1].set.event_ch, &ecu_coil_1_on);
    //reset 1
    make_timer_ch_it_init(&ign_coil[1].reset.event_ch, ECU_COIL_TIM_1, 4);
    timer_ch_event_set(&ign_coil[1].reset.event_ch, &ecu_coil_1_off);
    
    //set 2
    make_timer_ch_it_init(&ign_coil[2].set.event_ch, ECU_COIL_TIM_2, 1);
    timer_ch_event_set(&ign_coil[2].set.event_ch, &ecu_coil_2_on);
    //reset 2
    make_timer_ch_it_init(&ign_coil[2].reset.event_ch, ECU_COIL_TIM_2, 2);
    timer_ch_event_set(&ign_coil[2].reset.event_ch, &ecu_coil_2_off);
    
    //set 3
    make_timer_ch_it_init(&ign_coil[3].set.event_ch, ECU_COIL_TIM_2, 3);
    timer_ch_event_set(&ign_coil[3].set.event_ch, &ecu_coil_3_on);
    //reset 4
    make_timer_ch_it_init(&ign_coil[3].reset.event_ch, ECU_COIL_TIM_2, 4);
    timer_ch_event_set(&ign_coil[3].reset.event_ch, &ecu_coil_3_off);
}
