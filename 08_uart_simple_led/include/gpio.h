#ifndef GPIO_H
#define GPIO_H

#include <stdbool.h>

void gpio_init(void);
void gpio_set_led(bool on);

#endif