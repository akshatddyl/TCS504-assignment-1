//CardPayment.cpp: concrete strategy for card payments.
//Inheritance: CardPayment inherits publicly from Payment.
#include <string>
#include <iostream>

class CardPayment : public Payment{  //inheritance from Payment
private:
    std::string cardNumber;

    bool isValidCardNumber() const{
        if (isBlank(cardNumber)){
            return false;
        }
        bool startsWithFailedPrefix = startsWith(cardNumber, FAILED_CARD_PREFIX);
        return !startsWithFailedPrefix;
    }

public:
    explicit CardPayment(std::string cardNumber)
        : cardNumber(std::move(cardNumber))
    {}
    bool pay(double amount) override{
        bool isAmountValid = isValidAmount(amount);
        bool isCardValid = isValidCardNumber();
        if (!isAmountValid || !isCardValid){
            std::cout << "Card payment failed.\n";
            return false;
        }
        std::cout << "Card payment of " << CURRENCY_SYMBOL
                  << amount << " successful.\n";
        return true;
    }
    std::string getMethodName() const override{
        return "Card";
    }
};
