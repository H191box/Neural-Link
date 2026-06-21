@ Neural Link - GBA CRT0 Startup Code
@ Sets up stack, copies .data, zeroes .bss, enters main

.section .crt0, "ax", @progbits
.global _start
.type _start, %function

_start:
    @ Set up stack pointer in IWRAM
    ldr sp, =_iwram_stack_top

    @ Copy .data section from ROM to EWRAM
    ldr r0, =_data_lma     @ Source (ROM)
    ldr r1, =_data_start   @ Dest (EWRAM)
    ldr r2, =_data_end
    sub r2, r2, r1          @ Size
copy_data:
    cmp r2, #0
    beq copy_data_done
    ldr r3, [r0], #4
    str r3, [r1], #4
    sub r2, r2, #4
    b copy_data
copy_data_done:

    @ Zero .bss section
    ldr r0, =_bss_start
    ldr r1, =_bss_end
    mov r2, #0
zero_bss:
    cmp r0, r1
    bge zero_bss_done
    str r2, [r0], #4
    b zero_bss
zero_bss_done:

    @ Enable BIOS interrupt handlers
    mov r0, #1
    swi 0x01                @ RegisterRamReset - no, just call main

    @ Call main
    bl main

    @ Infinite loop (should never reach here)
hang:
    b hang

.section .bss
.space 256
_iwram_stack_top: