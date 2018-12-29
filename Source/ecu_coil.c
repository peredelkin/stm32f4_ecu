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

void ecu_coil_0_on(void* channel) {
    GPIOD->BSRRL = GPIO_ODR_ODR_13; //оранжевый
}

void ecu_coil_0_off(void* channel) {
    GPIOD->BSRRH = GPIO_ODR_ODR_13; //оранжевый
}

void ecu_coil_1_on(void* channel) {
    GPIOD->BSRRL = GPIO_ODR_ODR_14; //красный
}

void ecu_coil_1_off(void* channel) {
    GPIOD->BSRRH = GPIO_ODR_ODR_14; //красный
}

void ecu_coil_2_on(void* channel) {
    GPIOD->BSRRL = GPIO_ODR_ODR_15; //синий
}

void ecu_coil_2_off(void* channel) {
    GPIOD->BSRRH = GPIO_ODR_ODR_15; //синий
}

void ecu_coil_3_on(void * channel) {
    GPIOD->BSRRL = GPIO_ODR_ODR_12; //зеленый
}

void ecu_coil_3_off(void * channel) {
    GPIOD->BSRRH = GPIO_ODR_ODR_12; //зеленый
}

void ecu_all_coil_reset(void) {
    ecu_coil_0_off(NULL);
    ecu_coil_1_off(NULL);
    ecu_coil_2_off(NULL);
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

void ecu_coil_angle_update_check(ecu_t* ecu, coil_t* coil) {
    if ((coil->set.update) && (coil->reset.update)) {
        coil->set.update = false;
        coil->reset.update = false;
    }
    
    if (coil->set.update == false) {
        coil->reset.angle+=10;
        coil->dwell_angle = ecu_coil_delta_angle_calc(ecu,ecu->vr.prev_1,ecu->vr.count,5000);
        coil->set.angle = coil->reset.angle - coil->dwell_angle;
    }
}

void ecu_coil_handler(ecu_t* ecu) {
    if (ecu->gap_correct) {
        //угол захвата N+1
        uint16_t angle = ecu->crank.angle[ecu->vr.next_1];
        //угол захвата N+2
        uint16_t next_angle = ecu->crank.angle[ecu->vr.next_2];
        //угол захвата N+3
        uint16_t next_next_angle = ecu->crank.angle[ecu->vr.next_3];
        //значение захвата N+1
        uint16_t capture = ecu->crank.capture[ecu->vr.next_1];
        //период захвата N+2
        uint16_t next_period = ecu->crank.period[ecu->vr.next_2];

        //проверка углов с запуском событий
        ecu_coil_angle_check(&ign_coil[0].set,angle,next_angle,capture,next_period);
        ecu_coil_angle_check(&ign_coil[0].reset,angle,next_angle,capture,next_period);
//        if(ecu_coil_window_angle_check(ign_coil[0].set.angle,next_angle,next_next_angle) == false) {
            ecu_coil_angle_update_check(ecu,&ign_coil[0]);
//        }
            
        ecu_coil_angle_check(&ign_coil[1].set,angle,next_angle,capture,next_period);
        ecu_coil_angle_check(&ign_coil[1].reset,angle,next_angle,capture,next_period);
//        if(ecu_coil_window_angle_check(ign_coil[1].set.angle,next_angle,next_next_angle) == false) {
            ecu_coil_angle_update_check(ecu,&ign_coil[1]);
//        }
        
        ecu_coil_angle_check(&ign_coil[2].set,angle,next_angle,capture,next_period);
        ecu_coil_angle_check(&ign_coil[2].reset,angle,next_angle,capture,next_period);
        if(ecu_coil_window_angle_check(ign_coil[2].set.angle,next_angle,next_next_angle) == false) {
            ecu_coil_angle_update_check(ecu,&ign_coil[2]);
        }
//        ecu_coil_angle_check(&ign_coil[3].set,angle,next_angle,capture,next_period);
//        ecu_coil_angle_check(&ign_coil[3].reset,angle,next_angle,capture,next_period);
//        ecu_coil_angle_update_check(ecu,&ign_coil[3]);
    }
}

void ecu_coil_init(void) {
    ecu_coil_slave_timer_1_init();
    ecu_coil_slave_timer_2_init();

    ign_coil[0].set.angle = 44782;
    ign_coil[0].reset.angle = 45874;

    ign_coil[1].set.angle = 44782;
    ign_coil[1].reset.angle = 45874+2;

    ign_coil[2].set.angle = 44782;
    ign_coil[2].reset.angle = 45874+4;

    ign_coil[3].set.angle = 44782;
    ign_coil[3].reset.angle = 45874+6;

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
