#include "gpio.h"

#include <stdint.h>

#define RCC_AHBENR   (*(volatile uint32_t *)0x40021014U)
#define GPIOC_MODER  (*(volatile uint32_t *)0x48000800U)
#define GPIOC_BSRR   (*(volatile uint32_t *)0x48000818U)

#define GPIOC_CLOCK_ENABLE (1U << 19)
#define LED_PIN            13U

/*
 * Set to 1 for an active-low LED:
 *   GPIO LOW  = LED on
 *   GPIO HIGH = LED off
 *
 * Set to 0 if your board's LED is active-high.
 */
#define LED_ACTIVE_LOW 1

void gpio_init(void)
{
    RCC_AHBENR |= GPIOC_CLOCK_ENABLE;

    /* PC13: output mode (01). */
    GPIOC_MODER &= ~(3U << (LED_PIN * 2U));
    GPIOC_MODER |=  (1U << (LED_PIN * 2U));

    gpio_set_led(false);
}

void gpio_set_led(bool on)
{
#if LED_ACTIVE_LOW
    if (on) {
        /* Reset PC13: output LOW, LED on. */
        GPIOC_BSRR = (1U << (LED_PIN + 16U));
    } else {
        /* Set PC13: output HIGH, LED off. */
        GPIOC_BSRR = (1U << LED_PIN);
    }
#else
    if (on) {
        GPIOC_BSRR = (1U << LED_PIN);
    } else {
        GPIOC_BSRR = (1U << (LED_PIN + 16U));
    }
#endif
}