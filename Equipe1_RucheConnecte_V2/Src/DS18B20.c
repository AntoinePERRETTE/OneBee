/*------------------------------------------------------------
Programme realiser en L3 ESET 2025

Auteur du fichier : Depierre Alan

Auteur du programme : Depierre Alan



Fonction pour la sonde de température ds18b20




-------------------------------------------------------------*/
/*-----library-----*/
#include <DS18B20.h>

/*-----Fonction-----*/

void DS18B20_Init(void) {
	// Init GPIO PA9 (1-Wire) en mode normal sans pull up/down
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
	GPIOA->MODER &= ~GPIO_MODER_MODE9;
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;
}

uint16_t DS18B20_GetTemp(void){

	//1er transaction configuration du registre de config
	DS18B20_reset();
	DS18B20_write(0xCC);

	DS18B20_write(0x4E);

	DS18B20_write(0x00);
	DS18B20_write(0x00);
	DS18B20_write(0x1F);
	//2ème transation : mesure de la température

	DS18B20_reset();

	DS18B20_write(0xCC);
	DS18B20_write(0x44);

	GPIOA->ODR |= (1 << 9);
	delay_ms(110);

	//3ème transaction : lecture de mémoire

	DS18B20_reset();

	DS18B20_write(0xCC);
	DS18B20_write(0xBE);

	uint16_t raw_value = DS18B20_ReadConversion();
	float Temperature = DS18B20_Convertion(raw_value);

	if (!(DS18B20_Verification(Temperature))){
		return(Temperature);
	}
	else{
		DS18B20_GetTemp();
	}
}

//Tire la ligne vers le bas pendant 480us puis vérifie que le composant fait acte de présence

	int DS18B20_reset(void){
		GPIOA->MODER |= GPIO_MODER_MODE9_0;
		GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
		GPIOA->ODR &= ~(1 << 9);
		delay_us(480);

		GPIOA->OTYPER |= GPIO_OTYPER_OT9;
		GPIOA->MODER &= ~GPIO_MODER_MODE9_0;
		delay_us(80);

		if(!(GPIOA->IDR & (1 << 9))){
			delay_us(400);
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
			return 0;
		}

		else{
			delay_us(400);
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
			return 1;
		}
	}

// Ecrit un octet sur la ligne de donnée, si c'est un 1, la ligne est haute pendant 45us, sinon, elle est basse pendant 45 us

	void DS18B20_write(uint8_t data) {

		GPIOA->MODER |= GPIO_MODER_MODE9_0;

		for (int i = 0; i < 8; i++) {
			// Si le bit est "1"
			if (data & (1 << i)) {
				GPIOA->ODR &= ~(1 << 9);
				delay_us(1);

				GPIOA->ODR |= (1 << 9);
				delay_us(45);
			}
			// Si le bit est "0"
			else {
				GPIOA->ODR &= ~(1 << 9);
				delay_us(45);

				GPIOA->ODR |= (1 << 9);
				delay_us(1);
			}
		}
	}

//Lit deux octets pour avoir les 16 bits qui capte la température

	uint16_t DS18B20_ReadConversion(void){
		uint8_t low_byte, high_byte;

		// Lire les deux octets du scratchpad
		low_byte = DS18B20_read();
		high_byte = DS18B20_read();

		uint16_t raw_value = ((high_byte << 8) | low_byte)*10;

		return raw_value;
	}

//Lit les bits sur la ligne de donnée toutes les 60us et définis les états au bout de 12us

		uint8_t DS18B20_read(void){
			uint8_t data =0;

			GPIOA->OTYPER |= GPIO_OTYPER_OT9;
			GPIOA->MODER &= ~GPIO_MODER_MODE9_0;

			for (int i = 0; i <8; i++) {
					// Tirer la ligne basse pour initier la lecture
					GPIOA->MODER |= GPIO_MODER_MODE9_0;
					GPIOA->ODR &= ~(1 << 9);
					delay_us(2);

					// Relâcher la ligne et configurer GPIO en entrée pour lire le bit
					GPIOA->MODER &= ~GPIO_MODER_MODE9_0;
					delay_us(12);

					// Lire le bit et l'ajouter au résultat
					if (GPIOA->IDR & (1 << 9)) {
						data |= (1 << i);
					}

					// Attendre la fin de la fenêtre de 60 µs
					delay_us(50);
			}
			GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
			return data;
		}

// Fonction pour convertir la valeur brute DS18B20 en valeur flotante

		uint16_t DS18B20_Convertion(uint16_t raw_value) {

		float temp = raw_value * 0.0625;  // Multiplier par 0.0625 pour obtenir la température en °C

		return temp;
	}

//Vérifie qu'on ne recois pas un message d'erreur de la sonde

	uint8_t DS18B20_Verification (uint16_t temp){
		if (temp == 850 || temp <= -550 || temp >= 1250){
			return 1;
		}
		else{
			return 0;
		}
	}
