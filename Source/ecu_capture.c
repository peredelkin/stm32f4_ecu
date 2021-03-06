#include "ecu_capture.h"
#include "ecu_coil.h"

void ecu_capture_timer_init() {
    NVIC_SetPriority(TIM1_CC_IRQn, 6); //34 default
    NVIC_EnableIRQ(ECU_CAP_TIM_IRQn); // Capture/compare Interrupt Enable

    ECU_CAP_TIM->CCMR1 |= TIM_CCMR1_CC1S_0; // Capture TI1

    ECU_CAP_TIM->CCMR1 |= TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1; // Filter Fsampling N=8

    //      0000: No filter, sampling is done at fDTS
    //	0001: fSAMPLING=fCK_INT , N=2
    //	0010: fSAMPLING=fCK_INT , N=4
    //	0011: fSAMPLING=fCK_INT , N=8

    ECU_CAP_TIM->CCER |= TIM_CCER_CC1P; // !Falling edge

    ECU_CAP_TIM->CCER |= TIM_CCER_CC1E; // Capture Enable

    ECU_CAP_TIM->PSC = (uint16_t) ((SystemCoreClock/ECU_CAP_TIM_CLK) - 1); // Prescaler

    ECU_CAP_TIM->EGR = TIM_EGR_UG; // Re-initialize

    //=================Master========================

    ECU_CAP_TIM->CR2 |= TIM_CR2_MMS_0; // Master 001 Enable

    ECU_CAP_TIM->SMCR |= TIM_SMCR_MSM; // Fo better Sync

    ECU_CAP_TIM->SMCR &= ~TIM_SMCR_TS; // ITR0

    //=================Master End=====================
}

void ecu_crank_capture_period_ovf_write(ecu_t* ecu, uint16_t capture) {
    timer_ch_ccr_write(&ecu->ovf_cap_ch, (uint16_t) (capture + 0xffff)); //сдвиг ovf cap
    ecu_crank_capture_write(ecu, ecu->vr.count, capture); //запись захвата
    ecu_crank_period_write(ecu, ecu->vr.count, ecu_crank_period_calc(ecu)); //запись периода
    if (!(ECU_CAP_TIM->CR1 & TIM_CR1_CEN)) {
        timer_ch_it_enable(&ecu->ovf_cap_ch, true); //включение однократного прерывания останова
        ECU_COIL_TIM_1->CR1 |= TIM_CR1_CEN;
        ECU_COIL_TIM_2->CR1 |= TIM_CR1_CEN;
        ECU_CAP_TIM->CR1 |= TIM_CR1_CEN;
    }
}

void ecu_crank_capture_handler(ecu_t* ecu, void* tim_ch) {
    ecu_crank_counter(&ecu->vr); //vr_count++
    ecu_crank_capture_period_ovf_write(ecu, timer_ch_ccr_read(tim_ch)); //чтение захвата и запись ovf,capture,period
    ecu_crank_sync(ecu);
    ecu_instant_rpm_calc(ecu);
    crank_extrapolation_calc(ecu); //расчет экстраполяции в точках vr_count + 1 и vr_count + 2
}

void ecu_crank_capture_init(ecu_t* ecu) {
    ecu_capture_timer_init();
    make_timer_ch_it_init(&ecu->cap_ch, ECU_CAP_TIM, 1); //настройка канала "захват"
    make_timer_ch_it_init(&ecu->ovf_cap_ch, ECU_CAP_TIM, 2); //настройка канала "переполнение захвата"
}

void ecu_cap_irq_handler(ecu_t* ecu) {
    timer_ch_it_handler(&ecu->cap_ch);
    timer_ch_it_handler(&ecu->ovf_cap_ch);
}

void ECU_CAP_TIM_IRQHandler(void) {
    ecu_cap_irq_handler(&ecu_struct);
}

