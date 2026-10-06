MEMORY
{
    flash (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    sram  (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
    .text :
    {
        . = ALIGN(4);

        KEEP(*(.isr_vector))

        *(.text*)
        *(.rodata*)

        . = ALIGN(4);
    } > flash

    .data :
    {
        . = ALIGN(4);
        *(.data*)
        . = ALIGN(4);
    } > sram AT > flash

    .bss :
    {
        . = ALIGN(4);
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
    } > sram
}