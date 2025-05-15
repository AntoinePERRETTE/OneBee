#ifndef GPIO_H_
#define GPIO_H_

//-----Library-----//
#include "main.h"
#include <stdint.h>
#include <stm32g031xx.h>

//-----Prototype-----//
void GPIO_Init(void);

/*R/W des Input/Output*/
void GPIO_write(uint8_t num_pin, uint8_t valeur, GPIO_TypeDef * nom_port);
uint8_t GPIO_read(uint8_t num_pin, GPIO_TypeDef * nom_port);

#endif
