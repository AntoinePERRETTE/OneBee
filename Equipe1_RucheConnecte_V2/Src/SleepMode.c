#include "SleepMode.h"

void RTC_TAMP_IRQHandler(void) {
	// Déverrouiller les registres RTC (important!)
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// Effacer le flag d'interruption
	RTC->SCR |= RTC_SCR_CWUTF;

	// Verrouiller à nouveau les registres
	RTC->WPR = 0xFF;
}

void RTC_Init() {
	RCC->APBENR1 |= RCC_APBENR1_PWREN;
	RCC->APBENR1 |= RCC_APBENR1_RTCAPBEN;

	PWR->CR1 |= 1 << PWR_CR1_DBP_Pos;
	RCC->BDCR |= 1 << RCC_BDCR_RTCEN_Pos;
	RCC->BDCR |= (2 << RCC_BDCR_RTCSEL_Pos);  // Sélectionner LSI comme source RTC
	RCC->CSR |= RCC_CSR_LSION;	//start LSI

	while(!(RCC->CSR & RCC_CSR_LSIRDY)); // Wait for LSI ready
	// Réinitialiser RTC après réveil
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	RTC->CR &= ~RTC_CR_WUTE;  // Désactiver Wakeup Timer
	RTC->CR &= ~RTC_CR_WUTIE;  // Désactiver temporairement l'interruption

	RTC->WPR = 0xFF;
}

void enter_low_power_standby_mode(uint32_t sleepTime) {
	// Effacer les flags de réveil
	PWR->SCR |= PWR_SCR_CWUF;

	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// Configurer Standby mode
	PWR->CR1 |= (0b011 << PWR_CR1_LPMS_Pos);

	// Configurer RTC comme source de réveil
	//	PWR->CR3 |= PWR_CR3_EIWUL;  // Enable internal wakeup line
	//	PWR->CR3 |= PWR_CR3_EWUP1;
	//	PWR->CR4 |= PWR_CR4_WP1;    // Enable wakeup pin 1 (vérifiez la documentation de votre puce)

	//RTC->CR |= RTC_CR_WUTE;
	RTC->CR &= ~RTC_CR_WUTE;  // Désactiver Wakeup Timer

	while (!(RTC->ICSR & RTC_ICSR_WUTWF));  // Attendre que WUTWF soit prêt

	//RTC->WUTR = sleepTime;
	RTC->WUTR = 300; // Réveil dans 10 secondes

	// Select appropriate clock source: 100 binary (not decimal) for 1Hz
	RTC->SCR |= RTC_SCR_CWUTF;
	RTC->CR &= ~(7 << RTC_CR_WUCKSEL_Pos);  // Clear bits first
	RTC->CR |= (4 << RTC_CR_WUCKSEL_Pos);   // Set to binary 100 (decimal 4) //TODO change for 18h->36h binary 110 ??

	RTC->CR |= RTC_CR_WUTE | RTC_CR_WUTIE;  // Activer le Wakeup Timer et son interruption

	RTC->WPR = 0xFF;

	// 2. Entrer en mode veille
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk; // Configurer le mode veille profond
	NVIC_EnableIRQ(RTC_TAMP_IRQn);  // Activer l’IRQ RTC Wakeup

	delay_ms(10);

	__WFI();  // Entrer en veille (Wait For Interrupt)
}
