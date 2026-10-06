typedef unsigned int uint32_t;

//pg 39, 0x4002 1000 - 0x4002 13FF
#define RCC_AHBENR  (*(volatile uint32_t *)0x40021014U)
//pg 39, 0x4800 0800 - 0x4800 0BFF
//pg 142, 0x00
#define GPIOC_MODER (*(volatile uint32_t *)0x48000800U)
//pg 142, 0x14
#define GPIOC_ODR   (*(volatile uint32_t *)0x48000814U)

static void delay(volatile uint32_t n)
{
    while (n--)
        __asm volatile ("nop");
}

int main(void)
{
    int wait = 50000;
    /* Enable GPIOC clock */
    //pg 111
    RCC_AHBENR |= (1U << 19);

    /* PC13 = output */
    //pg 138
    GPIOC_MODER &= ~(3U << 26);
    GPIOC_MODER |=  (1U << 26);
    
    while (1)
    {
        GPIOC_ODR ^= (1U << 13);
        delay(wait);
    }
}