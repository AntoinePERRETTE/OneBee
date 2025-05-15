/*------------------------------------------------------------
Programme realiser en L3 ESET 2025

Auteur du fichier : Depierre Alianne

Auteur du programme : Depierre Alianne

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
void DS18B20_init(void);

uint16_t DS18B20_getTemp(void);

int DS18B20_reset(void);

void DS18B20_write(uint8_t data);

uint16_t DS18B20_convertMeasure(void);

uint8_t DS18B20_readMeasure(void);

float DS18B20_convertToCelsius(uint16_t raw_value);

uint8_t DS18B20_checkError(uint16_t temp);

#endif
