#include "pit.h";
#define RED_LED_PIN_BOARD (18) 
#define GREEN_LED_PIN_BOARD (19) 
#define BLUE_LED_PIN_BOARD (1)

uint16_t state_board = 3;

void pit_init(void)
{
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	PIT_MCR &= ~PIT_MCR_MDIS_MASK; 
	PIT->MCR |= PIT_MCR_FRZ_MASK; 
	
	PIT->CHANNEL[0].LDVAL =0x1CFFD40;
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK; 
	
	NVIC_ClearPendingIRQ(PIT_IRQn);
	NVIC_SetPriority(PIT_IRQn,5);
	NVIC_EnableIRQ(PIT_IRQn);
}

void LED_Sequence(int n)
{
    switch(n)
    {
        case 0: 
            // Red On
            PTB->PCOR = (1 << RED_LED_PIN_BOARD); 
            PTB->PSOR = (1 << GREEN_LED_PIN_BOARD);
            PTD->PSOR = (1 << BLUE_LED_PIN_BOARD);  
            break;
        case 1: 
            // Blue On
            PTB->PSOR = (1 << RED_LED_PIN_BOARD); 
            PTB->PSOR = (1 << GREEN_LED_PIN_BOARD);
            PTD->PCOR = (1 << BLUE_LED_PIN_BOARD);  
            break;
        case 2: 
            // Yellow On (Red + Green)
            PTB->PCOR = (1 << RED_LED_PIN_BOARD); 
            PTB->PCOR = (1 << GREEN_LED_PIN_BOARD);
            PTD->PSOR = (1 << BLUE_LED_PIN_BOARD);  
            break;
        case 3: 
            // All Off (Black)
            PTB->PSOR = (1 << RED_LED_PIN_BOARD); 
            PTB->PSOR = (1 << GREEN_LED_PIN_BOARD);
            PTD->PSOR = (1 << BLUE_LED_PIN_BOARD);  
            break;
        default:
            // Default to all off or handle error
            PTB->PSOR = (1 << RED_LED_PIN_BOARD); 
            PTB->PSOR = (1 << GREEN_LED_PIN_BOARD);
            PTD->PSOR = (1 << BLUE_LED_PIN_BOARD);
    }
}

void led_board_init(void){

	//leds on board
	PORTB->PCR[RED_LED_PIN_BOARD] = ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_PIN_BOARD] |= PORT_PCR_MUX(1);
	PTB->PDDR |= (1 << RED_LED_PIN_BOARD);
	PTB->PSOR = (1 << RED_LED_PIN_BOARD);
	
	PORTB->PCR[GREEN_LED_PIN_BOARD] = ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_PIN_BOARD] |= PORT_PCR_MUX(1);
	PTB->PDDR |= (1 << GREEN_LED_PIN_BOARD);
	PTB->PSOR = (1 << GREEN_LED_PIN_BOARD);
	
	PORTD->PCR[BLUE_LED_PIN_BOARD] = ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_PIN_BOARD] |= PORT_PCR_MUX(1);
	PTD->PDDR |= (1 << BLUE_LED_PIN_BOARD);
	PTD->PSOR = (1 << BLUE_LED_PIN_BOARD);

	
	
}

void PIT_IRQHandler(void) {
	if(PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		state_board ++;
		LED_Sequence(state_board%4);
		PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
	}
}
