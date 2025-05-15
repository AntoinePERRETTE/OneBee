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
#include "FLASH.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stm32g031xx.h>


/*-----Prototype-----*/
void HX_init(void);

/*Fonctionnement général du HX*/
uint32_t HX_read(uint8_t Low_Power);
uint32_t HX_getData(uint32_t tare);
void HX_getTare(void);

#endif
