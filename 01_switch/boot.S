.cpu cortex-m0
.syntax unified
.thumb

.section .isr_vector
.align 2

.word 0x20002000       /* Initial stack pointer */
.word _reset_          /* Reset */

.word spin             /* NMI */
.word spin             /* HardFault */

.word 0
.word 0
.word 0
.word 0
.word 0
.word 0
.word 0

.word spin             /* SVCall */

.word 0
.word 0

.word spin             /* PendSV */
.word spin             /* SysTick */


/* ------------------------- */

.section .text
.align 2

.thumb_func
spin:
    bkpt #0
    b spin

.thumb_func
_reset_:
    bl main

1:
    b 1b