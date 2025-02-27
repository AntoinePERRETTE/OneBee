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
//-----Constante-----//

//-----Programme Principale-----//
int main(void)
{
	uint16_t Temperature = 0;
	uint32_t Poid = 0;

	GPIO_Init();
	SYSTICK_Init();
	TIM2_Init();

	DS18B20_Init();
	HX711_Init();
	LORAWAN_Join();

	while(1){
		/*Verification de l'instalation des périphériques

		//LORAWAN
		int at_send_check_response(const char *p_ack, int timeout_ms, const char *p_cmd, ...) {
			char cmd_buffer[256];
			va_list args;

			va_start(args, p_cmd);
			vsnprintf(cmd_buffer, sizeof(cmd_buffer), p_cmd, args);
			va_end(args);

			UART_Send("%s", cmd_buffer); // Envoyer la commande
			memset(recv_buf, 0, sizeof(recv_buf));
			UART_Receive(recv_buf, timeout_ms);

			if (strstr(recv_buf, p_ack) != NULL) { // Vérifier si la réponse contient p_ack
				return 1;
			}
			return 0;
		}
		//HX711

		//DS18B20
		if(DS18B20_reset(void)){

		}
		*/

		//Mesure des grandeur
		Temperature = DS18B20_GetTemp();
		Poid = HX_Run(tare_user);

		//Envoie Node Red
		LORAWAN_Send(Poid, Temperature);
		//Stanby mode
	}
}
