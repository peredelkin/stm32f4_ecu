
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
#define DMA_SxCR_CT_SHIFT 19
#define DMA_SxCR_DBM_SHIFT 18
#define DMA_SxCR_PL_SHIFT 16
#define DMA_SxCR_PINCOS_SHIFT 15
#define DMA_SxCR_MSIZE_SHIFT 13
#define DMA_SxCR_PSIZE_SHIFT 11

typedef struct {
    DMA_Stream_TypeDef* stream;
    DMA_TypeDef* dma;
    uint8_t dma_isr_ifcr_n; //номер ISR/IFCR
    uint8_t dma_isr_ifcr_mask_shift; //сдвиг маски ISR/IFCR
} dma_t;

void dma_stream_fifo_error_interrupt_handler(dma_t* dma_stream, void* callback()) {
    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_FEIF << dma_stream->dma_isr_ifcr_mask_shift)) {
        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CFEIF << dma_stream->dma_isr_ifcr_mask_shift);
        if (callback) callback();
    }
}

void dma_stream_direct_mode_error_interrupt_handler(dma_t* dma_stream, void* callback()) {
    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_DMEIF << dma_stream->dma_isr_ifcr_mask_shift)) {
        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CDMEIF << dma_stream->dma_isr_ifcr_mask_shift);
        if (callback) callback();
    }
}

void dma_stream_transfer_error_interrupt_handler(dma_t* dma_stream, void* callback()) {
    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_TEIF << dma_stream->dma_isr_ifcr_mask_shift)) {
        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CTEIF << dma_stream->dma_isr_ifcr_mask_shift);
        if (callback) callback();
    }
}

void dma_stream_half_transfer_interrupt_handler(dma_t* dma_stream, void* callback()) {
    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_HTIF << dma_stream->dma_isr_ifcr_mask_shift)) {
        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CHTIF << dma_stream->dma_isr_ifcr_mask_shift);
        if (callback) callback();
    }
}

void dma_stream_transfer_complete_interrupt_handler(dma_t* dma_stream, void* callback()) {
    if (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_TCIF << dma_stream->dma_isr_ifcr_mask_shift)) {
        dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CTCIF << dma_stream->dma_isr_ifcr_mask_shift);
        if (callback) callback();
    }
}

void dma_stream_channel_selection(dma_t* dma_stream,uint8_t chsel) {
    dma_stream->stream->CR &= ~ DMA_SxCR_CHSEL;
    dma_stream->stream->CR |= (DMA_SxCR_CHSEL | (uint32_t)(chsel << DMA_SxCR_CHSEL_SHIFT));
}

void dma_stream_memory_burst_transfer_configuration(dma_t* dma_stream,uint8_t mburst) {
    dma_stream->stream->CR &= ~ DMA_SxCR_MBURST;
    dma_stream->stream->CR |= (DMA_SxCR_MBURST | (uint32_t)(mburst << DMA_SxCR_MBURST_SHIFT));
}

void dma_stream_peripheral_burst_transfer_configuration(dma_t* dma_stream,uint8_t pburst) {
    dma_stream->stream->CR &= ~ DMA_SxCR_PBURST;
    dma_stream->stream->CR |= (DMA_SxCR_PBURST | (uint32_t)(pburst << DMA_SxCR_PBURST_SHIFT));
}

void dma_stream_current_target(dma_t* dma_stream,bool ct) {
    dma_stream->stream->CR &= ~DMA_SxCR_CT;
    dma_stream->stream->CR |= (DMA_SxCR_CT | (uint32_t)(ct << DMA_SxCR_CT_SHIFT));
}

bool dma_stream_current_target_read(dma_t* dma_stream) {
    if(dma_stream->stream->CR & DMA_SxCR_CT) return 1;
    else return 0;
}

void dma_stream_double_buffer_mode(dma_t* dma_stream,bool dbm) {
    dma_stream->stream->CR &= ~DMA_SxCR_DBM;
    dma_stream->stream->CR |= (DMA_SxCR_DBM | (uint32_t)(dbm << DMA_SxCR_DBM_SHIFT));
}

void dma_stream_priority_level(dma_t* dma_stream,uint8_t pl) {
    dma_stream->stream->CR &= ~DMA_SxCR_PL;
    dma_stream->stream->CR |= (DMA_SxCR_PL | (uint32_t)(pl << DMA_SxCR_PL_SHIFT));
}

void dma_stream_peripheral_increment_offset_size(dma_t* dma_stream,bool pincos) {
    dma_stream->stream->CR &= ~DMA_SxCR_PINCOS;
    dma_stream->stream->CR |= (DMA_SxCR_PINCOS | (uint32_t)(pincos << DMA_SxCR_PINCOS_SHIFT));
}

void dma_stream_memory_data_size(dma_t* dma_stream,uint8_t msize) {
    dma_stream->stream->CR &= ~DMA_SxCR_MSIZE;
    dma_stream->stream->CR |= (DMA_SxCR_MSIZE | (uint32_t)(msize << DMA_SxCR_MSIZE_SHIFT));
}

void dma_stream_peripheral_data_size(dma_t* dma_stream,uint8_t psize) {
    dma_stream->stream->CR &= ~DMA_SxCR_PSIZE;
    dma_stream->stream->CR |= (DMA_SxCR_PSIZE | (uint16_t)(psize << DMA_SxCR_PSIZE_SHIFT));
}

#endif /* DMA_H */

