/*------------------------------------------------------------
Programme realiser en L3 ESET 2025

Auteur du fichier : Depierre Alan

Auteur du programme : Perret Antoine

Aider par : Secret Gillian



Programme d'entête pour l'envoie des données



-------------------------------------------------------------*/
#ifndef LORAWAN_H_
#define LORAWAN_H_

/*-----Library-----*/
#include "timer.h"
#include "main.h"
#include "gpio.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stm32g031xx.h>
/*-----Constante-----*/

//-----Type stucturé-----//
typedef enum {
	NOERROR,
	NOANSWER,
	ANSWERNOTFOUND
}LORA_error;

/*-----Prototype-----*/
/**
* \brief Initialise le port UART1 pour communiquer avec le module LoRawan.
* \details Manipule les registres APBENR2, CR1, CR2, et BRR pour configurer le port UART1,
* et les registres IOPENR, MODER, OTYPER, et AFR afin de configurer les GPIO utilisé par celui-ci (PB6 tx, PB7 rx).
*/
void LORA_initPortCom(void);

/**
* \brief Démarre le port UART1.
* \details Met à 1 le bit UE (UART Enable) du registre CR1
*/
void LORA_startPortCom(void);

/**
* \brief Arrête le port UART1.
* \details Met à 0 le bit UE (UART Enable) du registre CR1
*/
void LORA_stopPortCom(void);

/**
* \brief Join le module a la gateway
*/
void LORAWAN_Join(void);

/**
* \brief Envoie la valeur du poid et de la température.
*/
void LORAWAN_Send(uint32_t Poid, uint16_t Temperature);

/**
* \brief Envoie un simple message sur l'UART.
*/
void LORAWAN_Inform(char* string);

/**
* \brief Transmet une chaine de caractère via le port UART1 au module LoRawan.
* \details Place une chaine de caractère dans la FIFO du port UART1. S'arrête lorsque tout les caractères seront placé.
* \param[in] const char* msg la chaîne de caractère a transmètre. C'est un tableau.
* \param[in] uint32_t size la taille de cette chaine de caractère, aka la taille du tableau msg.
*/
void UART_send(const char*, uint32_t);
LORA_error LORA_wasReceived(const char*, char*, uint32_t);

#endif
