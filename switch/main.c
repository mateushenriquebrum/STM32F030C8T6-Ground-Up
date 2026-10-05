#include <stdbool.h>

typedef unsigned int uint32_t;

//pg 39, 0x4002 1000 - 0x4002 13FF
#define RCC_AHBENR  (*(volatile uint32_t *)0x40021014U)
//pg 39, 0x4800 0800 - 0x4800 0BFF
//pg 142, 0x00
#define GPIOC_MODER (*(volatile uint32_t *)0x48000800U)
//pg 142, 0x0C
#define GPIOC_PUPDR (*(volatile uint32_t *)0x4800080CU)
//pg 142, 0x14
#define GPIOC_ODR   (*(volatile uint32_t *)0x48000814U)
//pg 142, 0x10
#define GPIOC_IDR   (*(volatile uint32_t *)0x48000810U)

static void delay(volatile uint32_t n)
{
    while (n--)
        __asm volatile ("nop");
}

int main(void)
{
    bool was_pressed = false;
    bool is_on = false;
    /* Enable GPIOC clock */
    //pg 111
    RCC_AHBENR |= (1U << 19);

    /* PC13 = output */
    //pg 138
    GPIOC_MODER &= ~(3U << 26);
    GPIOC_MODER |=  (1U << 26);


    /* PC14 = input*/
    //pg 138
    GPIOC_MODER &= ~(3U << 28);
    //pull down
    //pg 137
    GPIOC_PUPDR &= ~(3U << 28);
    GPIOC_PUPDR |= (2U << 28);
    
    while (1)
    {
        bool pressed = GPIOC_IDR & (1U << 14);

        if (pressed & !was_pressed) {
            is_on = !is_on;
        }

        was_pressed = pressed;

        if (is_on) {
            GPIOC_ODR |= (1U << 13);
        } else {
            GPIOC_ODR &= ~(1U << 13);
        }
        delay(10000);
    }
}