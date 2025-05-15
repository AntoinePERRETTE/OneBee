/*
 * FLASH.c
 *
 *  Created on: Apr 10, 2025
 *      Author: perrettea
 */

#include "FLASH.h"


// Fonction pour lire dans la flash
uint64_t read_flash_memory(uint32_t memory_address)
{
	return *(__IO uint64_t*)memory_address;
}

// Fonction pour écrire 32 bits dans la flash
void flash_write(uint32_t address, uint64_t data)
{
	// 1. Vérifier que l'adresse est alignée sur 32 bits
	if (address % 4 != 0) return;

	// 2. Vérifier que l'adresse est dans la plage valide de la flash
	if (address < FLASH_BASE || address >= (FLASH_BASE + 64*1024)) return;

	// 3. Attendre que les opérations flash précédentes soient terminées
	while (FLASH->SR & FLASH_SR_BSY1);

	// 4. Déverrouiller la flash
	if (FLASH->CR & FLASH_CR_LOCK) {
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}

	// 5. Effacer les bits d'erreur potentiels
	FLASH->SR |= (FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR |
			FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR |
			FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_RDERR);

	// 6. Configurer pour programmation 32 bits
	FLASH->CR &= ~FLASH_CR_PG_Msk;
	FLASH->CR |= FLASH_CR_PG;

	// 7. Écrire les données
	*(__IO uint64_t*)address = data;

	// 8. Attendre la fin de l'écriture
	while (FLASH->SR & FLASH_SR_BSY1);

	// 9. Vérifier si erreur
	// TODO utiliser cette erreur !!
	uint32_t error = FLASH->SR & (FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR |
			FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR |
			FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_RDERR);

	// 10. Désactiver le mode programmation
	FLASH->CR &= ~FLASH_CR_PG;

	FLASH->SR &= ~FLASH_SR_EOP;

	// 11. Reverrouiller la flash
	FLASH->CR |= FLASH_CR_LOCK;
}

// Fonction pour effacer une page (2KB sur STM32G031)
void flash_erase_page(uint32_t page_addr)
{
	// Vérifier alignement sur début de page
	if (page_addr % 2048 != 0) return;

	// Attendre la fin des opérations flash
	while (FLASH->SR & FLASH_SR_BSY1);

	// Déverrouiller la flash
	if (FLASH->CR & FLASH_CR_LOCK) {
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
	}

	// Effacer les bits d'erreur
	FLASH->SR |= (FLASH_SR_OPERR | FLASH_SR_PROGERR | FLASH_SR_WRPERR |
			FLASH_SR_PGAERR | FLASH_SR_SIZERR | FLASH_SR_PGSERR |
			FLASH_SR_MISERR | FLASH_SR_FASTERR | FLASH_SR_RDERR);

	// Configurer pour effacement de page
	FLASH->CR |= FLASH_CR_PER;
	FLASH->CR &= ~FLASH_CR_PNB_Msk;
	FLASH->CR |= ((page_addr - FLASH_BASE) / 2048) << FLASH_CR_PNB_Pos;

	// Lancer l'effacement
	FLASH->CR |= FLASH_CR_STRT;

	// Attendre la fin de l'effacement
	while (FLASH->SR & FLASH_SR_BSY1);

	// Désactiver le mode effacement
	FLASH->CR &= ~FLASH_CR_PER;

	// Reverrouiller la flash
	FLASH->CR |= FLASH_CR_LOCK;
}
