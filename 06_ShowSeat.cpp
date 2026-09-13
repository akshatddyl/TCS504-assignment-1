//06_ShowSeat.cpp tracks the availability of a seat for a specific show.
//encapsulation: status is private and modified only through book()/release().
#include <string>

class ShowSeat{
private:
    Seat* seat;
    //Encapsulation: status is private.
    //It can only be changed through the public book() and release() methods.
    SeatStatus status;
public:
    explicit ShowSeat(Seat* seat)
        : seat(seat),
          status(SeatStatus::AVAILABLE)
    {}
    bool isAvailable() const{
        return status == SeatStatus::AVAILABLE;
    }
    bool isBooked() const{
        return status == SeatStatus::BOOKED;
    }
    //Encapsulation: controlled state transition to BOOKED.
    bool book(){
        if (isBooked()){
            return false;
        }
        status = SeatStatus::BOOKED;
        return true;
    }
    //Encapsulation: controlled state transition to AVAILABLE.
    bool release(){
        if (isAvailable()){
            return false;
        }
        status = SeatStatus::AVAILABLE;
        return true;
    }
    std::string getSeatNumber() const{
        return seat->getSeatNumber();
    }
    
    SeatType getSeatType() const{
        return seat->getSeatType();
    }
};
