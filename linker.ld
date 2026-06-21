/* Neural Link - GBA Linker Script */

OUTPUT_FORMAT(elf32-littlearm)
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
    ewram (rwx) : ORIGIN = 0x02000000, LENGTH = 256K
    iwram (rwx) : ORIGIN = 0x03000000, LENGTH = 32K
    rom   (rx)  : ORIGIN = 0x08000000, LENGTH = 32M
}

SECTIONS
{
    /* ROM code and rodata */
    .text 0x08000000 :
    {
        *(.crt0);
        *(.text);
        *(.text*);
        *(.rodata);
        *(.rodata*);
        . = ALIGN(4);
    } > rom

    /* Read-write data - copied to EWRAM at startup */
    .data :
    {
        _data_start = .;
        *(.data);
        *(.data*);
        . = ALIGN(4);
        _data_end = .;
    } > ewram AT > rom
    _data_lma = LOADADDR(.data);

    /* BSS - zeroed at startup */
    .bss :
    {
        _bss_start = .;
        *(.bss);
        *(.bss*);
        *(COMMON);
        . = ALIGN(4);
        _bss_end = .;
    } > ewram

    /* EWRAM pool for large data */
    .ewram (NOLOAD) :
    {
        *(.ewram);
        . = ALIGN(4);
    } > ewram
    _ewram_end = ORIGIN(ewram) + LENGTH(ewram);

    /* IWRAM for critical code/data */
    .iwram (NOLOAD) :
    {
        *(.iwram);
        . = ALIGN(4);
    } > iwram
    _iwram_end = ORIGIN(iwram) + LENGTH(iwram);

    /* Discard unwanted sections */
    /DISCARD/ :
    {
        *(.ARM.attributes);
        *(.comment);
        *(.ARM.exidx*);
    }
}