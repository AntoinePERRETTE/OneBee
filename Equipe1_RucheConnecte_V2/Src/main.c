//------Firmware OneBee----//
//			v-0.2.5

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
	uint16_t valueFromDownLink = 0;
	uint32_t tare = (uint32_t) read_flash_memory(ROMADDRTARE);
	uint16_t nbOfMeasurePerDay = (uint16_t) read_flash_memory(ROMADDRNBOFMEASURE);
	uint8_t isTareRequested = 0;

	GPIO_Init();
	GPIO_write(0, 1, GPIOA); //LED 6
	GPIO_write(5, 1, GPIOB);
	GPIO_SleepStatusInit();

	TIM2_Init();
	Timer3Init();	//For timeout in LORA
	SYSTICK_Init();
	RTC_Init();

	LORAWAN_Join(); //Alim coupée, donc nécessaire.

	DS18B20_init();
	HX_init();

	//Mesure des grandeur
	Temperature = DS18B20_getTemp();

	Poid = HX_getData(tare);

	if (WasReceived("+JOIN: Done")) {
		//Envoi sur TTN
		LORAWAN_Send(Poid, Temperature);
		SYSTICK_Delay(10000);	//seulement pour debug

		//Si commande de Tare, alors tare effectuée
		if (WasReceivedForSend("Done", &valueFromDownLink)) {
			isTareRequested = (valueFromDownLink & 0x8000) > 0;

			if ((valueFromDownLink & 0x7FFF) != nbOfMeasurePerDay && (valueFromDownLink & 0x7FFF) >= 3) {
				nbOfMeasurePerDay = (valueFromDownLink & 0x7FFF);
				flash_erase_page(ROMADDRNBOFMEASURE);
				flash_write(ROMADDRNBOFMEASURE, (uint64_t) nbOfMeasurePerDay);
			}

			if (isTareRequested) {
				HX_getTare();
			}
		}
	}

	GPIO_write(0, 0, GPIOA); //LED 6
	//GPIO_write(5, 0, GPIOB);
	PWR->CR3 |= 1 << PWR_CR3_APC_Pos;

	enter_low_power_standby_mode(86400 / nbOfMeasurePerDay);
	//DO NOT ADD CODE HERE AND AFTER
}


