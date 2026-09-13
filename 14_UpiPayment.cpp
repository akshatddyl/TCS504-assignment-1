//14_UpiPayment.cpp: Concrete strategy for UPI payments.
//Inheritance: UpiPayment inherits publicly from Payment.
//Strategy Pattern showcase
#include <string>
#include <iostream>

class UpiPayment : public Payment{  //inheritance from payment
private:
    std::string upiId;

    bool isValidUpiId() const{
        if (isBlank(upiId)){
            return false;
        }
        bool isFailedId = (upiId == FAILED_UPI_ID);
        return !isFailedId;
    }

public:
    explicit UpiPayment(std::string upiId)
        : upiId(std::move(upiId))
    {}

    bool pay(double amount) override{
        bool isAmountValid = isValidAmount(amount);
        bool isUpiValid = isValidUpiId();
        if (!isAmountValid || !isUpiValid){
            std::cout << "UPI payment failed.\n";
            return false;
        }
        std::cout << "UPI payment of " << CURRENCY_SYMBOL
                  << amount << " successful via " << upiId << ".\n";
        return true;
    }
    std::string getMethodName() const override{
        return "UPI";
    }
};
