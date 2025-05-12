//------Firmware OneBee----//
//			v-0.2.2

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
	T_RECEIVED_ERROR noErrorOfReception = NO_ERROR;
	uint32_t tare = (uint32_t) read_flash_memory(ROMADDRTARE);
	uint8_t nbOfMeasurePerDay = (uint8_t) read_flash_memory(ROMADDRNBOFMEASURE);
	uint8_t isTareRequested = 0;

	GPIO_Init();
	GPIOA->ODR |= 1 << GPIO_ODR_OD0_Pos; //LED D6
	GPIOB->ODR |= 1 << GPIO_ODR_OD5_Pos;
	TIM2_Init();
	Timer3Init();	//For timeout in LORA
	SYSTICK_Init(); // TODO use only one timer, BUG with TIM2 ???
	RTC_Init();

	LORAWAN_Join(); //Alim coupée, donc nécessaire.

	DS18B20_Init();
	HX711_Init();

	//Mesure des grandeur
	Temperature = DS18B20_GetTemp();

	Poid = HX_Run(tare);

	if (WasReceived("+JOIN: Done")) {
		//Envoi sur TTN
		LORAWAN_Send(Poid, Temperature);
		SYSTICK_Delay(10000);	//seulement pour debug
		//Si commande de Tare, alors tare effectuée
		noErrorOfReception = WasReceivedForSend("Done", &valueFromDownLink);
		isTareRequested = valueFromDownLink & 1;
		if (((valueFromDownLink & 0xFFFE) / 10) != nbOfMeasurePerDay && valueFromDownLink & 0xFFFE >= 10) {
			nbOfMeasurePerDay = (valueFromDownLink & 0xFFFE) / 10;
			//flash_erase_page(ROMADDRNBOFMEASURE);
			//flash_write(ROMADDRNBOFMEASURE, (uint64_t) nbOfMeasurePerDay);
		}

		if (isTareRequested) {
			doTare();
		}
	}
	//TODO TEST EVERYTHING !!

	//TODO maintenir PB5 a HIGH, et Data DS18B20 a LOW
	//voir test_Sleep -> pull down & pull up sur gpio quand µC en standby
	GPIOA->ODR &= ~GPIO_ODR_OD0_Msk; //LED D6
	GPIOB->ODR &= ~GPIO_ODR_OD5_Msk;

	enter_low_power_standby_mode(86400 / nbOfMeasurePerDay);
	//DO NOT ADD CODE HERE AND AFTER
}


