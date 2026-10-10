#include <stdint.h>

//pg 39 0x4002 1000 - 0x4002 13FF
//pg 125 0x14
#define RCC_AHBENR      (*(volatile uint32_t *) 0x40021014U) 

//pg 39, 0x4002 1000 - 0x4002 13FF
//pg 125 0x14
#define RCC_APB2ENR     (*(volatile uint32_t *) 0x40021018U) 

//pg 39, 0x4800 0000 - 0x4800 03FF
//pg 141, 0x00
#define GPIOA_MODER     (*(volatile uint32_t *) 0x48000000U)

//pg 39, 0x4800 0000 - 0x4800 03FF
//pg 143, 0x24
#define GPIOA_AFRH      (*(volatile uint32_t *) 0x48000024U)

//pg 40, 0x4001 3800 - 0x4001 3BFF
//pg 640, 0x0C
#define USART1_BRR       (*(volatile uint32_t *) 0x4001380CU)

//pg 40, 0x4001 3800 - 0x4001 3BFF
//pg 640, 0x00
#define USART1_CR1      (*(volatile uint32_t *)0x40013800U)

//pg 40, 0x4001 3800 - 0x4001 3BFF
//pg 641, 0x1C
#define USART1_ISR    (*(volatile uint32_t *)0x4001381CU)

//pg 40, 0x4001 3800 - 0x4001 3BFF
//pg 641, 0x28
#define USART1_TDR    (*(volatile uint32_t *)0x40013828U)

//pg 40, 0x4001 3800 - 0x4001 3BFF
//pg 641, 0x24
#define USART1_RDR    (*(volatile uint32_t *)0x40013824U) 

static void usart1_init(void)
{
    /* Enable GPIOA clock */
    RCC_AHBENR |= (1U << 17);

    /* Enable USART1 clock */
    RCC_APB2ENR |= (1U << 14);

    /* PA9: alternate-function mode */
    GPIOA_MODER &= ~(3U << 18);
    GPIOA_MODER |=  (2U << 18);

    /* PA9: AF1 = USART1_TX */
    GPIOA_AFRH &= ~(0xFU << 4);
    GPIOA_AFRH |=  (1U << 4);

    /* PA10: alternate-function mode */
    GPIOA_MODER &= ~(3U << 20);
    GPIOA_MODER |=  (2U << 20);

    /* PA10: AF1 = USART1_RX */
    GPIOA_AFRH &= ~(0xFU << 8);
    GPIOA_AFRH |=  (1U << 8);

    /* 115200 baud, assuming an 8 MHz USART clock */
    USART1_BRR = 69U;

    /* Enable transmitter, receiver and USART */
    USART1_CR1 |= (1U << 3);
    USART1_CR1 |= (1U << 2);
    USART1_CR1 |= (1U << 0);
}

static char usart1_read_character(void) {
    while((USART1_ISR & (1U << 5)) == 0U){}
    return (char)USART1_RDR;
}

static void usart1_write_character(char c) {
    while((USART1_ISR & (1U << 7)) == 0U) {}
    USART1_TDR = (uint32_t)c;
}

static void usart1_write_string(const char *s) {
    while(*s != '\0') {
        usart1_write_character(*s);
        s++;
    }
}

//$ picocom --echo -b 115200 /dev/ttyUSB0
//crt+a crt+x
void main(void) {
    usart1_init();
    usart1_write_string("UART echo ready!\r\n");
    while(1) {
        char c = usart1_read_character();
        usart1_write_string("\r\n");
        usart1_write_character(c);
        usart1_write_string("\r\n");

    }
}