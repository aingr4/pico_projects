#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"

// GPIO pins
#define SPEAKER_PIN 13
#define LED_PIN_GREEN 20
#define POT_PIN 27
#define BUTTON_PIN 28

#define CLOCK_SPD 125000000

// Max voltage
#define ADC_VREF 3.3
// The voltage is in the first 12 bits of the ADC value
#define ADC_VALUE_TO_VOLTAGE(x) x * (ADC_VREF / (1 << 12))
#define ADC_MAX_VALUE 0xffff
#define ADC_TO_DUTY(x) ADC_VALUE_TO_VOLTAGE(x) / ADC_VREF * 100

void picoInit();
uint pwmSetFrequencyDuty(uint sliceNum, uint channel, uint freq, int duty);
void turnOnLED(int ledPin);
void playPwmTone(uint sliceNum, uint channel, uint note, uint8_t volume, uint delay1, uint delay2);
void playJingleBells(uint sliceNum, uint channel);
void playDunDun(uint sliceNum, uint channel);

int main()
{
    stdio_init_all();

    picoInit();

    // setup button
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_down(BUTTON_PIN);

    // setup adc
    adc_init();
    adc_gpio_init(POT_PIN);
    adc_select_input(1);

    // setup SPEAKER_PIN to be PWM
    gpio_init(SPEAKER_PIN);
    gpio_set_function(SPEAKER_PIN, GPIO_FUNC_PWM);

    uint sliceNum = pwm_gpio_to_slice_num(SPEAKER_PIN);
    uint channel = pwm_gpio_to_channel(SPEAKER_PIN);
    pwm_set_enabled(sliceNum, true);

    while (1)
    {
        if (gpio_get(BUTTON_PIN))
        {
            playDunDun(sliceNum, channel);
            pwmSetFrequencyDuty(sliceNum, channel, 1000, 0);
            sleep_ms(100); // Make sure the button debounces
        }
    }
    return 0;
}

/** \brief Set PWM frequency and duty for a given channel and slice number
 *
 * \param slice_num PWM slice number
 * \param channel  8 bit integer part of the clock divider
 * \param freq frequency of the PWM
 * \param duty duty period of the PWM, expresed as a percentage
 */
uint pwmSetFrequencyDuty(uint sliceNum, uint channel, uint freq, int duty)
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

void playJingleBells(uint sliceNum, uint channel)
{
    uint A = 440;
    uint As = 466;
    uint B = 494;
    uint C = 523;
    uint Cs = 554;
    uint D = 587;
    uint Ds = 622;
    uint E = 659;
    uint F = 698;
    uint Fs = 740;
    uint G = 784;
    uint Gs = 830;


    uint barDuration = 1000;
    uint quarterNoteDuration = barDuration / 4;
    uint halfNoteDuration = barDuration / 2;
    uint wholeNoteDuration = barDuration;

    uint halfRestDuration = barDuration / 2;
    uint wholeRestDuration = barDuration;

    uint noteEndPauseDuration = 100;
    uint linkedNotePauseDuration = 0;
    // Call playPwmTone and set the volume to the adc value converted
    // to a duty period percentage
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), halfNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), halfNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, G, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, C, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, D, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), wholeNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, D, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, D, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, D, ADC_TO_DUTY(adc_read()), halfNoteDuration, linkedNotePauseDuration);
    playPwmTone(sliceNum, channel, D, ADC_TO_DUTY(adc_read()), quarterNoteDuration, linkedNotePauseDuration);
    playPwmTone(sliceNum, channel, G, ADC_TO_DUTY(adc_read()), halfNoteDuration, linkedNotePauseDuration);
    playPwmTone(sliceNum, channel, G, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    //

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), halfNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), halfNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, G, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, C, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, D, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), wholeNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, G, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, G, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, D, ADC_TO_DUTY(adc_read()), quarterNoteDuration, noteEndPauseDuration);

    playPwmTone(sliceNum, channel, C, ADC_TO_DUTY(adc_read()), wholeNoteDuration, noteEndPauseDuration);
}

void playDunDun(uint sliceNum, uint channel)
{
    uint A = 440;
    uint As = 466;
    uint B = 494;
    uint C = 523;
    uint Cs = 554;
    uint D = 587;
    uint Ds = 622;
    uint E = 659;
    uint F = 698;
    uint Fs = 740;
    uint G = 784;
    uint Gs = 830;


    uint barDuration = 1000;
    uint eightNoteDuration = barDuration / 8;
    uint quarterNoteDuration = barDuration / 4;
    uint halfNoteDuration = barDuration / 2;
    uint wholeNoteDuration = barDuration;

    uint halfRestDuration = barDuration / 2;
    uint wholeRestDuration = barDuration;

    uint noteEndPauseDuration = 100;
    uint linkedNotePauseDuration = 0;

    playPwmTone(sliceNum, channel, G, ADC_TO_DUTY(adc_read()), eightNoteDuration, 200);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), eightNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), eightNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, F, ADC_TO_DUTY(adc_read()), eightNoteDuration, noteEndPauseDuration);
    playPwmTone(sliceNum, channel, E, ADC_TO_DUTY(adc_read()), eightNoteDuration, noteEndPauseDuration);
}

void playPwmTone(uint sliceNum, uint channel, uint note, uint8_t volume, uint delay1, uint delay2)
{
    pwmSetFrequencyDuty(sliceNum, channel, note, volume);
    sleep_ms(delay1);
    pwmSetFrequencyDuty(sliceNum, channel, note, 0);
    sleep_ms(delay2);
}

void picoInit()
{
    // setup LED to show that we are running a program
    gpio_init(LED_PIN_GREEN);
    gpio_set_dir(LED_PIN_GREEN, GPIO_OUT);
    turnOnLED(LED_PIN_GREEN);
}

void turnOnLED(int ledPin)
{
    gpio_put(ledPin, 1);
}
