#ifndef GBA_TYPES_H
#define GBA_TYPES_H

/* Neural Link - GBA Type Definitions */

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;

typedef enum { FALSE = 0, TRUE = 1 } bool;

/* Common constants */
#define SCREEN_W    240
#define SCREEN_H    160
#define TILE_SIZE   8
#define TILE_MAP_W  32
#define TILE_MAP_H  32
#define BG_TILE_MAP_SIZE (TILE_MAP_W * TILE_MAP_H)

/* Color conversion macros */
#define RGB(r, g, b) ((u16)(((r) & 0x1F) | (((g) & 0x1F) << 5) | (((b) & 0x1F) << 10)))
#define RGB8(r, g, b) RGB((r)>>3, (g)>>3, (b)>>3)
#define RGB_RED(c)   ((c) & 0x1F)
#define RGB_GREEN(c) (((c) >> 5) & 0x1F)
#define RGB_BLUE(c)  (((c) >> 10) & 0x1F)

/* Math macros */
#define MIN(a, b)   ((a) < (b) ? (a) : (b))
#define MAX(a, b)   ((a) > (b) ? (a) : (b))
#define CLAMP(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
#define ABS(x)      ((x) < 0 ? -(x) : (x))
#define DIV_FLOOR(a, b) ((a) / (b) - ((a) % (b) && ((a) ^ (b)) < 0))

/* Array size macro */
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/* Null */
#ifndef NULL
#define NULL ((void*)0)
#endif

/* Byte swapping for big-endian ROM data */
#define SWAP16(x) ((((x) & 0xFF) << 8) | (((x) >> 8) & 0xFF))

/* String length (no libc) */
static inline int my_strlen(const char *s) {
    int len = 0;
    while (s[len]) len++;
    return len;
}

/* String compare (no libc) */
static inline int my_strcmp(const char *a, const char *b) {
    while (*a && *a == *b) { a++; b++; }
    return *(unsigned char*)a - *(unsigned char*)b;
}

/* String copy (no libc) */
static inline char* my_strcpy(char *dst, const char *src) {
    char *d = dst;
    while ((*d++ = *src++));
    return dst;
}

/* Memory set (no libc) */
static inline void* my_memset(void *s, int c, unsigned int n) {
    u8 *p = (u8*)s;
    while (n--) *p++ = (u8)c;
    return s;
}

/* Memory copy (no libc) */
static inline void* my_memcpy(void *dst, const void *src, unsigned int n) {
    u8 *d = (u8*)dst;
    const u8 *s = (const u8*)src;
    while (n--) *d++ = *s++;
    return dst;
}

#endif /* GBA_TYPES_H */