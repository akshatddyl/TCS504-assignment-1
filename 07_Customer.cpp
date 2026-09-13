// Customer.cpp represents a customer booking tickets.
#include <string>
class Customer{
private:
    std::string name;
    std::string phone;

public:
    Customer(std::string name, std::string phone)
        : name(std::move(name)),
          phone(std::move(phone)){}
          
    std::string getName() const{
        return this->name;
    }
    std::string getPhone() const{
        return this->phone;
    }
};