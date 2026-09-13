//CashPayment.cpp: Concrete strategy for cash payments.
//Inheritance: CashPayment inherits publicly from Payment.

#include <iostream>
class CashPayment : public Payment{  //inheritance from Payment
private:
    double cashReceived;

public:
    explicit CashPayment(double cashReceived)
        : cashReceived(cashReceived)
    {}

    bool pay(double amount) override{
        bool isAmountValid = isValidAmount(amount);
        bool isCashSufficient = (cashReceived >= amount);
        if (!isAmountValid || !isCashSufficient){
            std::cout << "Cash payment failed. Insufficient cash.\n";
            return false;
        }
        double change = cashReceived - amount;
        std::cout << "Cash payment of " << CURRENCY_SYMBOL
                  << amount << " successful.";
        if (change > ZERO_AMOUNT){
            std::cout << " Change: " << CURRENCY_SYMBOL << change;
        }
        std::cout << "\n";
        return true;
    }

    std::string getMethodName() const override{
        return "Cash";
    }
};
