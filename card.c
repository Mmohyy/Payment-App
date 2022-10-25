
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){
    printf(">>Please Enter The Card Holder Name: ");
    gets(cardData->cardHolderName);
    if((cardData->cardHolderName) == NULL || strlen(cardData->cardHolderName)<20 || strlen(cardData->cardHolderName)>24)
        return WRONG_NAME;
    return CARD_OK ;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){
    printf(">>Please Enter The Expiration date (MM/YY): ");
    gets(cardData->cardExpirationDate);
    if((cardData->cardExpirationDate) == NULL || strlen(cardData->cardExpirationDate)!= 5 || cardData->cardExpirationDate[2] != '/')
        return WRONG_EXP_DATE;
    return CARD_OK ;
}
EN_cardError_t getCardPAN(ST_cardData_t *cardData){
    printf(">>Please Enter The Card PAN : ");
    gets(cardData->primaryAccountNumber);
    if((cardData->primaryAccountNumber) == NULL || strlen(cardData->primaryAccountNumber)< 16 || strlen(cardData->primaryAccountNumber) > 19)
        return WRONG_PAN;
    return CARD_OK;
}
