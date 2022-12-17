#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "picoCommon.h"

void picoInit()
{
    // setup LED to show that we are running a program
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    picoTurnOnLED(LED_PIN_GREEN);
}

void picoTurnOnLED(int ledPin)
{
    gpio_put(ledPin, 1);
}

void picoTurnOffLED(int ledPin)
{
    gpio_put(ledPin, 0);
}

uint picoPwmSetFrequencyDuty(uint sliceNum, uint channel, uint freq, int duty)
{
    uint wrap = 0;
    uint clock = CLOCK_SPD;
    uint divider16 = clock / freq / 4096 + (clock % (freq * 4096) != 0);

    if (divider16 / 16 == 0)
        divider16 = 16;

    wrap = clock * 16 / divider16 / freq - 1;

    pwm_set_clkdiv_int_frac(sliceNum, divider16/16, divider16 & 0xF);
    pwm_set_wrap(sliceNum, wrap);
    pwm_set_chan_level(sliceNum, channel, wrap * duty / 100);

    return wrap;
}
