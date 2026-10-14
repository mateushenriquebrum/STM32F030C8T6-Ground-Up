#include "usart1.h"

#include <stdint.h>

#define RCC_AHBENR    (*(volatile uint32_t *)0x40021014U)
#define RCC_APB2ENR   (*(volatile uint32_t *)0x40021018U)

#define GPIOA_MODER   (*(volatile uint32_t *)0x48000000U)
#define GPIOA_AFRH    (*(volatile uint32_t *)0x48000024U)

#define USART1_CR1    (*(volatile uint32_t *)0x40013800U)
#define USART1_BRR    (*(volatile uint32_t *)0x4001380CU)
#define USART1_ISR    (*(volatile uint32_t *)0x4001381CU)
#define USART1_RDR    (*(volatile uint32_t *)0x40013824U)
#define USART1_TDR    (*(volatile uint32_t *)0x40013828U)

#define GPIOA_CLOCK_ENABLE  (1U << 17)
#define USART1_CLOCK_ENABLE (1U << 14)

#define USART_CR1_RE (1U << 2)
#define USART_CR1_TE (1U << 3)
#define USART_CR1_UE (1U << 0)

#define USART_ISR_RXNE (1U << 5)
#define USART_ISR_TXE  (1U << 7)

void usart1_init(void)
{
    /* Enable GPIOA and USART1 clocks. */
    RCC_AHBENR  |= GPIOA_CLOCK_ENABLE;
    RCC_APB2ENR |= USART1_CLOCK_ENABLE;

    /* PA9: alternate-function mode. */
    GPIOA_MODER &= ~(3U << 18);
    GPIOA_MODER |=  (2U << 18);

    /* PA9 alternate function AF1 = USART1_TX. */
    GPIOA_AFRH &= ~(0xFU << 4);
    GPIOA_AFRH |=  (1U << 4);

    /* PA10: alternate-function mode. */
    GPIOA_MODER &= ~(3U << 20);
    GPIOA_MODER |=  (2U << 20);

    /* PA10 alternate function AF1 = USART1_RX. */
    GPIOA_AFRH &= ~(0xFU << 8);
    GPIOA_AFRH |=  (1U << 8);

    /*
     * Assumes the USART1 clock is 8 MHz.
     * BRR = 69 gives approximately 115942 baud.
     */
    USART1_BRR = 69U;

    /* Enable transmitter, receiver, then USART. */
    USART1_CR1 |= USART_CR1_TE;
    USART1_CR1 |= USART_CR1_RE;
    USART1_CR1 |= USART_CR1_UE;
}

char usart1_read_character(void)
{
    while ((USART1_ISR & USART_ISR_RXNE) == 0U) {
        /* Wait until a received character is available. */
    }

    return (char)USART1_RDR;
}

void usart1_write_character(char c)
{
    while ((USART1_ISR & USART_ISR_TXE) == 0U) {
        /* Wait until the transmit data register is empty. */
    }

    USART1_TDR = (uint32_t)(unsigned char)c;
}

void usart1_write_string(const char *text)
{
    if (text == 0) {
        return;
    }

    while (*text != '\0') {
        usart1_write_character(*text);
        text++;
    }
}