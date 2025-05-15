/*
 * FLASH.h
 *
 *  Created on: Apr 10, 2025
 *      Author: perrettea
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>
#include <stm32g031xx.h>
#define ROMADDRTARE 0x800F800
#define ROMADDRNBOFMEASURE 0x0800F850

uint64_t read_flash_memory(uint32_t memory_address);
void flash_write(uint32_t address, uint64_t data);
void flash_erase_page(uint32_t page_addr);

#endif /* FLASH_H_ */
