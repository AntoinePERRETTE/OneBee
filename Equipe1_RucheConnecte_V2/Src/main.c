//------Firmware OneBee----//
//			v-0.1

//------Library-----//
#include "main.h"
#include "gpio.h"
#include "timer.h"
#include <stdint.h>
#include <stm32g031xx.h>
#include <stdio.h>

//-----En-tête de projet-----//
#include "DS18B20.h"
#include "HX711.h"
#include "LORAWAN.h"
#include "SleepMode.h"
//-----Constante-----//

//---En-Tête de Fonction---//

//-----Programme Principal-----//
int main(void)
{
	uint16_t Temperature = 0;
	uint32_t Poid = 0;

	GPIO_Init();
	TIM2_Init();
	SYSTICK_Init(); // TODO use only one timer
	RTC_Init();

	DS18B20_Init();
	HX711_Init();
	LORAWAN_Join(); //TODO if already join, don't

	//Mesure des grandeur
	Temperature = DS18B20_GetTemp();
	GPIOA->ODR |= 1 << GPIO_ODR_OD0_Pos;

	Poid = HX_Run(tare_user);

	LORAWAN_Send(Poid, Temperature);
	//TODO maintenir PB5 a HIGH, et Data DS18B20 a LOW

	//TODO IF TARE != TARE_IN_ROM := SAVE TARE ?? really necessary?
	GPIOA->ODR &= ~GPIO_ODR_OD0_Msk;
	enter_low_power_standby_mode();
}
