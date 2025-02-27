#include <stdint.h>
#include "stm32g031xx.h"
#include "timer.h"

uint32_t SystemCoreClock = 16000000;
uint32_t ticks = 0;

void SYSTICK_Init(void){
	SysTick_Config(SystemCoreClock / 1000);
}

void SYSTICK_Delay(uint32_t Delay)
{
	uint32_t tickstart = SYSTICK_Get();
	uint32_t wait = Delay;

	while((SYSTICK_Get() - tickstart) < wait);
}

uint32_t SYSTICK_Get(void){
	return ticks;
}

void TIM2_Init(void){
	// Init TIM2
	RCC->APBENR1 |= RCC_APBENR1_TIM2EN; 			// Activer l'horloge TIM2
	TIM2->PSC = 15; 								// Diviseur pour 1 µs (ajuster selon la fréquence système)
	TIM2->ARR = 1000000 - 1; 						// Période d'une seconde
	TIM2->CNT = 0; 									// Réinitialiser le compteur
	TIM2->CR1 |= TIM_CR1_CEN; 						// Activer le compteur
}

void delay_us(uint16_t us) {
	TIM2->CNT = 0; 									// Réinitialiser le compteur
	while (TIM2->CNT < us); 						// Attendre que le compteur atteigne la valeur
}

void delay_ms(uint16_t ms) {
	uint16_t i;
	for (i = 0; i < ms; i++) {
		delay_us(1000); 				// Appeler delay_us pour 1000 µs (soit 1 ms)
	}
}
