

#ifndef TIMER_H_
#define TIMER_H_

#include <stdint.h>

void SYSTICK_Init(void);
void SYSTICK_Delay(uint32_t Delay);
uint32_t SYSTICK_Get(void);

void TIM2_Init(void);

void delay_us(uint16_t us);

void delay_ms(uint16_t ms);

#endif
