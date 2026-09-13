//08_Booking.cpp: represents a confirmed or cancelled booking.
//Booking class does not process payment. Booking does not print ticket.
//Static member used for auto-generating booking IDs.
#include <string>
#include <vector>

class Booking{
public:
    //nested parameter struct to avoid a constructor with many parameters.
    struct Details{
        Show* show;
        Customer customer;
        std::vector<ShowSeat*> seats;
        double totalAmount;
        std::string paymentMethodName;
    };

private:
    //static member for booking ID generation.
    static int nextBookingId;

    int bookingId;
    Show* show;                        //aggregation: booking references Show
    Customer customer;
    std::vector<ShowSeat*> seats;      //aggregation: booking references ShowSeats
    double totalAmount;
    BookingStatus status;
    std::string paymentMethodName;

public:
    explicit Booking(const Details& details)
        : bookingId(nextBookingId++),
          show(details.show),
          customer(details.customer),
          seats(details.seats),
          totalAmount(details.totalAmount),
          status(BookingStatus::CONFIRMED),
          paymentMethodName(details.paymentMethodName)
    {}
    int getBookingId() const{
        return this->bookingId;
    }

    Show* getShow() const{
        return show;
    }

    const Customer& getCustomer() const{
        return customer;
    }

    const std::vector<ShowSeat*>& getSeats() const{
        return seats;
    }

    double getTotalAmount() const{
        return totalAmount;
    }

    std::string getPaymentMethodName() const{
        return paymentMethodName;
    }

    bool isConfirmed() const{
        return status == BookingStatus::CONFIRMED;
    }

    void markCancelled(){
        status = BookingStatus::CANCELLED;
    }
};

//static member
int Booking::nextBookingId = INITIAL_BOOKING_ID;
