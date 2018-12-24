#include "ecu_coil.h"
#include "ecu_math.h"
#include "ecu_compare.h"
#include <stddef.h>
#include "mutex.h"

mutex_t ecu_coil_tim_1_mutex;

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

    ECU_COIL_TIM_1->CR1 |= TIM_CR1_CEN; //Need Enable in Slave
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

void ecu_all_coil_reset(void) {
    timer_ch_it_disable(&coil_1_4.set.event_ch);
    timer_ch_it_disable(&coil_1_4.reset.event_ch);
    timer_ch_it_disable(&coil_2_3.set.event_ch);
    timer_ch_it_disable(&coil_2_3.reset.event_ch);
    ecu_coil_1_4_off(NULL);
    ecu_coil_2_3_off(NULL);
}

void ECU_COIL_TIM_1_IRQHandler(void) {
    mutex_lock(&ecu_coil_tim_1_mutex);
    timer_ch_it_handler(&coil_1_4.set.event_ch);
    timer_ch_it_handler(&coil_1_4.reset.event_ch);
    timer_ch_it_handler(&coil_2_3.set.event_ch);
    timer_ch_it_handler(&coil_2_3.reset.event_ch);
    mutex_unlock(&ecu_coil_tim_1_mutex);
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
    if (ecu_coil_window_angle_check(action->current.angle, angle, next_angle)) {
        uint16_t ccr = ecu_coil_interpolation_calc(action->current.angle, angle, capture, next_period, ((uint16_t) (next_angle - angle)));
        
        mutex_lock(&ecu_coil_tim_1_mutex);
        
        timer_ch_ccr_write(&action->event_ch, ccr);
        timer_ch_it_enable(&action->event_ch, true);
        
        mutex_unlock(&ecu_coil_tim_1_mutex);
        
        action->current.angle = action->next.angle;
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

        //проверка углов с запуском событий
        ecu_coil_angle_check(&coil_1_4.set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&coil_1_4.reset, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&coil_2_3.set, angle, next_angle, capture, next_period);
        ecu_coil_angle_check(&coil_2_3.reset, angle, next_angle, capture, next_period);

        //test
        //блокировка изменения углов
        if ((coil_1_4.reset.current.angle == coil_1_4.reset.next.angle) && (coil_1_4.set.current.angle == coil_1_4.set.next.angle)) {
            coil_1_4.reset.next.angle -=1092;
            coil_1_4.set.next.angle = ecu_coil_set_angle_calc(ecu, ecu->vr.prev_1, ecu->vr.count, coil_1_4.reset.next.angle, 3500); //
        }
        if ((coil_2_3.reset.current.angle == coil_2_3.reset.next.angle) && (coil_2_3.set.current.angle == coil_2_3.set.next.angle)) {
            coil_2_3.reset.next.angle -=1092;
            coil_2_3.set.next.angle = ecu_coil_set_angle_calc(ecu, ecu->vr.prev_1, ecu->vr.count, coil_2_3.reset.next.angle, 3500); //
        }
        //end test
    }
}

void ecu_coil_init(void) {
    ecu_coil_slave_timer_1_init();
    //    ecu_coil_slave_timer_2_init();

    coil_1_4.set.next.angle = 0;
    coil_1_4.reset.next.angle = (1092 * 3)+10;

    coil_2_3.set.next.angle = 0;
    coil_2_3.reset.next.angle = 1092 * 3;

    make_timer_ch_it_init(&coil_1_4.set.event_ch, ECU_COIL_TIM_1, 1);
    timer_ch_event_set(&coil_1_4.set.event_ch, &ecu_coil_1_4_on);

    make_timer_ch_it_init(&coil_1_4.reset.event_ch, ECU_COIL_TIM_1, 2);
    timer_ch_event_set(&coil_1_4.reset.event_ch, &ecu_coil_1_4_off);

    make_timer_ch_it_init(&coil_2_3.set.event_ch, ECU_COIL_TIM_1, 3);
    timer_ch_event_set(&coil_2_3.set.event_ch, &ecu_coil_2_3_on);

    make_timer_ch_it_init(&coil_2_3.reset.event_ch, ECU_COIL_TIM_1, 4);
    timer_ch_event_set(&coil_2_3.reset.event_ch, &ecu_coil_2_3_off);
}
