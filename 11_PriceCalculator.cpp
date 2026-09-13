//11_PriceCalculator.cpp: Calculates ticket prices based on seat types.

//SOLID Single Responsibility Principle (SRP):
//PriceCalculator only calculates prices. It does not book seats, process payments, or print tickets.

//Compile-Time Polymorphism: calculate() is overloaded for a single ShowSeat and a vector of ShowSeat pointers.

#include <vector>

class PriceCalculator{
public:
    static double priceForSeatType(SeatType seatType){
        switch (seatType){
            case SeatType::SILVER:   return SILVER_PRICE;
            case SeatType::GOLD:     return GOLD_PRICE;
            case SeatType::PLATINUM: return PLATINUM_PRICE;
        }
        return ZERO_AMOUNT;
    }

    //Compile-time Polymorphism: overloaded calculate for a single seat.
    static double calculate(const ShowSeat& showSeat){
        return priceForSeatType(showSeat.getSeatType());
    }

    //Compile-time Polymorphism: overloaded calculate for multiple seats.
    static double calculate(const std::vector<ShowSeat*>& showSeats){
        double total = ZERO_AMOUNT;
        for (const auto* showSeat : showSeats){
            total += calculate(*showSeat);
        }
        return total;
    }
};
