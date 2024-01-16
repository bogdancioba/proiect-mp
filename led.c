#include "led.h";
#define RED_LED_PIN (12) // PORT A
#define GREEN_LED_PIN (4) // PORT D
#define YELLOW_LED_PIN (8) // PORT C



int Is_LED_On(uint8_t led_pin, GPIO_Type *port) {
    // Check if the pin is set or cleared
    // Assuming active-high LED configuration
    return (port->PDIR & (1 << led_pin)) != 0 ? 1 : 0;
}

void Control_LEDs(uint8_t state) {
    // Turn off all LEDs first
    GPIOA_PSOR |= (1<<RED_LED_PIN); // Red LED off
    GPIOD_PSOR |= (1<<GREEN_LED_PIN); // Green LED off (Port B)
    GPIOC_PSOR |= (1<<YELLOW_LED_PIN); // Blue LED off
		int greenStatus = 0;
		int yellowStatus = 0;
    switch(state) {
        case 1: // Only Red LED on
					  greenStatus = Is_LED_On(GREEN_LED_PIN, GPIOD);
						if(greenStatus == 1)
						{
							GPIOD_PCOR |= (1<<GREEN_LED_PIN);
						}
						yellowStatus = Is_LED_On(YELLOW_LED_PIN, GPIOC);
						if(yellowStatus == 1)
						{
							GPIOC_PCOR |= (1<<YELLOW_LED_PIN);
						}
            break;
        case 2: // Red and Green LEDs on
						greenStatus = Is_LED_On(GREEN_LED_PIN, GPIOD);
						if(greenStatus == 0)
						{
							GPIOD_PCOR |= (1<<GREEN_LED_PIN);
						}
						yellowStatus = Is_LED_On(YELLOW_LED_PIN, GPIOC);
						if(yellowStatus == 1)
						{
							GPIOC_PCOR |= (1<<YELLOW_LED_PIN);
						}						
						break;
        case 3: // All LEDs on
            yellowStatus = Is_LED_On(YELLOW_LED_PIN, GPIOC);
						if(yellowStatus == 0)
						{
							GPIOC_PCOR |= (1<<YELLOW_LED_PIN);
						}	
            break;
        default: // All LEDs off
            break;
    }
}



void Test_LED(void){
    // Assuming RED_LED_PIN is the pin you want to test
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;  // Enable clock to Port B
    PORTC->PCR[YELLOW_LED_PIN] = PORT_PCR_MUX(1);  // Set pin as GPIO
    GPIOC_PDDR |= (1<<YELLOW_LED_PIN);  // Set pin as output

    // Toggle LED
    while(1) {
        GPIOC_PCOR |= (1<<YELLOW_LED_PIN);  // Turn on LED
        for(volatile int i=0; i<1000000; i++);  // Delay
        GPIOC_PSOR |= (1<<YELLOW_LED_PIN);  // Turn off LED
        for(i=0; i<1000000; i++);  // Delay
    }
}

void RGBLed_Init(void){
	
    // Activating the clock signal for the pins used in the RGB LED
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTB_MASK;;
    
    // --- RED LED on Port A ---
    PORTA->PCR[RED_LED_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[RED_LED_PIN] |= PORT_PCR_MUX(1);
    GPIOA_PDDR |= (1<<RED_LED_PIN);
    //GPIOA_PSOR |= (1<<RED_LED_PIN);
	
    // --- GREEN LED on Port D ---
    PORTD->PCR[GREEN_LED_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[GREEN_LED_PIN] |= PORT_PCR_MUX(1);
    GPIOD_PDDR |= (1<<GREEN_LED_PIN);
    //GPIOD_PSOR |= (1<<GREEN_LED_PIN);
    
    // --- BLUE LED on Port D ---
    PORTC->PCR[YELLOW_LED_PIN] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[YELLOW_LED_PIN] |= PORT_PCR_MUX(1);
    GPIOC_PDDR |= (1<<YELLOW_LED_PIN);
    //GPIOC_PSOR |= (1<<YELLOW_LED_PIN);
}




