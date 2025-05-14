/*------------------------------------------------------------
Programme realiser en L3 ESET 2025

Auteur du fichier : Depierre Alan

Auteur du programme : Poncet Gregory

Aider par : Perrette Antoine

Fonction pour la mesure du poid




-------------------------------------------------------------*/

/*-----Library-----*/
#include "HX711.h"

/*-----Fonction-----*/

/**
 * \brief Init GPIO, input/output pin
 */
void HX_init(void){
	/* Clock Init */
	RCC->IOPENR   |= RCC_IOPENR_GPIOBEN;

	GPIOB->MODER  &= ~(GPIO_MODER_MODE8_Msk);
	GPIOB->MODER  |= OUTPUT_MODE << GPIO_MODER_MODE8_Pos;	// PB8 Output PDSK pour HX711

	GPIOB->MODER  &= ~(GPIO_MODER_MODE9_Msk);
	GPIOB->MODER  |= INPUT_MODE << GPIO_MODER_MODE9_Pos;	// PB9 en Input DOUT HX711

//	GPIOB->MODER  &= ~(GPIO_MODER_MODE4_Msk);
//	GPIOB->MODER  |= INPUT_MODE << GPIO_MODER_MODE4_Pos;	// PB4 en Input pour BP de Tare
}

/**
 * \brief Main du HX711
 * \details Cette fonction gère le fonctionement du HX après l'init et appelle des fonctions d'envoie des données
 * \param[in] tare tare à 0 selon le terrain où se trouve la balance
 */
uint32_t HX_getData(uint32_t tare){
	uint32_t donnee = 0;

	donnee = HX_read(1) & 0x7FFFFF;

	return (uint32_t)((((float)donnee - (float)tare) / 11363.87)*1000);
	delay_ms(500);
}

/**
 * \brief Récupère les données transmisent par le HX711
 * \details Cette fonction reçoit 24 bits de donnée et doit envoyer 25 coups d'horloge au HX
 * \param[in] Low_Power si est à 1, met le hx en mode LP
 */
uint32_t HX_read(uint8_t Low_Power){
	uint32_t Count = 0;
	uint8_t i;
	//uint8_t return_value;

	GPIO_write(8,0,GPIOB);
	while(GPIO_read(9,GPIOB)==1);


	for (i=0;i<24;i++){

		GPIO_write(8,1,GPIOB);
		Count=Count<<1;
		delay_us(40);
		if(GPIO_read(9,GPIOB)== 1){
			Count++;
		}
		GPIO_write(8,0,GPIOB);

		delay_us(40);
	}


	GPIO_write(8,1,GPIOB);
	Count=Count^0x800000;

	if(Low_Power == 1){					//Après réception des données, le hx peut se mettre en LP si PDS_clk reste à 1
		GPIO_write(8,1,GPIOB);
	}
	else{
		GPIO_write(8,0,GPIOB);
	}

	return Count;
}

//-----Interuption-----//

/**
 * \brief Mesure du poid zero de la balance
 * \details Cette fonction mesure une série de masse, puis écrit la moyenne de ces valeur dans tare et la page rom associée.
 */
void HX_getTare(void) {
	uint32_t tare = 0;
	uint32_t calibration[5] = {0};

	for (uint8_t i = 0; i < 5; i++) {
		calibration[i] = HX_read(1) & 0x7FFFFF;
	}

	//moyenne
	for (uint8_t i = 0; i < 5; i++) {
		tare += calibration[i];
	}
	tare /= 5;
	flash_erase_page(ROMADDRTARE);
	flash_write(ROMADDRTARE, (uint64_t) tare);
	EXTI->RPR1 |= 1 << EXTI_RPR1_RPIF4_Pos;	//Ecrire 1 pour reset ATTENTION !!
}
