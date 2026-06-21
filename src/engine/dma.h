#ifndef DMA_H
#define DMA_H

#include "gba_types.h"
#include "gba_header.h"

/* 16-bit DMA transfer (DMA3, fastest, no IRQ) */
void dma16(const void *src, void *dst, u32 count);

/* 32-bit DMA transfer (DMA3, fastest, no IRQ) */
void dma32(const void *src, void *dst, u32 count);

/* Fast 16-bit fill */
void dma16_fill(u16 value, void *dst, u32 count);

/* Fast 32-bit fill */
void dma32_fill(u32 value, void *dst, u32 count);

/* Copy to VRAM (handles alignment) */
void dma_to_vram(const void *src, void *dst, u32 size);

/* Copy to palette memory */
void dma_to_palette(const void *src, void *dst, u32 size);

/* Copy to OAM */
void dma_to_oam(const void *src, void *dst, u32 size);

#endif /* DMA_H */