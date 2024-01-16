#include "adc.h"
#include "led.h"
#include "uart.h"
#include "pit.h"

#define ROTATION_SENSOR_CHANNEL 15
#define LOW_THRESHOLD 180
#define MEDIUM_THRESHOLD 252

int main(void) {
    // Initialize modules
	  ADC0_Init();
    RGBLed_Init();
		led_board_init();
		pit_init();



		//Test_LED();
    UART0_Initialize(38400);

    uint8_t adcValue;

    while (1) {
        //Check if data is available to read
        if (UART0_DataAvailable()) {
            adcValue = UART0_Receive();  // Read the received value

            //Process the ADC value
            if (adcValue < LOW_THRESHOLD) {
                Control_LEDs(1);  // Low value - Light up Red LED
            } else if (adcValue < MEDIUM_THRESHOLD) {
                Control_LEDs(2);  // Medium value - Light up Red and Green LEDs
            } else {
               Control_LEDs(3);  // High value - Light up Red, Green, and Blue LEDs
            }

            delay_ms(2000);  // Wait for 2 seconds
					
        } else {
            //If no data is received, continue using the ADC value
            adcValue = ADC0_ReadChannel(ROTATION_SENSOR_CHANNEL);
        }

        //Transmit the adcValue for debugging
        UART0_Transmit_Number(adcValue);

        //Process the ADC value
        if (adcValue < LOW_THRESHOLD) {
            //Low value - Light up Red LED
            Control_LEDs(1);
        } else if (adcValue < MEDIUM_THRESHOLD) {
            //Medium value - Light up Red and Green LEDs
            Control_LEDs(2);
        } else {
            //High value - Light up Red, Green, and Blue LEDs
            Control_LEDs(3);
        }
    }

    return 0;
}


