/*------------------------------------------------------------
Programme realiser en L3 ESET 2025

Auteur du fichier : Depierre Alan

Auteur du programme : Poncet Gregory

Aider par : Perrette Antoine

Fonction pour la mesure du poid




-------------------------------------------------------------*/

/*-----Library-----*/
#include "HX711.h"

//-----Variable Globale-----//
uint32_t tare_user = 0;
uint32_t calibration[5] = {0};

/*-----Fonction-----*/

/**
 * \brief Appelle toutes les fonctions d'init des systèmes
 */
void HX711_Init(void){
	HX_Init();
	NVIC_Init();
}

/**
 * \brief Init GPIO, input/output pin
 */
void HX_Init(void){
	/* Clock Init */
	RCC->IOPENR   |= RCC_IOPENR_GPIOBEN;

	GPIOB->MODER  &= ~(GPIO_MODER_MODE8_Msk);
	GPIOB->MODER  |= OUTPUT_MODE << GPIO_MODER_MODE8_Pos;	// PB8 Output PDSK pour HX711

	GPIOB->MODER  &= ~(GPIO_MODER_MODE9_Msk);
	GPIOB->MODER  |= INPUT_MODE << GPIO_MODER_MODE9_Pos;	// PB9 en Input DOUT HX711

	GPIOB->MODER  &= ~(GPIO_MODER_MODE4_Msk);
	GPIOB->MODER  |= INPUT_MODE << GPIO_MODER_MODE4_Pos;	// PB4 en Input pour BP de Tare
}

/**
 * \brief Init NVIC, PB4 en interrupt
 */
void NVIC_Init(void){
	EXTI->EXTICR[1] |= (1 << EXTI_EXTICR2_EXTI4_Pos);		//PB4 sur la ligne EXTI
	EXTI->RTSR1 |= (1 << EXTI_RTSR1_RT4_Pos);					//Rising Trigger
	EXTI->IMR1 |= (1 << EXTI_IMR1_IM4_Pos);
	NVIC_EnableIRQ(EXTI4_15_IRQn);
}

/**
 * \brief Main du HX711
 * \details Cette fonction gère le fonctionement du HX après l'init et appelle des fonctions d'envoie des données
 * \param[in] tare tare à 0 selon le terrain où se trouve la balance
 */
uint32_t HX_Run(uint32_t tare){
	uint32_t donnee = 0;
	//float masse = 0.0;

	donnee = hx_read(0) & 0x7FFFFF;

	//	masse = (roundf(masse * 100) / 100)*10;

	return (uint32_t)((((float)donnee - (float)tare) / 11363.87)*1000);
	delay_ms(500);
}

/**
 * \brief Met une valeur donnée à un pin de sortie
 * \details Manipule le registre ODR pour mettre une sortie à 0 ou 1 en effectuant
 * un décalage et une opération logique pour ne changer que la valeur du pin de sortie
 * \warning Cette fonction ne DOIT PAS être utilisée sur une sortie UART ou autre
 * \param[in] num_pin prends le numéro de la pin dont la valeur doit changer pour effectuer le décalage
 * \param[in] valeur prends la nouvelle valeur de sortie souhaitée
 * \param[in] nom_port prends le nom du port sur lequel la pin se situe pour pouvoir fonctionner sur tous les ports GPIO
 */
void ODR_write(uint8_t num_pin, uint8_t valeur, GPIO_TypeDef * nom_port)
{
	uint32_t mask = 1 << num_pin;


	if(valeur == 0)
	{
		nom_port->ODR &= ~(mask);
	}
	else
	{
		nom_port->ODR |= mask;
	}
}

/**
 * \brief Lit une valeur sur un pin d'entrée
 * \details Manipule le registre IDR pour lire une entrée 0 ou 1 en effectuant
 * un décalage et une opération logique pour ne récuperer que la valeur du pin souhaité
 * \warning Cette fonction ne DOIT PAS être utilisée sur une sortie UART ou autre
 * \param[in] num_pin prends le numéro de la pin dont la valeur doit changer pour effectuer le décalage
 * \param[in] nom_port prends le nom du port sur lequel la pin se situe pour pouvoir fonctionner sur tous les ports GPIO
 */
uint8_t IDR_read(uint8_t num_pin, GPIO_TypeDef * nom_port)
{
	uint32_t value = nom_port->IDR;

	value = value >> num_pin;
	value &= 1;

	return (uint8_t)(value);
}

/**
 * \brief Récupère les données transmisent par le HX711
 * \details Cette fonction reçoit 24 bits de donnée et doit envoyer 25 coups d'horloge au HX
 * \param[in] Low_Power si est à 1, met le hx en mode LP
 */
uint32_t hx_read(uint8_t Low_Power){
	uint32_t Count = 0;
	uint8_t i;
	//uint8_t return_value;

	ODR_write(8,0,GPIOB);
	while(IDR_read(9,GPIOB)==1);


	for (i=0;i<24;i++){

		ODR_write(8,1,GPIOB);
		Count=Count<<1;
		delay_us(40);
		if(IDR_read(9,GPIOB)== 1){
			Count++;
		}
		ODR_write(8,0,GPIOB);

		delay_us(40);
	}


	ODR_write(8,1,GPIOB);
	Count=Count^0x800000;

	if(Low_Power == 1){					//Après réception des données, le hx peut se mettre en LP si PDS_clk reste à 1
		ODR_write(8,1,GPIOB);
	}
	else{
		ODR_write(8,0,GPIOB);
	}

	return Count;
}

//-----Interuption-----//

/**
 * \brief Gestion d'interruption HX711
 * \details Cette fonction gère l'interruption de la tare_user via GPIOB PB4
 */
void EXTI4_15_IRQHandler(void) {
	tare_user = 0;

	for (uint8_t i = 0; i < 10; i++) {
		calibration[i] = hx_read(0) & 0x7FFFFF;
	}

	//moyenne
	for (uint8_t i = 0; i < 10; i++) {
		tare_user += calibration[i];
	}
	tare_user /= 10;
	EXTI->RPR1 |= 1 << EXTI_RPR1_RPIF4_Pos;	//Ecrire 1 pour reset ATTENTION !!
}
