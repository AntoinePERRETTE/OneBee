/*------------------------------------------------------------
Programme realiser en L3 ESET 2025

Auteur du fichier : Depierre Alan

Auteur du programme : Depierre Alan



Programme d'entête pour la sonde de température




-------------------------------------------------------------*/

#ifndef DS18B20_H_
#define DS18B20_H_

/*-----Library-----*/
#include "main.h"
#include "timer.h"
#include "gpio.h"
#include <stdint.h>
#include <stm32g031xx.h>
#include <stdbool.h>
#include <stdio.h>

/*-----Constante-----*/

/*-----Prototype-----*/
void DS18B20_Init(void);

uint16_t DS18B20_GetTemp(void);

int DS18B20_reset(void);

void DS18B20_write(uint8_t data);

uint16_t DS18B20_ReadConversion(void);

uint8_t DS18B20_read(void);

uint16_t DS18B20_Convertion(uint16_t raw_value);

uint8_t DS18B20_Verification (uint16_t temp);

#endif
