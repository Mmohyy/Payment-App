#include "server.h"

ST_accountsDB_t database[255] = {
    {3000.00,BLOCKED,"1234567891011121301"},
    {4000.00,RUNNING,"1411234567891018112"},
    {5000.00,RUNNING,"2131234567891017123"},
    {7000.00,BLOCKED,"1111234567891029134"},
    {8000.00,RUNNING,"9101234567811121745"},
    {10000.00,BLOCKED,"6781234591011128156"},
    {13000.00,BLOCKED,"4561237891011151167"},
    {14000.00,RUNNING,"7812345678910171178"},
    {17000.00,BLOCKED,"1489456789101141189"},
    {19000.00,RUNNING,"0154567891011121190"},
};
ST_transaction_t transactionsDB[255]={0};

static uint8_t sequence_no = 0;

EN_transState_t recieveTransactionData(ST_transaction_t *transData){

    for(uint8_t i=0;i<10;i++){ // implemented accounts

        if ((strcmp(transData->cardHolderData.primaryAccountNumber,database[i].primaryAccountNumber))!=0)
            return FRAUD_CARD;

        else{

            if ((database[i].balance) < (transData->terminalData.transAmount))
                return DECLINED_INSUFFECIENT_FUND;
            else if ((database[i].state) == BLOCKED)
                return DECLINED_STOLEN_CARD;
            else if ((saveTransaction(transData)) == SAVING_FAILED)
                return INTERNAL_SERVER_ERROR;
            else
                return APPROVED;
            }
    }
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence){
    //Search for the PAN of the card in the DB
    for(int i=0;i<10;i++)
        if(strcmp(cardData->primaryAccountNumber,database[i].primaryAccountNumber) == 0){
            accountRefrence = &database[i];
            return SERVER_OK;
        }

    accountRefrence = NULL;
    return ACCOUNT_NOT_FOUND;
}


EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence){
    if(accountRefrence->state == BLOCKED)
        return BLOCKED_ACCOUNT;
    return SERVER_OK;
}


EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountRefrence){
    if((termData->transAmount) > (accountRefrence->balance))
        return LOW_BALANCE;
    return SERVER_OK;
}


EN_serverError_t saveTransaction(ST_transaction_t *transData){
    EN_transState_t result = recieveTransactionData(transData);
    sequence_no++;
	 if (sequence_no > 255)
		 return SAVING_FAILED;

	 else {
			 transactionsDB[sequence_no - 1] = *transData;
			 transactionsDB[sequence_no - 1].transState = result;
			 transactionsDB[sequence_no - 1].transactionSequenceNumber = sequence_no;
		 }
    return SERVER_OK;
}

void listSavedTransactions(void){
	for(uint8_t i=0;i<sequence_no;i++){

			printf("\n#######################\n");
			printf("Transaction Sequence Number : %d\n",sequence_no);
			printf("Transaction Date : %s\n",transactionsDB[i].terminalData.transactionDate);
			printf("Transaction Amount : %.02f\n",transactionsDB[i].terminalData.transAmount);
			printf("Transaction State : %d\n",database[i].state);
			printf("Terminal Max Amount : %.02f\n",transactionsDB[i].terminalData.maxTransAmount);
			printf("Card Holder Name : %s\n",transactionsDB[i].cardHolderData.cardHolderName);
			printf("PAN : %s\n",transactionsDB[i].cardHolderData.primaryAccountNumber);
			printf("Card Expiration Data : %s\n",transactionsDB[i].cardHolderData.cardExpirationDate);
			printf("#######################\n");

    }
}
