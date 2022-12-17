
#ifndef _PICOCOMMON_H
#define _PICOCOMMON_H

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"

#define CLOCK_SPD 125000000

// Max voltage
#define ADC_VREF 3.3
// The voltage is in the first 12 bits of the ADC value
#define ADC_VALUE_TO_VOLTAGE(x) x * (ADC_VREF / (1 << 12))
#define ADC_MAX_VALUE 0xffff
#define ADC_TO_PERCENTAGE(x) ADC_VALUE_TO_VOLTAGE(x) / ADC_VREF * 100

// GPIO pins
#define SPEAKER_PIN 13
#define LED_PIN_GREEN 20
#define POT_PIN 27
#define BUTTON_PIN 28

// Function declarations
uint picoPwmSetFrequencyDuty(uint sliceNum, uint channel, uint freq, int duty);
void picoInit();
void picoTurnOnLED(int ledPin);
void picoTurnOffLED(int ledPin);

#endif /* _PICOCOMMON_H */