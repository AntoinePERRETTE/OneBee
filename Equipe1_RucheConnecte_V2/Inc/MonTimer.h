#ifndef __MonTimer_h__
#define __MonTimer_h__

#include <stm32g031xx.h>


void Timer3Init(void);
void Tim3Flag_ms(uint32_t x_uint32_delay);
uint8_t get_CptFlag3(void);
void TIM3_Delay_ms(uint32_t x_uint32_delay);

//typedef union{
//    uint32_t m_uint32_registre;
//    struct{
//        uint16_t m_uint16_tmp;
//        uint16_t m_uint16_CptVal : 15;
//    };
//}T_U;

#endif
