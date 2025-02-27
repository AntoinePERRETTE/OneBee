///*------------------------------------------------------------
//Programme realiser en L3 ESET 2025
//
//Auteur du fichier : Depierre Alan
//
//Auteur du programme : Depierre Alan
//
//
//
//Fonctions basse consomation
//
//
//
//
//-------------------------------------------------------------*/
//
//#include "SleepMode.h"
//
///* Fonction principale */
//int sleep(void) {
//	/* Désactiver les interruptions globales pendant la configuration */
//	__disable_irq();
//
//	/* Configuration de l'horloge système */
//	SystemClock_Config();
//
//	/* Configuration du RTC pour générer une alarme */
//	RTC_Config();
//
//	/* Entrer en mode Standby */
//	Enter_Standby_Mode();
//
//	/* Code exécuté après un réveil */
//	while (1) {
//		// Exemple : Toggling d'une LED
//		for (volatile int i = 0; i < 1000000; i++); // Délai rudimentaire
//	}
//}
//
///* Configuration de l'horloge système */
//void SystemClock_Config(void) {
//	/* Activer l'horloge HSI (High-Speed Internal) */
//	RCC->CR |= RCC_CR_HSION;                      // Activer HSI
//	while (!(RCC->CR & RCC_CR_HSIRDY));           // Attendre que HSI soit prêt
//
//	/* Activer l'horloge LSE (Low-Speed External) pour le RTC */
//	RCC->APBENR1 |= RCC_APBENR1_PWREN;            // Activer l'horloge PWR
//	PWR->CR1 |= PWR_CR1_DBP;                      // Déverrouiller l'accès au domaine de sauvegarde
//	RCC->BDCR |= RCC_BDCR_LSEON;                  // Activer l'oscillateur LSE
//	while (!(RCC->BDCR & RCC_BDCR_LSERDY));       // Attendre que LSE soit stable
//	RCC->BDCR |= RCC_BDCR_RTCSEL_0;               // Sélectionner LSE comme source d'horloge RTC
//	RCC->BDCR |= RCC_BDCR_RTCEN;                  // Activer l'horloge RTC
//}
//
///* Configuration du RTC */
//void RTC_Config(void) {
//	/* Déverrouiller l'accès au RTC */
//	PWR->CR1 |= PWR_CR1_DBP;
//
//	/* Placer le RTC en mode initialisation */
//	RTC->WPR = 0xCA;                              // Désactiver la protection d'écriture
//	RTC->WPR = 0x53;
//	RTC->ISR |= RTC_ISR_INIT;                     // Activer le mode initialisation
//	while (!(RTC->ISR & RTC_ISR_INITF));          // Attendre que le mode init soit prêt
//
//	/* Configurer le prédiviseur pour une horloge RTC de 1 Hz */
//	RTC->PRER = (127 << 16) | 255;                // Prédiviseur pour LSE (32.768 kHz)
//
//	/* Configurer l'alarme A pour se déclencher toutes les 30 minutes */
//	RTC->ALRMAR = (30 << 8);                      // Minutes = 30 (format BCD)
//	RTC->CR |= RTC_CR_ALRAIE;                     // Activer l'interruption de l'alarme A
//	RTC->CR |= RTC_CR_ALRAE;                      // Activer l'alarme A
//
//	/* Quitter le mode initialisation */
//	RTC->ISR &= ~RTC_ISR_INIT;                    // Désactiver le mode initialisation
//	RTC->WPR = 0xFF;                              // Réactiver la protection d'écriture
//
//	/* Configurer l'interruption NVIC pour l'alarme RTC */
//	NVIC_EnableIRQ(RTC_TAMP_IRQn);                // Activer l'interruption RTC
//	NVIC_SetPriority(RTC_TAMP_IRQn, 0);
//}
//
///* Gestionnaire d'interruption pour l'alarme RTC */
//void RTC_TAMP_IRQHandler(void) {
//	if (RTC->ISR & RTC_ISR_ALRAF) {
//		RTC->ISR &= ~RTC_ISR_ALRAF;               // Effacer le drapeau d'alarme
//	}
//}
//
///* Entrer en mode Standby */
//void Enter_Standby_Mode(void) {
//	/* Configurer le Wakeup Pin (optionnel, peut être utilisé avec d'autres sources de réveil) */
//	PWR->CR3 |= PWR_CR3_EWUP1;
//
//	/* Effacer le drapeau WUF (Wakeup Flag) */
//	PWR->SCR |= PWR_SCR_CWUF;
//
//	/* Configurer le mode Standby */
//	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;           // Activer le mode profond (deep sleep)
//	PWR->CR1 |= PWR_CR1_LPMS_STANDBY;            // Activer le mode Standby
//
//	/* Entrer en mode Standby */
//	__WFI();                                     // Wait for Interrupt
//}
//
///* Gestion des erreurs */
//void Error_Handler(void) {
//	while (1) {
//		// Boucle infinie pour indiquer une erreur
//	}
//}
