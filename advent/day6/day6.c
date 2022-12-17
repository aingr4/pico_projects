#include <stdio.h>
#include "pico/stdlib.h"
#include "picoCommon.h"
#include "hardware/adc.h"

#define PHOTO_SENSOR_PIN 27
#define PHOTO_SENSOR_ADC_INPUT 1

int main()
{
    stdio_init_all();
    picoInit();

    // setup adc
    adc_init();
    adc_gpio_init(PHOTO_SENSOR_PIN);
    adc_select_input(PHOTO_SENSOR_ADC_INPUT);

    while(1)
    {
        uint photoSensorVal = adc_read();
        printf("Photo sensor value: 0x%03x, Voltage: %f V, Percentage: %f%%\n", photoSensorVal, ADC_VALUE_TO_VOLTAGE(photoSensorVal), ADC_TO_PERCENTAGE(photoSensorVal));
        sleep_ms(100);
    }

    return 0;
}
