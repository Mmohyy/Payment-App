#include "app.h"

void appStart(void){
    // data definition
    ST_cardData_t cardData;
	EN_cardError_t cardError;

	ST_terminalData_t terminalData;
	EN_terminalError_t terminalError;
    float maxAmount;
    ST_transaction_t transData;
    EN_transState_t transactionError;
	EN_serverError_t serverError;
    ST_accountsDB_t *accountRefrence = &database;

    printf("\t***** Welcome to our System *****\n");

	//Taking card info
	do{
		cardError = getCardHolderName(&cardData);
		if (cardError == WRONG_NAME)
			printf("** Wrong Name\n");
	}while (cardError != CARD_OK);

	do{
		cardError = getCardExpiryDate(&cardData);
		if (cardError == WRONG_EXP_DATE)
			printf("** Wrong Expiration Date\n");
	}while (cardError != CARD_OK);

	do{
		cardError = getCardPAN(&cardData);
		if (cardError == WRONG_PAN)
			printf("** Wrong PAN\n");
	}while (cardError != CARD_OK);
    printf("\n******************************************************************\n\n");

    //Terminal info
	do{
		terminalError = getTransactionDate(&terminalData);
		if (terminalError == WRONG_DATE)
			printf("** Wrong Transaction Date\n");
	}while (terminalError != TERMINAL_OK);

	terminalError = isCardExpired(&cardData, &terminalData);
	if (terminalError == EXPIRED_CARD) {
		printf("\n** Expired Card\n");
		return ;
	}

	do {
		terminalError = getTransactionAmount(&terminalData);
		if (terminalError == INVALID_AMOUNT)
			printf("** Invalid Amount\n");
	}while (terminalError != TERMINAL_OK);

	do{
        float maxAmount=0;
        printf(">>Enter maximum Amount : ");
        scanf("%f",&maxAmount);
		terminalError = setMaxAmount(&terminalData, maxAmount);
		if (terminalError == INVALID_MAX_AMOUNT)
			printf("** Invalid Maximum Amount\n");
	}while(terminalError != TERMINAL_OK);

	terminalError = isBelowMaxAmount(&terminalData);
	if (terminalError == EXCEED_MAX_AMOUNT) {
		printf("** Exceed Max Amount\n");
		return ;
	}
	printf("\n******************************************************************\n\n");
    //Server operation

    transactionError = recieveTransactionData(&transData);
    switch (transactionError){
        case FRAUD_CARD:
            printf("Declined Fraud Card !!\n"); break;
        case DECLINED_INSUFFECIENT_FUND:
             printf("Declined Insuffecient Fund !!"); break;
        case DECLINED_STOLEN_CARD:
             printf("Declined Stolen Card !!\n"); break;
        case INTERNAL_SERVER_ERROR:
             printf("Declined Internal Server Error !!\n"); break;
        case APPROVED:
             printf("Approved Transaction.\n"); break;
    }

	serverError = isValidAccount(&cardData,accountRefrence);
	if (serverError == ACCOUNT_NOT_FOUND) {
		printf("** Invalid Account\n");
		return ;
	}
    serverError = isBlockedAccount(accountRefrence);
	if (serverError == BLOCKED_ACCOUNT) {
		printf("** Blocked Account\n");
		return ;
	}
	serverError = isAmountAvailable(&terminalData,accountRefrence);
	if (serverError == LOW_BALANCE) {
		printf("** Low Balance\n");
		return ;
	}
	serverError = saveTransaction(&transData);
	if (serverError == SAVING_FAILED) {
		printf("** Saving Failed !!\n");
		return ;
	}else if(serverError == SERVER_OK)
        printf("** Saved Successfully\n");

    listSavedTransactions();
    return ;
}
