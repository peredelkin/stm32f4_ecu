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
    ecu_coil_1_4_off(NULL);
    ecu_coil_2_3_off(NULL);
}

void ECU_COIL_TIM_1_IRQHandler(void) {
    timer_ch_it_handler(&coil_set[0].event_ch);
    timer_ch_it_handler(&coil_set[1].event_ch);
    timer_ch_it_handler(&coil_set[2].event_ch);
    timer_ch_it_handler(&coil_set[3].event_ch);
}

void ECU_COIL_TIM_2_IRQHandler(void) {
    timer_ch_it_handler(&coil_reset[0].event_ch);
    timer_ch_it_handler(&coil_reset[1].event_ch);
    timer_ch_it_handler(&coil_reset[2].event_ch);
    timer_ch_it_handler(&coil_reset[3].event_ch);
}

/**
 * Проверка угла события в окне углов захвата N+1 и N+2 с заданием события,если достигнуто искомое окно
 * @param action Задание
 * @param angle Угол захвата N+1
 * @param next_angle Угол захвата N+2
 * @param capture Значение захвата N+1
 * @param next_period Период между захватами N+1 и N+2
 */
void ecu_coil_angle_check(coil_event_t** action, uint16_t angle,
        uint16_t next_angle, uint16_t capture, uint16_t next_period) {
    if (ecu_coil_window_angle_check((*action)->angle, angle, next_angle)) {
        uint16_t ccr = ecu_coil_interpolation_calc((*action)->angle, angle, capture, next_period, ((uint16_t) (next_angle - angle)));
        timer_ch_ccr_write(&(*action)->event_ch, ccr);
        timer_ch_it_enable(&(*action)->event_ch, true);
        if((*action)->next) *action = (*action)->next;
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
        ecu_coil_angle_check(&coil_set_current,angle,next_angle,capture,next_period);
        ecu_coil_angle_check(&coil_reset_current,angle,next_angle,capture,next_period);
    }
}

void ecu_coil_list_set(coil_event_t* current,coil_event_t* next) {
    current->next = next;
    next->prev = current;
}

void ecu_coil_list_init() {
    ecu_coil_list_set(&coil_set[0],&coil_set[1]);
    ecu_coil_list_set(&coil_set[1],&coil_set[2]);
    ecu_coil_list_set(&coil_set[2],&coil_set[3]);
    ecu_coil_list_set(&coil_set[3],&coil_set[0]);
    
    ecu_coil_list_set(&coil_reset[0],&coil_reset[1]);
    ecu_coil_list_set(&coil_reset[1],&coil_reset[2]);
    ecu_coil_list_set(&coil_reset[2],&coil_reset[3]);
    ecu_coil_list_set(&coil_reset[3],&coil_reset[0]);
}

void ecu_coil_init(void) {
    ecu_coil_slave_timer_1_init();
    ecu_coil_slave_timer_2_init();
    
    coil_set_current = coil_set;
    coil_reset_current = coil_reset;
    
    coil_set[0].angle = 0;
    coil_reset[0].angle = 1092;
    
    coil_set[1].angle = 65536/2;
    coil_reset[1].angle = (65536/2) + 1092;
    
    coil_set[2].angle = 0;
    coil_reset[2].angle = 1092;
    
    coil_set[3].angle = 65536/2;
    coil_reset[3].angle = (65536/2) + 1092;
    
    ecu_coil_list_init();

    //set
    make_timer_ch_it_init(&coil_set[0].event_ch, ECU_COIL_TIM_1, 1);
    timer_ch_event_set(&coil_set[0].event_ch, &ecu_coil_1_4_on);

    make_timer_ch_it_init(&coil_set[1].event_ch, ECU_COIL_TIM_1, 2);
    timer_ch_event_set(&coil_set[1].event_ch, &ecu_coil_2_3_on);

    make_timer_ch_it_init(&coil_set[2].event_ch, ECU_COIL_TIM_1, 3);
    timer_ch_event_set(&coil_set[2].event_ch, &ecu_coil_1_4_on);

    make_timer_ch_it_init(&coil_set[3].event_ch, ECU_COIL_TIM_1, 4);
    timer_ch_event_set(&coil_set[3].event_ch, &ecu_coil_2_3_on);
    
    //reset
    make_timer_ch_it_init(&coil_reset[0].event_ch, ECU_COIL_TIM_2, 1);
    timer_ch_event_set(&coil_reset[0].event_ch, &ecu_coil_1_4_off);

    make_timer_ch_it_init(&coil_reset[1].event_ch, ECU_COIL_TIM_2, 2);
    timer_ch_event_set(&coil_reset[1].event_ch, &ecu_coil_2_3_off);

    make_timer_ch_it_init(&coil_reset[2].event_ch, ECU_COIL_TIM_2, 3);
    timer_ch_event_set(&coil_reset[2].event_ch, &ecu_coil_1_4_off);

    make_timer_ch_it_init(&coil_reset[3].event_ch, ECU_COIL_TIM_2, 4);
    timer_ch_event_set(&coil_reset[3].event_ch, &ecu_coil_2_3_off);
}
