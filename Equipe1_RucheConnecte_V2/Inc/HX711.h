/*------------------------------------------------------------
Programme realiser en L3 ESET 2025

Auteur du fichier : Depierre Alan

Auteur du programme : Poncet Gregory

Aider par : Perrette Antoine

Programme d'entête pour la mesure du poid




-------------------------------------------------------------*/

#ifndef HX711_H_
#define HX711_H_

/*-----Library-----*/
#include "main.h"
#include "gpio.h"
#include "timer.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm32g031xx.h>
/*-----Constante-----*/

/*-----Variable globale-----*/
extern uint32_t tare_user;
extern uint32_t calibration[];

/*-----Prototype-----*/
void HX711_Init(void);
void HX_Init(void);
void USART_Init(void);
void NVIC_Init(void);

/*R/W des Input/Output*/
void ODR_setup(uint8_t num_pin, uint8_t valeur, GPIO_TypeDef * nom_port);
uint8_t IDR_read(uint8_t num_pin, GPIO_TypeDef * nom_port);

/*Initialisations des fonctions nécessaires du système de pesée*/
void TIM2_Delay_us(uint32_t delay);
uint32_t calibration_loadCell(void);

/*Fonctionnement général du HX*/
uint32_t hx_read(uint8_t Low_Power);
void USART_send(const char* msg, uint32_t size);
uint32_t HX_Run(uint32_t tare);
uint32_t doTare(void);

#endif
