
#include "terminal.h" // terminal & card & stdio

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData){
    printf(">>Please Enter The Transaction Date(DD/MM/YYYY) : ");
    gets(termData->transactionDate);
    if((termData->transactionDate) == NULL || strlen(termData->transactionDate)!= 10 || termData->transactionDate[2] != '/' || termData->transactionDate[5] != '/' )
        return WRONG_DATE;
    return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData){
    if (cardData->cardExpirationDate[3] < termData->transactionDate[8]){
		return EXPIRED_CARD;
    }
	else if (cardData->cardExpirationDate[3] == termData->transactionDate[8]) {
		if (cardData->cardExpirationDate[4] < termData->transactionDate[9]) {
			return EXPIRED_CARD;
		}
		else if (cardData->cardExpirationDate[4] == termData->transactionDate[9]) {
			if (cardData->cardExpirationDate[0] < termData->transactionDate[3]) {
				return EXPIRED_CARD;
			}
			else if (cardData->cardExpirationDate[0] == termData->transactionDate[3]) {
				if (cardData->cardExpirationDate[1] < termData->transactionDate[4]) {
					return EXPIRED_CARD;
				}
				else {
					return TERMINAL_OK;
				}
			}
			else {
				return TERMINAL_OK;
			}
		}
		else {
			return TERMINAL_OK;
		}
	}
	else {
		return TERMINAL_OK;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData){
    printf(">>Please Enter The Transaction Amount :");
    scanf("%f",&termData->transAmount);
    if((termData->transAmount) <=0)
        return INVALID_AMOUNT;
    return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData){
    if((termData->transAmount) > (termData->maxTransAmount))
        return EXCEED_MAX_AMOUNT;
    return TERMINAL_OK;
}
EN_terminalError_t setMaxAmount(ST_terminalData_t *termData , float maxAmount){

    if(maxAmount <= 0)
        return INVALID_MAX_AMOUNT;
    termData->maxTransAmount=maxAmount;
    return TERMINAL_OK;
}
