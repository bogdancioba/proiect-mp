#include "adc.h"
#include "led.h"
#include "uart.h"

#define ROTATION_SENSOR_CHANNEL 15

#define LOW_THRESHOLD 180
#define MEDIUM_THRESHOLD 252

int main(void) {
    // Initialize modules
    ADC0_Init();
    RGBLed_Init();
    UART0_Initialize(38400);
		//Test_LED();
    while (1) {
        // Read the ADC value
        uint8_t adcValue = ADC0_ReadChannel(ROTATION_SENSOR_CHANNEL);
        UART0_Transmit_Number(adcValue);

        // Process the ADC value
        if (adcValue < LOW_THRESHOLD) {
            // Low value - Light up Red LED
            Control_LEDs(1);
        } else if (adcValue < MEDIUM_THRESHOLD) {
            // Medium value - Light up Red and Green LEDs
            Control_LEDs(2);
        } else {
            // High value - Light up Red, Green, and Blue LEDs
            Control_LEDs(3);
        }
    }

    return 0;
}
