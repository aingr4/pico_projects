#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// Max voltage
#define ADC_VREF 3.3
// The voltage is in the first 12 bits of the ADC value
#define ADC_VALUE_TO_VOLTAGE(x) x * (ADC_VREF / (1 << 12))

const uint POT_PIN = 27;
const uint LED_PIN_GREEN = 20;
const uint LED_PIN_RED = 18;

void turnOnLED(int ledPin);

int main()
{
    stdio_init_all();

    // setup LED to show we are running
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    turnOnLED(LED_PIN_GREEN);

    // setup adc
    adc_init();
    adc_gpio_init(POT_PIN);
    adc_select_input(1);

    // setup LED_PIN_RED to be PWM
    gpio_set_function(LED_PIN_RED, GPIO_FUNC_PWM);
    uint sliceNum = pwm_gpio_to_slice_num(LED_PIN_RED);
    // Max value of the pwm
    pwm_set_wrap(sliceNum, 4095);
    pwm_set_chan_level(sliceNum, PWM_CHAN_A, 1);
    pwm_set_chan_level(sliceNum, PWM_CHAN_B, 3);
    pwm_set_enabled(sliceNum, true);

    uint16_t potvalue = 0;

    while (1)
    {
        potvalue = adc_read();
        printf("Pot Value: 0x%03x, Voltage: %f V\n", potvalue, ADC_VALUE_TO_VOLTAGE(potvalue));
        
        pwm_set_gpio_level(LED_PIN_RED, potvalue);
        sleep_ms(1);
    }
    return 0;
}

void turnOnLED(int ledPin)
{
    gpio_put(ledPin, 1);
}

