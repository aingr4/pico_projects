#include <stdio.h>
#include "pico/stdlib.h"


const uint LED_PIN_RED = 18;
const uint LED_PIN_ORANGE = 19;
const uint LED_PIN_GREEN = 20;

const uint BUTTON_PIN_1 = 13;
const uint BUTTON_PIN_2 = 8;
const uint BUTTON_PIN_3 = 3;

void TurnOnLED(int ledPin);
void TurnOffLED(int ledPin);

int main()
{
    stdio_init_all();

    // initialize gpio output pins
    gpio_init(LED_PIN_RED);
    gpio_set_dir(LED_PIN_RED, GPIO_OUT);

    gpio_init(LED_PIN_ORANGE);
    gpio_set_dir(LED_PIN_ORANGE, GPIO_OUT);



    // initialize gpio input pins
    gpio_init(BUTTON_PIN_1);
    gpio_set_dir(BUTTON_PIN_1, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_1);

    gpio_init(BUTTON_PIN_2);
    gpio_set_dir(BUTTON_PIN_2, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_2);

    gpio_init(BUTTON_PIN_3);
    gpio_set_dir(BUTTON_PIN_3, GPIO_IN);
    gpio_pull_down(BUTTON_PIN_3);

    while (1)
    {
        if (gpio_get(BUTTON_PIN_1))
        {
            TurnOnLED(LED_PIN_GREEN);
        }
        else if (gpio_get(BUTTON_PIN_2))
        {
            TurnOnLED(LED_PIN_ORANGE);
        }
        else if (gpio_get(BUTTON_PIN_3))
        {
            TurnOnLED(LED_PIN_RED);
        }
        else
        {
            TurnOffLED(LED_PIN_ORANGE);
            TurnOffLED(LED_PIN_RED);
            TurnOffLED(LED_PIN_GREEN);
        }
    }
    return 0;
}

void TurnOnLED(int ledPin)
{
    puts("Toggling LED RED");
    gpio_put(ledPin, 1);
}

void TurnOffLED(int ledPin)
{
    puts("Toggling LED RED");
    gpio_put(ledPin, 0);
}

void ButtonPressed(int buttonPin)
{

}
