#include "uart.h"
#include <stdio.h>  // For sprintf

void UART0_Transmit(uint8_t data) {
	// Utilizam masca TDRE pentru a verifica
	// disponibilitatea buffer-ului de transmisie
	while(!(UART0->S1 & UART_S1_TDRE_MASK)) {}
	UART0->D = data;
}

uint8_t UART0_Receive(void) {
	// Utilizam masca RDRF pentru a verifica
	// disponibilitatea buffer-ului de receptie
	while(!(UART0->S1 & UART_S1_RDRF_MASK)) {}
	return UART0->D;
}

void delay_ms(uint32_t milliseconds) {
    for(uint32_t i = 0; i < milliseconds * 1000; ++i) {
        // This loop will need to be calibrated based on your system's clock speed
        __NOP();  // No Operation - does nothing but consume time
    }
}


// Add a function to check if data is available
uint8_t UART0_DataAvailable(void) {
    return (UART0->S1 & UART_S1_RDRF_MASK);
}

void UART0_Transmit_String(const char* str) {
    while (*str) {
        UART0_Transmit(*str++);
    }
}

void UART0_Transmit_Number(uint16_t num) {
    char buffer[10];  // Large enough for a 16-bit number
    sprintf(buffer, "%u\r\n", num);  // Convert number to string
    UART0_Transmit_String(buffer);  // Transmit the string
}


void UART0_Initialize(uint32_t baud_rate) {
	
	uint16_t osr = 16;
	uint16_t sbr;
	
	SIM->SCGC4 = SIM->SCGC4 | SIM_SCGC4_UART0_MASK;

	// Activarea semnalului de ceas pentru portul A
	// PTA1 - receptie UART0
	// PTA2 - transmisie UART0
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
	// Dezactivare receptor si emitator pentru realizarea configuratiilor
	UART0->C2 &= ~((UART0_C2_RE_MASK) | (UART0_C2_TE_MASK)); 
	
	// Setarea sursei de ceas pentru modulul UART la 48MHz
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(01);
	
	// Configurarea pinilor pentru emisie/receptie
	PORTA->PCR[1] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[1] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // RX
	PORTA->PCR[2] = ~PORT_PCR_MUX_MASK;
	PORTA->PCR[2] = PORT_PCR_ISF_MASK | PORT_PCR_MUX(2); // TX
	
	
	// Setarea baud rate-ului si a ratei de supraesantionare
	sbr = (uint16_t)((DEFAULT_SYSTEM_CLOCK)/(baud_rate * (osr)));
	UART0->BDH &= UART0_BDH_SBR_MASK;
	UART0->BDH |= UART0_BDH_SBR(sbr>>8);
	UART0->BDL = UART_BDL_SBR(sbr);
	UART0->C4 |= UART0_C4_OSR(osr - 1);
			
	// Setarea numarului de biti de date la 8 si fara bit de paritate
	UART0->C1 = UART0_C1_M(0) | UART0_C1_PE(0);
	
	// Transmisie incepand cu LSB
	UART0->S2 = UART0_S2_MSBF(0);
	
	UART0->C2 |= ((UART_C2_RE_MASK) | (UART_C2_TE_MASK));
		
}
