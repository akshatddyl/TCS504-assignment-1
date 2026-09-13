// 02Seat.cpp tells a physical seat in a screen.
//it represents a physical seat only but it does not know availability of the seat.
#include <string>

class Seat{
private:
    std::string seatNumber;
    SeatType seatType;

public:
    Seat(std::string seatNumber, SeatType seatType): seatNumber(std::move(seatNumber)),seatType(seatType){}
    std::string getSeatNumber() const{
        return this->seatNumber;
    }

    SeatType getSeatType() const{
        return this->seatType;
    }
};
