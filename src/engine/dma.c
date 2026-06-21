#include "dma.h"

void dma16(const void *src, void *dst, u32 count) {
    REG_DMA3CNT = 0;
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_16 | DMA_NOW;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma32(const void *src, void *dst, u32 count) {
    REG_DMA3CNT = 0;
    REG_DMA3SAD = (u32)src;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_32 | DMA_NOW;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma16_fill(u16 value, void *dst, u32 count) {
    REG_DMA3CNT = 0;
    REG_DMA3SAD = (u32)&value;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_16 | DMA_SRC_FIXED | DMA_NOW;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma32_fill(u32 value, void *dst, u32 count) {
    REG_DMA3CNT = 0;
    REG_DMA3SAD = (u32)&value;
    REG_DMA3DAD = (u32)dst;
    REG_DMA3CNT = DMA_ENABLE | DMA_32 | DMA_SRC_FIXED | DMA_NOW;
    while (REG_DMA3CNT & DMA_ENABLE);
}

void dma_to_vram(const void *src, void *dst, u32 size) {
    /* Ensure even alignment for 16-bit DMA */
    dma16(src, dst, size / 2);
}

void dma_to_palette(const void *src, void *dst, u32 size) {
    dma16(src, dst, size / 2);
}

void dma_to_oam(const void *src, void *dst, u32 size) {
    dma16(src, dst, size / 2);
}