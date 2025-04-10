//------Firmware OneBee----//
//			v-0.1.1

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
#include "FLASH.h"
//-----Constante-----//

//---En-Tête de Fonction---//

//-----Programme Principal-----//
int main(void)
{
	uint16_t Temperature = 0;
	uint32_t Poid = 0;
	tare_user = (uint32_t) read_flash_memory(ROMADDR);

	GPIO_Init();
	TIM2_Init();
	SYSTICK_Init(); // TODO use only one timer, BUG with TIM2 ???
	RTC_Init();

	DS18B20_Init();
	HX711_Init();
	LORAWAN_Join(); //Alim coupée, donc nécessaire.

	//Mesure des grandeur
	Temperature = DS18B20_GetTemp();

	//GPIOA->ODR |= 1 << GPIO_ODR_OD0_Pos; //LED D6

	Poid = HX_Run(tare_user);

	LORAWAN_Send(Poid, Temperature);

	//TODO maintenir PB5 a HIGH, et Data DS18B20 a LOW
	//GPIOA->ODR &= ~GPIO_ODR_OD0_Msk; //LED D6

//	if((uint32_t) read_flash_memory(ROMADDR) != tare_user && tare_user != 0) {
//		flash_erase_page(ROMADDR);
//		flash_write(ROMADDR, (uint64_t) tare_user);
//	}
	enter_low_power_standby_mode();
	//DO NOT ADD CODE HERE AND AFTER
}


