#include "dma.h"

uint16_t dma_stream_fifo_error_interrupt_status_read(dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_FEIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_fifo_error_interrupt_status_clear(dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CFEIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint16_t dma_stream_direct_mode_error_interrupt_read(dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_DMEIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_direct_mode_error_interrupt_clear(dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CDMEIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint16_t dma_stream_transfer_error_interrupt_read(dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_TEIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_transfer_error_interrupt_clear(dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CTEIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint16_t dma_stream_half_transfer_interrupt_read(dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_HTIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_half_transfer_interrupt_clear(dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CHTIF << dma_stream->dma_isr_ifcr_mask_shift);
}

uint16_t dma_stream_transfer_complete_interrupt_read(dma_t* dma_stream) {
    return (dma_stream->dma->ISR[dma_stream->dma_isr_ifcr_n] & (uint16_t) (DMA_ISR_TCIF << dma_stream->dma_isr_ifcr_mask_shift));
}

void dma_stream_transfer_complete_interrupt_clear(dma_t* dma_stream) {
    dma_stream->dma->IFCR[dma_stream->dma_isr_ifcr_n] = (uint16_t) (DMA_IFCR_CTCIF << dma_stream->dma_isr_ifcr_mask_shift);
}

void dma_stream_channel_selection(dma_t* dma_stream, uint8_t chsel) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_CHSEL);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_CHSEL & (uint32_t) (chsel << DMA_SxCR_CHSEL_SHIFT)));
}

void dma_stream_memory_burst_transfer_configuration(dma_t* dma_stream, uint8_t mburst) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MBURST);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_MBURST & (uint32_t) (mburst << DMA_SxCR_MBURST_SHIFT)));
}

void dma_stream_peripheral_burst_transfer_configuration(dma_t* dma_stream, uint8_t pburst) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PBURST);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PBURST & (uint32_t) (pburst << DMA_SxCR_PBURST_SHIFT)));
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
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PL & (uint32_t) (pl << DMA_SxCR_PL_SHIFT)));
}

void dma_stream_peripheral_increment_offset_size(dma_t* dma_stream, bool pincos) {
    if (pincos) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PINCOS);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PINCOS);
}

void dma_stream_memory_data_size(dma_t* dma_stream, uint8_t msize) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_MSIZE);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_MSIZE & (uint32_t) (msize << DMA_SxCR_MSIZE_SHIFT)));
}

void dma_stream_peripheral_data_size(dma_t* dma_stream, uint8_t psize) {
    CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PSIZE);
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_PSIZE & (uint32_t) (psize << DMA_SxCR_PSIZE_SHIFT)));
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
    SET_BIT(dma_stream->stream->CR, (DMA_SxCR_DIR & (uint32_t) (dir << DMA_SxCR_DIR_SHIFT)));
}

void dma_stream_peripheral_flow_controller(dma_t* dma_stream, bool pfctrl) {
    if (pfctrl) SET_BIT(dma_stream->stream->CR, DMA_SxCR_PFCTRL);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_PFCTRL);
}

void dma_stream_transfer_complete_interrupt_enable(dma_t* dma_stream, bool tcie) {
    if (tcie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_TCIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_TCIE);
}

void dma_stream_half_transfer_interrupt_enable(dma_t* dma_stream, bool htie) {
    if (htie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_HTIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_HTIE);
}

void dma_stream_transfer_error_interrupt_enable(dma_t* dma_stream, bool teie) {
    if (teie) SET_BIT(dma_stream->stream->CR, DMA_SxCR_TEIE);
    else CLEAR_BIT(dma_stream->stream->CR, DMA_SxCR_TEIE);
}

void dma_stream_direct_mode_error_interrupt_enable(dma_t* dma_stream, bool dmeie) {
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

uint16_t dma_stream_number_of_data_read(dma_t* dma_stream) {
    return dma_stream->stream->NDTR;
}

void dma_stream_peripheral_address(dma_t* dma_stream,uint32_t par) {
    dma_stream->stream->PAR = par;
}

void dma_stream_memory_address(dma_t* dma_stream,uint8_t mar_n,uint32_t mar) {
    dma_stream->stream->MAR[mar_n] = mar;
}

void dma_stream_struct_init(dma_t* dma_stream, DMA_TypeDef* dma, DMA_Stream_TypeDef* stream, uint8_t stream_n) {
    dma_stream->dma = dma;
    dma_stream->stream = stream;
    dma_stream->dma_isr_ifcr_n = (uint8_t)(stream_n / 2);
    dma_stream->dma_isr_ifcr_mask_shift = (uint8_t)(6 * (stream_n % 2));
}

void dma_stream_deinit(dma_t* dma_stream) {
    dma_stream_enable(dma_stream,false);
    dma_stream->stream->CR = 0;
}