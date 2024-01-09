#include "MKL25Z4.h"
void RGBLed_Init(void);
void Control_LEDs(uint8_t state);
void Test_LED(void);
int Is_LED_On(uint8_t led_pin, GPIO_Type *port);
