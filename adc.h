#ifndef ADC_H
#define ADC_H

#include <stdint.h>

void ADC0_Init(void);
uint8_t ADC0_ReadChannel(uint8_t channel);

#endif // ADC_H
