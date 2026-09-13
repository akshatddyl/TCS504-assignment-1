// Payment.cpp abstract base class for all payment methods.
// OOP Abstraction: Payment defines a pure virtual interface.

// Strategy Pattern: Payment is the strategy interface.
// Concrete strategies (UpiPayment, CardPayment, CashPayment) implement pay().


// SOLID Open/Closed Principle (OCP):
//   New payment methods can be added by creating a new subclass of Payment
//   without modifying BookingService or any existing payment class.
//
// SOLID Dependency Inversion Principle (DIP):
//   BookingService depends on this abstraction (Payment&), not on
//   concrete payment classes.
//
// SOLID Interface Segregation Principle (ISP):
//   refund() is NOT added here because refunds are not required.
//   Not all payment types need refund capability, so forcing it
//   would violate ISP.

#include <string>

class Payment{
public:
    //pure virtual methods makes Payment abstract.
    virtual bool pay(double amount) = 0;
    virtual std::string getMethodName() const = 0;

    //virtual destructor for safe deletion.
    virtual ~Payment() = default;

protected:
    bool isValidAmount(double amount) const
    {
        return amount > ZERO_AMOUNT;
    }
};