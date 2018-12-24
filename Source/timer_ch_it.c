#include "timer_ch_it.h"

void timer_ch_it_init(timer_ch_it_t* t_it_ch,TIM_TypeDef* TIM,
        const uint8_t channel,const uint16_t status_mask,
        const uint16_t interrupt_mask) {
    switch(channel) {
        case 1:
            t_it_ch->CCR = &TIM->CCR1;
            break;
        case 2:
            t_it_ch->CCR = &TIM->CCR2;
            break;
        case 3:
            t_it_ch->CCR = &TIM->CCR3;
            break;
        case 4:
            t_it_ch->CCR = &TIM->CCR4;
            break;
        default: return; break;
    }

    t_it_ch->DIER = (uint32_t *)(&TIM->DIER);
    t_it_ch->SR = (uint32_t *)(&TIM->SR);
    t_it_ch->SR_MASK = status_mask;
    t_it_ch->IE_MASK = interrupt_mask;
}

void timer_ch_it_handler(timer_ch_it_t* t_it_ch) {
    if ((*t_it_ch->DIER & t_it_ch->IE_MASK) && (*t_it_ch->SR & t_it_ch->SR_MASK)) { //чтение разрешения прерывания и статуса
        *t_it_ch->SR = ~t_it_ch->SR_MASK; //очистка статуса
        if (t_it_ch->once) {
            *t_it_ch->DIER &= ~t_it_ch->IE_MASK; //запрет прерывания при однократном выполнении
        }
        if (t_it_ch->event) t_it_ch->event(t_it_ch); //вызов
    }
}

void timer_ch_it_enable(timer_ch_it_t* t_it_ch,bool once) {
    t_it_ch->once = once;
    *t_it_ch->SR = ~t_it_ch->SR_MASK;
    *t_it_ch->DIER |= t_it_ch->IE_MASK;
}

void timer_ch_it_disable(timer_ch_it_t* t_it_ch) {
    *t_it_ch->DIER &= ~t_it_ch->IE_MASK;
}

uint16_t timer_ch_ccr_read(timer_ch_it_t* t_it_ch) {
    return (uint16_t)(*t_it_ch->CCR);
}

void timer_ch_ccr_write(timer_ch_it_t* t_it_ch,const uint16_t ccr) {
    *t_it_ch->CCR = (uint16_t)ccr;
}

void timer_ch_event_set(timer_ch_it_t* t_it_ch,void (*event)(void*)) {
    t_it_ch->event = event;
}
