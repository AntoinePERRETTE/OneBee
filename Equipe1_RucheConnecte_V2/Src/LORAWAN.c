/*------------------------------------------------------------
Programme realisé en L3 ESET 2025

Auteur du fichier : Depierre Alan

Auteur du programme : Perrette Antoine

Aidé par : Secret Gillian



Fonctions pour l'envoie des données



-------------------------------------------------------------*/
/*-----Library-----*/
#include <LORAWAN.h>

//-----Variable Global-----//
uint8_t rcvBuffer[512] = {0};
uint16_t iterateurRcvBuffer = 0;

//-----Prototype-----//
void LORA_initPortCom(void) {
	/* GPIO Init for UART on NUCLEO Board */
	RCC->IOPENR |=  RCC_IOPENR_GPIOBEN;

	GPIOB->MODER  &= ~(GPIO_MODER_MODE6_Msk);
	GPIOB->MODER  |= ALTERN_MODE << GPIO_MODER_MODE6_Pos;	// PB6 mode alternate function
	GPIOB->MODER  &= ~(GPIO_MODER_MODE7_Msk);
	GPIOB->MODER  |= ALTERN_MODE << GPIO_MODER_MODE7_Pos;	// PB7 mode alternate function

	//open-drain
	GPIOB->OTYPER |= 1 << GPIO_OTYPER_OT6_Pos;

	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk);				//Reset
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL7_Msk);				//Reset

	//Init UART
	RCC->APBENR2 |= RCC_APBENR2_USART1EN;

	//TODO uncomment if UART rx used
//	USART1->CR1 |= 1 << USART_CR1_RXNEIE_RXFNEIE_Pos;		//Enable interruption on reception
//	NVIC_EnableIRQ(USART1_IRQn);

	USART1->CR1 |= (1<<USART_CR1_TE_Pos) | (1 << USART_CR1_RE_Pos);
	USART1->BRR = SystemCoreClock / 9600;
}

void LORA_startPortCom(void) {
	USART1->CR1 |= (1<<USART_CR1_UE_Pos);
}

void LORA_stopPortCom(void) {
	USART1->CR1 &= ~(1<<USART_CR1_UE_Pos);
}

void LORAWAN_Join(void){
	SYSTICK_Delay(1000);
	LORA_initPortCom();
	LORA_startPortCom();

	char bufferToSend[255] = "AT+JOIN\r\n";

	UART_send(bufferToSend, 10);

	SYSTICK_Delay(15000);
}

void LORAWAN_Send(uint32_t Poid, uint16_t Temperature)
{
    uint8_t payload[6]; // 2 octets pour chaque valeur

    // Copier les valeurs uint16_t dans le tableau payload
    memcpy(&payload[0], &Poid, sizeof(uint32_t));         // Poid dans octets 0-1
    memcpy(&payload[4], &Temperature, sizeof(uint16_t));  // Temperature dans octets 2-3

    // Préparer la chaîne HEX pour l'envoi
    char bufferToSend[255];
    sprintf(bufferToSend, "AT+CMSGHEX=\"%02X%02X%02X%02X%02X%02X\"\r\n",
            payload[0], payload[1], payload[2], payload[3], payload[4],payload[5]);

    // Envoyer la commande via UART
    UART_send(bufferToSend, strlen(bufferToSend));
    SYSTICK_Delay(15000);
}

void LORAWAN_Inform(char* string)
{
	char bufferToSend[255];

	sprintf(bufferToSend, "AT+CMSG=\"%s\"\r\n", string);
	UART_send(bufferToSend, strlen(bufferToSend));
	SYSTICK_Delay(15000);
}

void UART_send(const char* msg, uint32_t size) {
	uint32_t i = 0;
	while(i < size) {
		if (USART1->ISR & USART_ISR_TXE_TXFNF_Msk) {
			USART1->TDR = msg[i];
			i++;
		}
	}
	while(!(USART1->ISR & USART_ISR_TC_Msk));	//wait transmition complete
}

//void USART1_IRQHandler(void) {
//	rcvBuffer[iterateurRcvBuffer] = USART1->RDR;
//	if (iterateurRcvBuffer > 511) iterateurRcvBuffer = 0;
//	else iterateurRcvBuffer++;
//	//interrupt reset by reading RDR
//	USART1->ICR |= 1 << USART_ICR_ORECF_Pos;
//}
