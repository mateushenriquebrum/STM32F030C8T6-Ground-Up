#include <stdbool.h>

typedef unsigned int uint32_t;

//pg 39, 0x4002 1000 - 0x4002 13FF, 0x14
#define RCC_AHBENR  (*(volatile uint32_t *)0x40021014U)
//pg 39, 0x4800 0800 - 0x4800 0BFF
//pg 142, 0x00
#define GPIOC_MODER (*(volatile uint32_t *)0x48000800U)
//pg 142, 0x14
#define GPIOC_ODR   (*(volatile uint32_t *)0x48000814U)
//pg 142, 0x10
#define GPIOC_IDR   (*(volatile uint32_t *)0x48000810U)
//pg 142, 0x0C
#define GPIOC_PUPDR (*(volatile uint32_t *)0x4800080CU)
//pg 39, 0x4002 1000 - 0x4002 13FF, 0X1
#define RCC_APB1ENR (*(volatile uint32_t *)0x4002101CU)
//pg 41, 0x4000 0400 - 0x4000 07FF
//pg 359, 0X00
#define TIM3_CR1    (*(volatile uint32_t *)0x40000400U)
//pg 359, 0X10
#define TIM3_SR     (*(volatile uint32_t *)0x40000410U)
//pg 359, 0X28
#define TIM3_PSC    (*(volatile uint32_t *)0x40000428U)
//pg 359, 0X2C
#define TIM3_ARR    (*(volatile uint32_t *)0x4000042CU)
//pg 359, 0x0C
#define TIM3_DIER   (*(volatile uint32_t *)0x4000040CU)
//pg 39 0xE000 0000 - 0xE00F FFFF, 
#define NVIC_ISER   (*(volatile uint32_t *)0xE000E100U)

//pg 39, 0x4002 1000 - 0x4002 13FF
//pg 125, 0x18
#define RCC_APB2ENR (*(volatile uint32_t *)0x40021018U)

//pg 40, 0x4001 0000 - 0x4001 03FF
#define SYSCFG_EXTICR4 (*(volatile uint32_t *) 0x40010014U)
//pg 40, 0x4001 0400 - 0x4001 07FF
//pg 180, 0x00
#define EXTI_IMR (*(volatile uint32_t *) 0x40010400U)
//pg 40, 0x4001 0400 - 0x4001 07FF
//pg 180, 0x08
#define EXTI_RTSR (*(volatile uint32_t *) 0x40010408U)
//pg 40, 0x4001 0400 - 0x4001 07FF
//pg 180, 0x14
#define EXTI_PR (*(volatile uint32_t *)0x40010414U)

volatile bool bliking = false;

int main(void)
{
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

    /* PC14 = pull-down */
    GPIOC_PUPDR &= ~(3U << 28);
    GPIOC_PUPDR |=  (2U << 28);

    /* Enable Timer 3*/
    //pg 125
    RCC_APB1ENR |= (1U << 1);
    
    //8MHz / 7999 + 1 = 1KHz
    TIM3_PSC = 7999;

    // 250 ms
    TIM3_ARR = 249;
    
    //pg 339
    TIM3_CR1 |= (1U << 0);

    //pg 343
    TIM3_DIER |= (1U << 0);

    //pg 170
    NVIC_ISER |= (1U << 16);

    /* Enable SYSCFG*/
    //pg 125
    RCC_APB2ENR |= (1U << 0);

    /*Enable EXTI14 for PC*/
    //pg 151
    SYSCFG_EXTICR4 &= ~(0xFU << 8); //00
    SYSCFG_EXTICR4 |=  (0x2U << 8); //10

    /*Allow emit event*/
    //pg 180, 0x00
    EXTI_IMR |= (0x1U << 14);
    /*Rising trigger*/
    //pg 180, 0x08
    EXTI_RTSR |= (0x1U << 14);

    /*Enable interruption 7 in cortex-m0 */
    NVIC_ISER |= (1U << 7);

    while(1) {}
   
}

void TIM3_IRQHandler(void)
{
    if (TIM3_SR & (1U << 0)){
        TIM3_SR &= ~(1U << 0);

        if (bliking){
            GPIOC_ODR ^= (1U << 13);
        }
    }
}

void EXTI4_15_IRQHandler(void) {
    if (EXTI_PR & (1U << 14)){
        EXTI_PR |= (1U << 14);
        GPIOC_ODR &= ~(1U << 13);
        bliking = !bliking;
    }
}