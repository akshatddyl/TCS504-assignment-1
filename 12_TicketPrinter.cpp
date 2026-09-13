// TicketPrinter.cpp: prints a formatted ticket for a booking.

// SOLID Single Responsibility Principle (SRP): TicketPrinter only prints. It does not modify bookings or seats.

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

class TicketPrinter{
private:
    static constexpr const char* TICKET_HEADER =
        "================ TICKET =================";
    static constexpr const char* TICKET_FOOTER =
        "=========================================";

    void printHeader() const{
        std::cout << colorText("\n" + std::string(TICKET_HEADER) + "\n", TerminalColor::BOLD_GREEN);
    }

    void printFooter() const{ 
        std::cout << colorText(std::string(TICKET_FOOTER) + "\n\n", TerminalColor::BOLD_GREEN);
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
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << amount;
        std::cout << colorText("Total Amount   : ", TerminalColor::CYAN) 
                  << CURRENCY_SYMBOL << colorText(oss.str(), TerminalColor::BOLD_GREEN) << "\n";
    }

    void printBody(const Booking& booking) const{
        std::cout << colorText("Booking ID     : ", TerminalColor::CYAN) << booking.getBookingId() << "\n";
        std::cout << colorText("Movie          : ", TerminalColor::CYAN) << colorText(booking.getShow()->getMovie()->getTitle(), TerminalColor::BOLD) << "\n";
        std::cout << colorText("Screen         : ", TerminalColor::CYAN) << booking.getShow()->getScreen()->getScreenNumber() << "\n";
        std::cout << colorText("Start Time     : ", TerminalColor::CYAN) << booking.getShow()->getStartTime() << "\n";
        std::cout << colorText("Seats          : ", TerminalColor::CYAN) << colorText(buildSeatListText(booking), TerminalColor::YELLOW) << "\n";
        printAmount(booking.getTotalAmount());
        std::cout << colorText("Payment Method : ", TerminalColor::CYAN) << booking.getPaymentMethodName() << "\n";
    }

public:
    void print(const Booking& booking) const{
        printHeader();
        printBody(booking);
        printFooter();
    }
};