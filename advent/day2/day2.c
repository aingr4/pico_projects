#include <stdio.h>
#include "pico/stdlib.h"


const uint LED_PIN_RED = 18;
const uint LED_PIN_ORANGE = 19;
const uint LED_PIN_GREEN = 20;

int main()
{
    stdio_init_all();

    // initialize gpio pins
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);

    gpio_init(LED_PIN_ORANGE);
    gpio_set_dir(LED_PIN_ORANGE, GPIO_OUT);

    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);

    uint set = 0x1;

    while (1)
    {
        set ^= 1;
        puts("Toggling LED RED");
        gpio_put(LED_PIN_RED, set);
        sleep_ms(1000);
        puts("Toggling LED ORANGE");
        gpio_put(LED_PIN_ORANGE, set); 
        sleep_ms(1000);
        puts("Toggling LED GREEN");
        gpio_put(LED_PIN_GREEN, set);
        sleep_ms(1000);
    }
    return 0;
}
