#include "gpio.h"

void GPIO_Init(void){
	RCC->IOPENR   |= RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN|RCC_IOPENR_GPIOAEN;

	GPIOA->MODER  &= ~(GPIO_MODER_MODE0_Msk);
	GPIOA->MODER  |= OUTPUT_MODE << GPIO_MODER_MODE0_Pos;	//PA0 Led D1
	GPIOB->MODER  &= ~(GPIO_MODER_MODE5_Msk);
	GPIOB->MODER  |= OUTPUT_MODE << GPIO_MODER_MODE5_Pos;
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
void GPIO_write(uint8_t num_pin, uint8_t valeur, GPIO_TypeDef * nom_port)
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
uint8_t GPIO_read(uint8_t num_pin, GPIO_TypeDef * nom_port)
{
	uint32_t value = nom_port->IDR;

	value = value >> num_pin;
	value &= 1;

	return (uint8_t)(value);
}
