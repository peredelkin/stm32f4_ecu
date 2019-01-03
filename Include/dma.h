
#ifndef DMA_H
#define DMA_H

#include <stm32f4xx.h>

#define DMA_ISR_FEIF DMA_LISR_FEIF0
#define DMA_IFCR_CFEIF DMA_LIFCR_CFEIF0

#define DMA_ISR_DMEIF DMA_LISR_DMEIF0
#define DMA_IFCR_CDMEIF DMA_LIFCR_CDMEIF0

#define DMA_ISR_TEIF DMA_LISR_TEIF0
#define DMA_IFCR_CTEIF DMA_LIFCR_CTEIF0

#define DMA_ISR_HTIF DMA_LISR_HTIF0
#define DMA_IFCR_CHTIF DMA_LIFCR_CHTIF0

#define DMA_ISR_TCIF DMA_LISR_TCIF0
#define DMA_IFCR_CTCIF DMA_LIFCR_CTCIF0

#define DMA_SxCR_CHSEL_SHIFT 25
#define DMA_SxCR_MBURST_SHIFT 23
#define DMA_SxCR_PBURST_SHIFT 21
#define DMA_SxCR_PL_SHIFT 16
#define DMA_SxCR_MSIZE_SHIFT 13
#define DMA_SxCR_PSIZE_SHIFT 11
#define DMA_SxCR_DIR_SHIFT 6

typedef struct {
    DMA_Stream_TypeDef* stream;
    DMA_TypeDef* dma;
    uint8_t dma_isr_ifcr_n; //номер ISR/IFCR
    uint8_t dma_isr_ifcr_mask_shift; //сдвиг маски ISR/IFCR
} dma_t;

//void dma_stream_fifo_error_interrupt_handler(dma_t* dma_stream, void* callback()) {
//    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_FEIF << dma_stream->dma_isr_ifcr_mask_shift)) {
//        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CFEIF << dma_stream->dma_isr_ifcr_mask_shift);
//        if (callback) callback();
//    }
//}
//
//void dma_stream_direct_mode_error_interrupt_handler(dma_t* dma_stream, void* callback()) {
//    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_DMEIF << dma_stream->dma_isr_ifcr_mask_shift)) {
//        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CDMEIF << dma_stream->dma_isr_ifcr_mask_shift);
//        if (callback) callback();
//    }
//}
//
//void dma_stream_transfer_error_interrupt_handler(dma_t* dma_stream, void* callback()) {
//    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_TEIF << dma_stream->dma_isr_ifcr_mask_shift)) {
//        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CTEIF << dma_stream->dma_isr_ifcr_mask_shift);
//        if (callback) callback();
//    }
//}
//
//void dma_stream_half_transfer_interrupt_handler(dma_t* dma_stream, void* callback()) {
//    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_HTIF << dma_stream->dma_isr_ifcr_mask_shift)) {
//        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CHTIF << dma_stream->dma_isr_ifcr_mask_shift);
//        if (callback) callback();
//    }
//}
//
//void dma_stream_transfer_complete_interrupt_handler(dma_t* dma_stream, void* callback()) {
//    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_TCIF << dma_stream->dma_isr_ifcr_mask_shift)) {
//        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CTCIF << dma_stream->dma_isr_ifcr_mask_shift);
//        if (callback) callback();
//    }
//}

void dma_stream_channel_selection(dma_t* dma_stream, uint8_t chsel) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_CHSEL);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_CHSEL | (uint32_t) (chsel << DMA_SxCR_CHSEL_SHIFT)));
}

void dma_stream_memory_burst_transfer_configuration(dma_t* dma_stream, uint8_t mburst) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MBURST);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_MBURST | (uint32_t) (mburst << DMA_SxCR_MBURST_SHIFT)));
}

void dma_stream_peripheral_burst_transfer_configuration(dma_t* dma_stream, uint8_t pburst) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PBURST);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PBURST | (uint32_t) (pburst << DMA_SxCR_PBURST_SHIFT)));
}

void dma_stream_current_target(dma_t* dma_stream, bool ct) {
    if (ct) SET_BIT(dma_stream->stream->CR, DMA_SxCR_CT);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_CT);
}

bool dma_stream_current_target_read(dma_t* dma_stream) {
    if (READ_BIT(dma_stream->stream->CR, DMA_SxCR_CT)) return 1;
    else return 0;
}

void dma_stream_double_buffer_mode(dma_t* dma_stream, bool dbm) {
    if (dbm) dma_stream->stream->CR |= DMA_SxCR_DBM;
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_DBM);
}

void dma_stream_priority_level(dma_t* dma_stream, uint8_t pl) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PL);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PL | (uint32_t) (pl << DMA_SxCR_PL_SHIFT)));
}

void dma_stream_peripheral_increment_offset_size(dma_t* dma_stream, bool pincos) {
    if (pincos) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PINCOS);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PINCOS);
}

void dma_stream_memory_data_size(dma_t* dma_stream, uint8_t msize) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MSIZE);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_MSIZE | (uint32_t) (msize << DMA_SxCR_MSIZE_SHIFT)));
}

void dma_stream_peripheral_data_size(dma_t* dma_stream, uint8_t psize) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PSIZE);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PSIZE | (uint32_t) (psize << DMA_SxCR_PSIZE_SHIFT)));
}

void dma_stream_memory_increment_mode(dma_t* dma_stream, bool minc) {
    if (minc) SET_BIT(dma_stream->stream->CR, DMA_SxCR_MINC);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MINC);
}

void dma_stream_peripheral_increment_mode(dma_t* dma_stream, bool pinc) {
    if (pinc) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PINC);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PINC);
}

void dma_stream_circular_mode(dma_t* dma_stream, bool circ) {
    if (circ) SET_BIT(dma_stream->stream->CR, DMA_SxCR_CIRC);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_CIRC);
}

void dma_stream_data_transfer_direction(dma_t* dma_stream, uint8_t dir) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_DIR);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_DIR | (uint32_t) (dir << DMA_SxCR_DIR_SHIFT)));
}

void dma_stream_peripheral_flow_controller(dma_t* dma_stream, bool pfctrl) {
    if (pfctrl) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PFCTRL);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PFCTRL);
}

void dma_stream_transfer_complete_interrupt(dma_t* dma_stream, bool tcie) {
    if (tcie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_TCIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_TCIE);
}

void dma_stream_half_transfer_interrupt(dma_t* dma_stream, bool htie) {
    if (htie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_HTIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_HTIE);
}

void dma_stream_transfer_error_interrupt(dma_t* dma_stream, bool teie) {
    if (teie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_TEIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_TEIE);
}

void dma_stream_direct_mode_error_interrupt(dma_t* dma_stream, bool dmeie) {
    if (dmeie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_DMEIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_DMEIE);
}

void dma_stream_enable(dma_t* dma_stream, bool en) {
    if (en) SET_BIT(dma_stream->stream->CR, DMA_SxCR_EN);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_EN);
}

bool dma_stream_ready(dma_t* dma_stream) {
    if (READ_BIT(dma_stream->stream->CR, DMA_SxCR_EN)) return 0; //not ready
    else return 1; //ready
}

void dma_stream_number_of_data(dma_t* dma_stream,uint16_t ndtr) {
    dma_stream->stream->NDTR = ndtr;
}

void dma_stream_peripheral_address_register(dma_t* dma_stream,uint32_t par) {
    dma_stream->stream->PAR = par;
}

void stream_memory_address_register(dma_t* dma_stream,uint8_t mar_n,uint32_t mar) {
    dma_stream->stream->MAR[mar_n] = mar;
}

#endif /* DMA_H */

