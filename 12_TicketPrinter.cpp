// TicketPrinter.cpp: prints a formatted ticket for a booking.

// SOLID Single Responsibility Principle (SRP): TicketPrinter only prints. It does not modify bookings or seats.

#include <iostream>
#include <iomanip>
#include <string>

class TicketPrinter{
private:
    static constexpr const char* TICKET_HEADER =
        "================ TICKET =================";
    static constexpr const char* TICKET_FOOTER =
        "=========================================";

    void printHeader() const{
        std::cout << "\n" << TICKET_HEADER << "\n";
    }

    void printFooter() const{ 
        std::cout << TICKET_FOOTER << "\n\n";
    }

    std::string buildSeatListText(const Booking& booking) const{
        std::string seatList;
        const auto& seats = booking.getSeats();
        for (std::size_t i = 0; i < seats.size(); ++i)
        {
            seatList += seats[i]->getSeatNumber();
            bool isLastSeat = (i == seats.size() - 1);
            if (!isLastSeat)
            {
                seatList += ", ";
            }
        }
        return seatList;
    }

    void printAmount(double amount) const{
        std::cout << "Total Amount   : " << CURRENCY_SYMBOL
                  << std::fixed << std::setprecision(2) << amount << "\n";
    }

    void printBody(const Booking& booking) const{
        std::cout << "Booking ID     : " << booking.getBookingId() << "\n";
        std::cout << "Movie          : " << booking.getShow()->getMovie()->getTitle() << "\n";
        std::cout << "Screen         : " << booking.getShow()->getScreen()->getScreenNumber() << "\n";
        std::cout << "Start Time     : " << booking.getShow()->getStartTime() << "\n";
        std::cout << "Seats          : " << buildSeatListText(booking) << "\n";
        printAmount(booking.getTotalAmount());
        std::cout << "Payment Method : " << booking.getPaymentMethodName() << "\n";
    }

public:
    void print(const Booking& booking) const{
        printHeader();
        printBody(booking);
        printFooter();
    }
};
