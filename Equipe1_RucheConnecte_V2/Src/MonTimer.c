#include "MonTimer.h"

/**
 * @brief initialise le timer 2
 */
void Timer2Init(void)
{
	//active la clock sur le périphérique TIM2
	RCC->APBENR1 |= RCC_APBENR1_TIM2EN;
	//defini le prescaler(prescaler-1)
	TIM2->PSC = 15;
	//fixe le prescaler
	TIM2->EGR = TIM_EGR_UG;
	//met à zero le compteur ainci que le flag associer
	TIM2->CNT = 0;
	TIM2->SR &= ~(1 << 0);
	//lance le timer TIM2
	TIM2->CR1 |= (1<<0);
}

/**
 * @brief initialise le timer 3
 */
void Timer3Init(void)
{
	//active la clock sur le périphérique TIM2
	RCC->APBENR1 |= RCC_APBENR1_TIM3EN;
	//defini le prescaler(prescaler-1)
	TIM3->PSC = 15999;
	//fixe le prescaler
	TIM3->EGR = TIM_EGR_UG;
	//met à zero le compteur ainci que le flag associer
	TIM3->CNT = 0;
	TIM3->SR &= ~(1 << 0);
	//lance le timer TIM2
	TIM3->CR1 |= (1<<0);
}

/**
 * @brief déclanche un comptage en us sur TIM2
 */
void Tim2Flag_us(uint32_t x_uint32_delay)
{
	TIM2->ARR = x_uint32_delay;
	TIM2->CNT = 0;
	TIM2->SR &= ~(1 << 0);
}

/**
 * @brief déclanche un comptage en ms sur TIM3
 */
void Tim3Flag_ms(uint32_t x_uint32_delay)
{
	TIM3->ARR = x_uint32_delay;
	TIM3->CNT = 0;
	TIM3->SR &= ~(1 << 0);
}

/**
 * @brief scrute la valeur du flag d'overflow du compteur lié au timer 2
 * 
 * @return la valeur du flag (bool)
 */
uint8_t get_CptFlag2(void)//getFlagOfCompteur
{
	return (TIM2->SR & (1<<0)) != 0;
}

/**
 * @brief scrute la valeur du flag d'overflow du compteur lié au timer 3
 *
 * @return la valeur du flag (bool)
 */
uint8_t get_CptFlag3(void)
{
	return (TIM3->SR & (1<<0)) != 0;
}

/**
 * @brief TIM2_Delay_us réalise une attante en us
 *
 * @param x_uint32_delay variable contenent la valeur de l'attante en us
 */
void TIM2_Delay_us(uint32_t x_uint32_delay)
{
	TIM2->ARR = 0xffffffff;
	TIM2->CNT = 0;
	while(TIM2->CNT < x_uint32_delay);
}

/**
 * @brief TIM2_Delay_us réalise une attante en ms
 *
 * @param x_uint32_delay variable contenent la valeur de l'attante en ms
 */
void TIM3_Delay_ms(uint32_t x_uint32_delay)
{
	TIM3->ARR = 0xffff;
	TIM3->CNT = 0;
	while(TIM3->CNT < x_uint32_delay);
}
