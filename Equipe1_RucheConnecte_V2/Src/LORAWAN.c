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
char rcvBuffer[512] = {0};
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

	//PB6 et PB7 liée a rx et tx
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6_Msk);	//Reset
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL7_Msk);	//Reset

	//Init UART
	RCC->APBENR2 |= RCC_APBENR2_USART1EN;

	//TODO uncomment if UART rx used
	USART1->CR1 |= 1 << USART_CR1_RXNEIE_RXFNEIE_Pos;		//Enable interruption on reception
	NVIC_EnableIRQ(USART1_IRQn);

	USART1->CR1 |= (1<<USART_CR1_TE_Pos) | (1 << USART_CR1_RE_Pos);
	USART1->BRR = SystemCoreClock / 9600;
}

void LORA_startPortCom(void) {
	USART1->CR1 |= (1<<USART_CR1_UE_Pos);
}

void LORA_stopPortCom(void) {
	USART1->CR1 &= ~(1<<USART_CR1_UE_Pos);
	char nullBuffer[512] = {0};
	memcpy(rcvBuffer, nullBuffer, iterateurRcvBuffer+2);
	iterateurRcvBuffer = 0;
}

void LORAWAN_Join(void){
	SYSTICK_Delay(3000);
	LORA_initPortCom();
	LORA_startPortCom();

	char bufferToSend[255] = "AT+JOIN\r\n";

	UART_send(bufferToSend, 10);
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

void USART1_IRQHandler(void) {
	rcvBuffer[iterateurRcvBuffer] = USART1->RDR;
	if (iterateurRcvBuffer > 511) iterateurRcvBuffer = 0;
	else iterateurRcvBuffer++;
	//interrupt reset by reading RDR
	USART1->ICR |= 1 << USART_ICR_ORECF_Pos;
}

/**
 * @brief WasReceived vérifi si on ressoi la bonne réponce
 *
 * @param x_str_StrCompar représante la chaine de caractère à tester
 *
 * @return une valeur de type T_ERROR pour donner le résultat du test
 */
T_RECEIVED_ERROR WasReceived(char* x_str_StrCompar) {
	//pointeur utiliser pour la vérification avec strstr
	char* l_PtrChar_return = NULL;
	//buffeur stoquant le morceau de réponce à tester
	T_BUFFER l_tBuffer_pille;

	BufferInit(&l_tBuffer_pille);

	//met en route un timoute sur le timeurs 3
	Tim3Flag_ms(15000);
	do {
		if(iterateurRcvBuffer != 0)
		{
			//stock un morceau de message ressu dans le buffer
			do {
				ReadBuffer(&l_tBuffer_pille);
			} while ((l_tBuffer_pille.m_bool_MsgOk != 1) && (!get_CptFlag3()));

			//compart le morceau de réponce avec la réponce attandu
			l_PtrChar_return = strstr(l_tBuffer_pille.m_str_pille, x_str_StrCompar);
		}
	} while ((l_PtrChar_return == NULL)&&(!get_CptFlag3()));

	//vérifi si une valeur à été ressu
	if(iterateurRcvBuffer == 0)
		return ERROR;

	//réinitialise l'itérateur du buffer de la liaison UART
	iterateurRcvBuffer = 0;

	//vérifi si le test à été validé
	if(l_PtrChar_return == NULL)
		return ERROR;

	return NO_ERROR;
}

/**
 * @brief WasReceivedForSend vérifi si on ressoi la bonne réponce
 *
 * @param x_str_StrCompar représante la chaine de caractère à tester
 * @param x_PtrUint16_ParamVal pinteur vers la valeur de retour du downlink
 *
 * @return une valeur de type T_ERROR pour donner le résultat du test
 */
T_RECEIVED_ERROR WasReceivedForSend(char* x_str_StrCompar, uint16_t* x_PtrUint16_ParamVal)
{
	//pointeur utiliser pour la vérification avec strstr
	char* l_PtrChar_return = NULL;
	//buffeur stoquant le morceau de réponce à tester
	T_BUFFER l_tBuffer_pille;
	//chaine de caractère servant à vérifier si nous avons ressu une donné pour parametrer le module
	char l_str_AkParam[] = "+CMSGHEX: PORT: 15";
	//variabl de travaille
	uint16_t l_uint16_buffer = 0;
	//valeur du paramètre retourner
	*x_PtrUint16_ParamVal = 0;

	BufferInit(&l_tBuffer_pille);

	//met en route un timoute sur le timeur 3
	Tim3Flag_ms(10000);
	do {
		if(iterateurRcvBuffer != 0)
		{
			//stock un morceau de message ressu dans le buffer
			do {
				ReadBuffer(&l_tBuffer_pille);
			} while ((l_tBuffer_pille.m_bool_MsgOk != 1) && (!get_CptFlag3()));

			//compart le morceau de réponce avec la réponce attandu
			l_PtrChar_return = strstr(l_tBuffer_pille.m_str_pille, x_str_StrCompar);
		}
	} while ((l_PtrChar_return == NULL)&&(!get_CptFlag3()));

	//vérifi si une valeur à été ressu
	if(iterateurRcvBuffer == 0) return ERROR;

	//vérifi si le test à été validé
	if(l_PtrChar_return == NULL) return ERROR;

	//vérifi si un paramètre à été ajouter à l'acknowledge du message
	l_PtrChar_return = strstr(rcvBuffer, l_str_AkParam);
	//vérifi si le test à été validé
	if(l_PtrChar_return != NULL)
	{
		//cherche le début de la valeur du downlink
		while(l_PtrChar_return[0] != '\0' && l_PtrChar_return[0] != '\"')
		{
			l_PtrChar_return++;
		}

		//incrémante de 1 pour pointer sur la première valeur hexa
		l_PtrChar_return++;

		//converti la valeur hexadecimal en decimal
		for(uint8_t i = 0 ; i < 4 ; i++)
		{
			if(l_PtrChar_return[i] >='0' && l_PtrChar_return[i] <= '9')
			{
				l_uint16_buffer			= l_PtrChar_return[i] - '0';
				l_uint16_buffer			= l_uint16_buffer << (4 * (3-i));
				*x_PtrUint16_ParamVal	+= l_uint16_buffer;
			}else if (l_PtrChar_return[i] >='A' && l_PtrChar_return[i] <= 'F')
			{
				l_uint16_buffer			= 10 + l_PtrChar_return[i] - 'A';
				l_uint16_buffer			= l_uint16_buffer << (4 * (3-i));
				*x_PtrUint16_ParamVal	+= l_uint16_buffer;
			}
		}
	}

	//réinitialise l'itérateur du buffer de la liaison UART
	iterateurRcvBuffer = 0;

	return NO_ERROR;
}

/**
 * @brief initialise la structure
 *
 * @param x_tBuffer pointe vers la structure à initialiser
 */
void BufferInit(T_BUFFER* x_tBuffer)
{
	for(uint8_t i = 0 ; i < PILLE_SIZE ; i++)
	{
		x_tBuffer->m_str_pille[i] = 0;
	}

	x_tBuffer->m_uint7_PilleIt = 0;

	x_tBuffer->m_uint16_BufferIt = 0;

	x_tBuffer->m_bool_MsgOk = 0;
}

/**
 * @brief  ajoute une valeur à la pille
 *
 * @param x_tBuffer pointe vers la structure repésantant la pille
 * @param x_uint8_data valeur à ajouter
 * @return retourne 1 si l'ajoue à pue ce faire 0 si non
 */
uint8_t BufferEmpille(T_BUFFER* x_tBuffer, uint8_t x_uint8_data)
{
    if((x_tBuffer == NULL) || (x_tBuffer->m_uint7_PilleIt >= PILLE_SIZE - 1))
    {
        return -1;
    }

    x_tBuffer->m_str_pille[x_tBuffer->m_uint7_PilleIt] = x_uint8_data;
    x_tBuffer->m_uint7_PilleIt++;

    return 0 ;
}

/**
 * @brief retourne la dernière valeur ajouter et l'décrémante l'itérateur
 *
 * @param x_tBuffer pointe vers la structure repésantant la pille
 * @return retourne la dernière valeur ajouter 255 si non
 */
uint8_t BufferDepille(T_BUFFER* x_tBuffer)
{
    if((x_tBuffer->m_uint7_PilleIt < 1) || (x_tBuffer == NULL))
    {
        return 255;
    }

    x_tBuffer->m_uint7_PilleIt--;
    return x_tBuffer->m_str_pille[x_tBuffer->m_uint7_PilleIt];
}

/**
 * @brief lis la une trame contenu dans le buffer
 *
 * @param x_tBuffer structure gérant la copie du buffer
 */
void ReadBuffer(T_BUFFER* x_tBuffer)
{
    uint8_t i = 0;

    //recharche d'un message valide
    while ((rcvBuffer[x_tBuffer->m_uint16_BufferIt + i] != '\r') && i < PILLE_SIZE && (x_tBuffer->m_uint16_BufferIt + i) < BUFFER_SIZE/*à modiffier avec la taille du buffer*/)
    {
        BufferEmpille(x_tBuffer, rcvBuffer[x_tBuffer->m_uint16_BufferIt + i]);
        i++;
    }

    //verifi si on a trouver un message valide. si ce n'est pas le cas on met m_bool_MsgOk à 0
    if(rcvBuffer[x_tBuffer->m_uint16_BufferIt + i] != '\r')
    {
        x_tBuffer->m_bool_MsgOk			= 0;
        x_tBuffer->m_uint7_PilleIt		= 0;
        x_tBuffer->m_uint16_BufferIt	= 0;
    }else{
    	//si le message est valide alor on note où on en est dans la lecture du buffer et on met m_bool_MsgOk à 1
		x_tBuffer->m_uint16_BufferIt    += (i + 1);
		x_tBuffer->m_bool_MsgOk         = 1;
		x_tBuffer->m_uint7_PilleIt      = 0;

		if((x_tBuffer->m_uint16_BufferIt + i) >= BUFFER_SIZE)
		{
			x_tBuffer->m_uint16_BufferIt = 0;
		}
    }
}

void LORAWAN_Inform(char* string)
{
	char bufferToSend[255];

	sprintf(bufferToSend, "AT+CMSG=\"%s\"\r\n", string);
	UART_send(bufferToSend, strlen(bufferToSend));
	SYSTICK_Delay(15000);
}
