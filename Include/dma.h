
#ifndef DMA_H
#define DMA_H

#include <stm32f4xx.h>
#include <stdbool.h>

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

extern uint16_t dma_stream_fifo_error_interrupt_status_read(dma_t* dma_stream);

extern void dma_stream_fifo_error_interrupt_status_clear(dma_t* dma_stream);

extern uint16_t dma_stream_direct_mode_error_interrupt_read(dma_t* dma_stream);

extern void dma_stream_direct_mode_error_interrupt_clear(dma_t* dma_stream);

extern uint16_t dma_stream_transfer_error_interrupt_read(dma_t* dma_stream);

extern void dma_stream_transfer_error_interrupt_clear(dma_t* dma_stream);

extern uint16_t dma_stream_half_transfer_interrupt_read(dma_t* dma_stream);

extern void dma_stream_half_transfer_interrupt_clear(dma_t* dma_stream);

extern uint16_t dma_stream_transfer_complete_interrupt_read(dma_t* dma_stream);

extern void dma_stream_transfer_complete_interrupt_clear(dma_t* dma_stream);

extern void dma_stream_channel_selection(dma_t* dma_stream, uint8_t chsel);

extern void dma_stream_memory_burst_transfer_configuration(dma_t* dma_stream, uint8_t mburst);

extern void dma_stream_peripheral_burst_transfer_configuration(dma_t* dma_stream, uint8_t pburst);

extern void dma_stream_current_target(dma_t* dma_stream, bool ct);

extern bool dma_stream_current_target_read(dma_t* dma_stream);

extern void dma_stream_double_buffer_mode(dma_t* dma_stream, bool dbm);

extern void dma_stream_priority_level(dma_t* dma_stream, uint8_t pl);

extern void dma_stream_peripheral_increment_offset_size(dma_t* dma_stream, bool pincos);

extern void dma_stream_memory_data_size(dma_t* dma_stream, uint8_t msize);

extern void dma_stream_peripheral_data_size(dma_t* dma_stream, uint8_t psize);

extern void dma_stream_memory_increment_mode(dma_t* dma_stream, bool minc);

extern void dma_stream_peripheral_increment_mode(dma_t* dma_stream, bool pinc);

extern void dma_stream_circular_mode(dma_t* dma_stream, bool circ);

extern void dma_stream_data_transfer_direction(dma_t* dma_stream, uint8_t dir);

extern void dma_stream_peripheral_flow_controller(dma_t* dma_stream, bool pfctrl);

extern void dma_stream_transfer_complete_interrupt_enable(dma_t* dma_stream, bool tcie);

extern void dma_stream_half_transfer_interrupt_enable(dma_t* dma_stream, bool htie);

extern void dma_stream_transfer_error_interrupt_enable(dma_t* dma_stream, bool teie);

extern void dma_stream_direct_mode_error_interrupt_enable(dma_t* dma_stream, bool dmeie);

extern void dma_stream_enable(dma_t* dma_stream, bool en);

extern bool dma_stream_ready(dma_t* dma_stream);

extern void dma_stream_number_of_data(dma_t* dma_stream,uint16_t ndtr);

extern uint16_t dma_stream_number_of_data_read(dma_t* dma_stream);

extern void dma_stream_peripheral_address(dma_t* dma_stream,uint32_t par);

extern void dma_stream_memory_address(dma_t* dma_stream,uint8_t mar_n,uint32_t mar);

extern void dma_stream_struct_init(dma_t* dma_stream, DMA_TypeDef* dma, DMA_Stream_TypeDef* stream, uint8_t stream_n);

extern void dma_stream_deinit(dma_t* dma_stream);

#endif /* DMA_H */

