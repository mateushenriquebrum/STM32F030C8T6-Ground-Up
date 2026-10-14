.cpu cortex-m0
.syntax unified
.thumb

.section .isr_vector
.align 2

.word 0x20002000       /* Initial stack pointer */
.word _reset_          /* Reset */

.word spin             /* NMI */
.word spin             /* HardFault */

.word 0                /* Reserved */
.word 0                /* Reserved */
.word 0                /* Reserved */
.word 0                /* Reserved */
.word 0                /* Reserved */
.word 0                /* Reserved */
.word 0                /* Reserved */
.word spin             /* SVCall */

.word 0                /* Reserved */
.word 0                /* Reserved */

.word spin             /* PendSV */
.word spin             /* SysTick */


/* External interrupts */
/* IRQ 0  */
.word spin                  /* WWDG */
/* IRQ 1  */
.word spin                  /* PVD */
/* IRQ 2  */
.word spin                  /* RTC */
/* IRQ 3  */
.word spin                  /* FLASH */
/* IRQ 4  */
.word spin                  /* RCC */
/* IRQ 5  */
.word spin                  /* EXTI0_1 */
/* IRQ 6  */
.word spin                  /* EXTI2_3 */
/* IRQ 7  */
.word spin                  /* EXTI4_15 */
/* IRQ 8  */
.word spin                  /* TSC */
/* IRQ 9  */
.word spin                  /* DMA1_Channel1 */
/* IRQ 10 */
.word spin                  /* DMA1_Channel2_3 */
/* IRQ 11 */
.word spin                  /* DMA1_Channel4_5 */
/* IRQ 12 */
.word spin                  /* ADC1_COMP */
/* IRQ 13 */
.word spin                  /* TIM1_BRK_UP_TRG_COM */
/* IRQ 14 */
.word spin                  /* TIM1_CC */
/* IRQ 15 */
.word spin                  /* TIM2 */
/* IRQ 16 */
.word spin                  /* TIM3 */

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